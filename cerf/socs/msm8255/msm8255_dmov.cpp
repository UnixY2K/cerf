#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../cpu/physical_bus.h"
#include "../../peripherals/peripheral_base.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../guest_cpu_reset.h"
#include "../irq_controller.h"

#include <cstdint>
#include <mutex>

namespace {

constexpr uint32_t kBase = 0xAC400800u;
constexpr uint32_t kSize = 0x00002000u;

/* Linux arch/arm/mach-msm dma.c: MSM_DMOV_CHANNEL_COUNT. */
constexpr uint32_t kChannelCount = 16u;

/* Linux arch/arm/mach-msm include mach dma.h: every channel register is
   DMOV_ADDR(off, ch) = off + (ch << 2) inside one security domain. */
constexpr uint32_t kRegCmdPtr      = 0x000u;
constexpr uint32_t kRegRslt        = 0x040u;
constexpr uint32_t kRegStatus      = 0x200u;
constexpr uint32_t kRegRsltConf    = 0x300u;
constexpr uint32_t kRegIsr         = 0x380u;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_RSLT_VALID, _ERROR,
   _FLUSH, _DONE and _USER. */
constexpr uint32_t kRsltValid = 1u << 31;
constexpr uint32_t kRsltDone  = 1u << 1;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_STATUS_CMD_PTR_RDY,
   DMOV_STATUS_RSLT_VALID, DMOV_STATUS_RSLT_COUNT and _CMD_COUNT. */
constexpr uint32_t kStatusCmdPtrRdy   = 1u << 0;
constexpr uint32_t kStatusRsltValid   = 1u << 1;
constexpr uint32_t kStatusRsltCountSh = 29u;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_RSLT_CONF_IRQ_EN,
   _FORCE_FLUSH_RSLT and _FORCE_TOP_PTR_RSLT. */
constexpr uint32_t kRsltConfIrqEn  = 1u << 0;
constexpr uint32_t kRsltConfFlush  = 1u << 1;
constexpr uint32_t kRsltConfServed = kRsltConfIrqEn | kRsltConfFlush;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_CMD_ADDR is addr >> 3 and
   the command type occupies bits 31:29 as DMOV_CMD_LIST, DMOV_CMD_PTR_LIST,
   DMOV_CMD_INPUT_CFG and DMOV_CMD_OUTPUT_CFG. */
constexpr uint32_t kCmdPtrTypeShift = 29u;
constexpr uint32_t kCmdPtrTypeList  = 0u;

/* Linux arch/arm/mach-msm include mach dma.h: CMD_PTR_ADDR is addr >> 3,
   CMD_PTR_LP marks the last pointer of the list and CMD_PTR_PT occupies
   bits 30:29. */
constexpr uint32_t kPtrAddrMask = 0x1FFFFFFFu;
constexpr uint32_t kPtrLast     = 1u << 31;
constexpr uint32_t kPtrType     = 3u << 29;

/* Linux arch/arm/mach-msm include mach dma.h: CMD_LC marks the last command,
   CMD_SAH and CMD_DAH hold the source and destination addresses, and the
   transfer mode occupies the low bits with CMD_MODE_SINGLE zero. */
constexpr uint32_t kCmdLast     = 1u << 31;
constexpr uint32_t kCmdDstHold  = 1u << 18;
constexpr uint32_t kCmdSrcHold  = 1u << 17;
constexpr uint32_t kCmdModeMask = 7u;
constexpr uint32_t kCmdModeSingle = 0u;
constexpr uint32_t kCmdActed = kCmdLast | kCmdDstHold | kCmdSrcHold |
                               kCmdModeMask;

/* Linux arch/arm/mach-msm irqs-7x30.h: INT_ADM_AARM is INT_ADM_SC2. */
constexpr int kVicLine = 64 + 15;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_STATUS_RSLT_COUNT reads
   bits 31:29, so a count above seven cannot be reported. */
constexpr uint32_t kRsltFifoDepth = 7u;

constexpr uint32_t kMaxPointers = 256u;
constexpr uint32_t kMaxCommands = 1024u;

class Msm8255Dmov : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        return emu_.Get<BoardContext>().GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        ResetState();
        emu_.Get<GuestCpuReset>().RegisterResetListener(
            [this](ResetLineKind) { ResetState(); });
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioBase() const override { return kBase; }
    uint32_t MmioSize() const override { return kSize; }

    uint32_t ReadWord(uint32_t addr) override {
        const uint32_t off = addr - kBase;
        if (off == kRegIsr) {
            std::lock_guard<std::mutex> g(lock_);
            return IsrWordLocked();
        }
        const uint32_t ch = ChannelOf(off);
        if (ch == kChannelCount) HaltUnsupportedAccess("ReadWord", addr, 0);
        const uint32_t reg = RegOf(off);
        std::lock_guard<std::mutex> g(lock_);
        if (reg == kRegStatus)   return StatusLocked(ch);
        if (reg == kRegRslt)     return PopResultLocked(ch);
        if (reg == kRegRsltConf) return chans_[ch].rslt_conf;
        HaltUnsupportedAccess("ReadWord", addr, 0);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t ch = ChannelOf(addr - kBase);
        if (ch == kChannelCount) HaltUnsupportedAccess("WriteWord", addr, value);
        const uint32_t reg = RegOf(addr - kBase);
        if (reg == kRegRsltConf) {
            if ((value & ~kRsltConfServed) != 0u) {
                HaltUnsupportedAccess("WriteWord", addr, value);
            }
            std::lock_guard<std::mutex> g(lock_);
            chans_[ch].rslt_conf = value;
            PublishLineLocked();
            return;
        }
        if (reg == kRegCmdPtr) {
            RunTransfer(ch, value);
            return;
        }
        HaltUnsupportedAccess("WriteWord", addr, value);
    }

    void SaveState(StateWriter& w) override {
        std::lock_guard<std::mutex> g(lock_);
        for (const Channel& c : chans_) {
            w.Write<uint32_t>(c.rslt_conf);
            w.Write<uint32_t>(c.count);
            w.Write<uint32_t>(c.head);
            for (uint32_t v : c.fifo) w.Write<uint32_t>(v);
        }
    }

    void RestoreState(StateReader& r) override {
        std::lock_guard<std::mutex> g(lock_);
        for (Channel& c : chans_) {
            r.Read(c.rslt_conf);
            r.Read(c.count);
            r.Read(c.head);
            for (uint32_t& v : c.fifo) r.Read(v);
            if (c.count > kRsltFifoDepth || c.head >= kRsltFifoDepth) {
                emu_.Get<Fatal>().Die(
                    "msm8255 dmov: restored channel result fifo carries count %u "
                    "head %u past its depth %u", c.count, c.head, kRsltFifoDepth);
            }
        }
    }

    void PostRestore() override {
        std::lock_guard<std::mutex> g(lock_);
        PublishLineLocked();
    }

private:
    struct Channel {
        uint32_t rslt_conf = 0u;
        uint32_t fifo[kRsltFifoDepth] = {};
        uint32_t head  = 0u;
        uint32_t count = 0u;
    };

    static uint32_t RegOf(uint32_t off) { return off & ~0x3Cu; }

    static uint32_t ChannelOf(uint32_t off) {
        if ((off & 3u) != 0u) return kChannelCount;
        const uint32_t ch = (off & 0x3Cu) / 4u;
        const uint32_t reg = RegOf(off);
        const bool known = reg == kRegCmdPtr || reg == kRegRslt ||
                           reg == kRegStatus || reg == kRegRsltConf;
        return known ? ch : kChannelCount;
    }

    uint32_t StatusLocked(uint32_t ch) const {
        uint32_t v = kStatusCmdPtrRdy;
        if (chans_[ch].count != 0u) {
            v |= kStatusRsltValid;
            v |= chans_[ch].count << kStatusRsltCountSh;
        }
        return v;
    }

    uint32_t PopResultLocked(uint32_t ch) {
        Channel& c = chans_[ch];
        if (c.count == 0u) return 0u;
        const uint32_t v = c.fifo[c.head];
        c.head = (c.head + 1u) % kRsltFifoDepth;
        --c.count;
        PublishLineLocked();
        return v;
    }

    void PushResultLocked(uint32_t ch, uint32_t value) {
        Channel& c = chans_[ch];
        if (c.count == kRsltFifoDepth) {
            emu_.Get<Fatal>().Die(
                "msm8255 dmov: channel %u result fifo overflowed at depth %u",
                ch, kRsltFifoDepth);
        }
        c.fifo[(c.head + c.count) % kRsltFifoDepth] = value;
        ++c.count;
        PublishLineLocked();
    }

    uint32_t IsrWordLocked() const {
        uint32_t v = 0u;
        for (uint32_t ch = 0; ch < kChannelCount; ++ch) {
            if (chans_[ch].count != 0u &&
                (chans_[ch].rslt_conf & kRsltConfIrqEn) != 0u) {
                v |= 1u << ch;
            }
        }
        return v;
    }

    void PublishLineLocked() {
        auto& vic = emu_.Get<IrqController>();
        if (IsrWordLocked() != 0u) {
            vic.AssertIrq(kVicLine);
        } else {
            vic.DeAssertIrq(kVicLine);
        }
    }

    uint32_t BusRead(uint32_t pa) {
        uint32_t v = 0;
        if (!emu_.Get<PhysicalBus>().Read(pa, BusWidth::Word, &v)) {
            emu_.Get<Fatal>().Die(
                "msm8255 dmov: command list read at 0x%08X reaches neither "
                "memory nor a peripheral", pa);
        }
        return v;
    }

    void Move(uint32_t cmd, uint32_t src, uint32_t dst, uint32_t len) {
        auto& bus = emu_.Get<PhysicalBus>();
        const bool hold_src = (cmd & kCmdSrcHold) != 0u;
        const bool hold_dst = (cmd & kCmdDstHold) != 0u;
        const BusWidth w = ((len | src | dst) & 3u) == 0u ? BusWidth::Word
                                                          : BusWidth::Byte;
        const uint32_t step = static_cast<uint32_t>(w);
        for (uint32_t done = 0; done < len; done += step) {
            const uint32_t s = hold_src ? src : src + done;
            const uint32_t d = hold_dst ? dst : dst + done;
            uint32_t v = 0;
            if (!bus.Read(s, w, &v) || !bus.Write(d, w, v)) {
                emu_.Get<Fatal>().Die(
                    "msm8255 dmov: transfer endpoint 0x%08X -> 0x%08X reaches "
                    "neither memory nor a peripheral", s, d);
            }
        }
    }

    void RunCommandArray(uint32_t pa) {
        for (uint32_t i = 0; i < kMaxCommands; ++i) {
            const uint32_t cmd = BusRead(pa);
            const uint32_t mode = cmd & kCmdModeMask;
            if (mode != kCmdModeSingle) {
                emu_.Get<Fatal>().Die(
                    "msm8255 dmov: command at 0x%08X selects transfer mode %u, "
                    "whose descriptor layout is not modeled", pa, mode);
            }
            if ((cmd & ~kCmdActed) != 0u) {
                emu_.Get<Fatal>().Die(
                    "msm8255 dmov: command word 0x%08X at 0x%08X carries fields "
                    "outside the address-hold, mode and last-command set this "
                    "engine acts on", cmd, pa);
            }
            Move(cmd, BusRead(pa + 4u), BusRead(pa + 8u), BusRead(pa + 12u));
            pa += 16u;
            if ((cmd & kCmdLast) != 0u) return;
        }
        emu_.Get<Fatal>().Die(
            "msm8255 dmov: command array passed %u entries with no last-command "
            "marker", kMaxCommands);
    }

    void RunTransfer(uint32_t ch, uint32_t value) {
        const uint32_t type = value >> kCmdPtrTypeShift;
        if (type != kCmdPtrTypeList) {
            emu_.Get<Fatal>().Die(
                "msm8255 dmov: command pointer 0x%08X selects command type %u, "
                "whose walk is not modeled", value, type);
        }
        uint32_t list = (value & kPtrAddrMask) << 3;
        for (uint32_t i = 0; i < kMaxPointers; ++i) {
            const uint32_t entry = BusRead(list);
            if ((entry & kPtrType) != 0u) {
                emu_.Get<Fatal>().Die(
                    "msm8255 dmov: pointer entry 0x%08X at 0x%08X carries a "
                    "pointer type this engine does not model", entry, list);
            }
            RunCommandArray((entry & kPtrAddrMask) << 3);
            if ((entry & kPtrLast) != 0u) {
                std::lock_guard<std::mutex> g(lock_);
                PushResultLocked(ch, kRsltValid | kRsltDone);
                return;
            }
            list += 4u;
        }
        emu_.Get<Fatal>().Die(
            "msm8255 dmov: pointer list passed %u entries with no last-pointer "
            "marker", kMaxPointers);
    }

    void ResetState() {
        std::lock_guard<std::mutex> g(lock_);
        for (Channel& c : chans_) c = Channel{};
    }

    std::mutex lock_;
    Channel    chans_[kChannelCount];
};

}  // namespace

REGISTER_SERVICE(Msm8255Dmov);
