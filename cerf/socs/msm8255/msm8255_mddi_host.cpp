#include "../../peripherals/peripheral_base.h"

#include "msm8255_mddi_client.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../cpu/emulated_memory.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../irq_controller.h"
#include "../guest_cpu_reset.h"

#include <atomic>
#include <cstdint>

namespace {

constexpr uint32_t kMddiBase = 0xAD600000u;
constexpr uint32_t kMddiSize = 0x00000100u;

constexpr uint32_t kWordCount = kMddiSize / 4u;

/* Linux arch/arm/mach-msm irqs-7x30.h: INT_PMDH, which INT_MDDI_PRI aliases. */
constexpr uint32_t kVicLine = 44u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h: the MDDI host register
   offsets, MDDI_CMD through MDDI_PAD_CAL. */
constexpr uint32_t kRegCmd            = 0x00u;
constexpr uint32_t kRegVersion        = 0x04u;
constexpr uint32_t kRegBps            = 0x10u;
constexpr uint32_t kRegSpm            = 0x14u;
constexpr uint32_t kRegInt            = 0x18u;
constexpr uint32_t kRegInten          = 0x1Cu;
constexpr uint32_t kRegRevPtr         = 0x20u;
constexpr uint32_t kRegRevSize        = 0x24u;
constexpr uint32_t kRegStat           = 0x28u;
constexpr uint32_t kRegRevRateDiv     = 0x2Cu;
constexpr uint32_t kRegRevCrcErr      = 0x30u;
constexpr uint32_t kRegTa1Len         = 0x34u;
constexpr uint32_t kRegTa2Len         = 0x38u;
constexpr uint32_t kRegRevPktCnt      = 0x44u;
constexpr uint32_t kRegDriveHi        = 0x48u;
constexpr uint32_t kRegDriveLo        = 0x4Cu;
constexpr uint32_t kRegDispWake       = 0x50u;
constexpr uint32_t kRegRevEncapSz     = 0x54u;
constexpr uint32_t kRegRtdVal         = 0x58u;
constexpr uint32_t kRegPadCtl         = 0x68u;
constexpr uint32_t kRegDriverStartCnt = 0x6Cu;
constexpr uint32_t kRegCoreVer        = 0x8Cu;
constexpr uint32_t kRegPadIoCtl       = 0xA0u;
constexpr uint32_t kRegPadCal         = 0xA4u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h: the MDDI_CMD_* command words
   the host accepts, where the low bit of the hibernate form asks the link to
   hibernate after one empty subframe. */
constexpr uint32_t kCmdPowerdown        = 0x0100u;
constexpr uint32_t kCmdHibernate        = 0x0300u;
constexpr uint32_t kCmdHibernateAfterSf = 0x0301u;
constexpr uint32_t kCmdReset            = 0x0400u;
constexpr uint32_t kCmdDispListen       = 0x0500u;
constexpr uint32_t kCmdDispIgnore       = 0x0501u;
constexpr uint32_t kCmdGetClientCap     = 0x0601u;
constexpr uint32_t kCmdSendRtd          = 0x0700u;
constexpr uint32_t kCmdLinkActive       = 0x0900u;
constexpr uint32_t kCmdPeriodicRevEncap = 0x0A00u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h: MDDI_INT_LINK_ACTIVE and
   MDDI_INT_IN_HIBERNATION, the two the MDDI_INT_LINK_STATE_CHANGES pair names. */
constexpr uint32_t kIntLinkActive    = 0x2000u;
constexpr uint32_t kIntInHibernation = 0x4000u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h: MDDI_STAT_LINK_ACTIVE and
   MDDI_STAT_IN_HIBERNATION. */
constexpr uint32_t kStatLinkActive    = 0x0001u;
constexpr uint32_t kStatInHibernation = 0x0010u;

constexpr uint32_t kIntRevEncapDone = 0x00080000u;

constexpr uint32_t kCapPacketType   = 66u;
constexpr uint32_t kCapPacketBytes  = 76u;
constexpr uint32_t kCapPacketLength = kCapPacketBytes - 2u;
constexpr uint32_t kCapWords        = kCapPacketBytes / 4u;

constexpr uint32_t kCapWordBitmap  = 4u;
constexpr uint32_t kCapWordWindow  = 5u;
constexpr uint32_t kCapWordMfr     = 15u;
constexpr uint32_t kCapWordProduct = 16u;

constexpr uint32_t kRevPacketOne = 1u;
constexpr uint32_t kRevNoCrcErrors = 0u;

constexpr uint32_t kCoreVersion = 0x28u;

constexpr uint32_t kRegReset = 0u;

class Msm8255MddiHost : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        return emu_.Get<BoardContext>().GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        ResetState();
        emu_.Get<GuestCpuReset>().RegisterResetListener([this](ResetLineKind) {
            ResetState();
            PublishLine();
        });
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioBase() const override { return kMddiBase; }
    uint32_t MmioSize() const override { return kMddiSize; }

    uint32_t ReadWord(uint32_t addr) override {
        const uint32_t off = addr - MmioBase();
        if (off == kRegCoreVer) return kCoreVersion;
        if (off == kRegPadCtl || off == kRegInten || off == kRegInt ||
            off == kRegStat || off == kRegRtdVal ||
            off == kRegRevPktCnt || off == kRegRevCrcErr) {
            return Reg(off);
        }
        HaltUnsupportedAccess("ReadWord", addr, 0);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t off = addr - MmioBase();
        if (off == kRegCmd) {
            WriteCommand(addr, value);
            return;
        }
        /* Linux arch/arm/mach-msm video-msm mddihosti.c mddi_host_isr writes the
           bits it has just handled back to INT, so a set bit clears that
           source. */
        if (off == kRegInt) {
            SetReg(kRegInt, Reg(kRegInt) & ~value);
            PublishLine();
            return;
        }
        if (off == kRegInten) {
            SetReg(kRegInten, value);
            PublishLine();
            return;
        }
        if (!IsConfigReg(off)) {
            HaltUnsupportedAccess("WriteWord", addr, value);
        }
        SetReg(off, value);
    }

    void SaveState(StateWriter& w) override {
        for (uint32_t i = 0; i < kWordCount; ++i) {
            w.Write<uint32_t>(regs_[i].load(std::memory_order_acquire));
        }
    }

    void RestoreState(StateReader& r) override {
        for (uint32_t i = 0; i < kWordCount; ++i) {
            uint32_t v = kRegReset;
            r.Read(v);
            regs_[i].store(v, std::memory_order_release);
        }
    }

    void PostRestore() override { PublishLine(); }

private:
    uint32_t Reg(uint32_t off) const {
        return regs_[off / 4u].load(std::memory_order_acquire);
    }

    void SetReg(uint32_t off, uint32_t value) {
        regs_[off / 4u].store(value, std::memory_order_release);
    }

    /* Linux arch/arm/mach-msm video-msm mddihosti.c mddi_host_isr takes the
       pending set as INT masked by INTEN, so the line follows that product. */
    void PublishLine() {
        auto& vic = emu_.Get<IrqController>();
        if ((Reg(kRegInt) & Reg(kRegInten)) != 0u) {
            vic.AssertIrq(kVicLine);
        } else {
            vic.DeAssertIrq(kVicLine);
        }
    }

    void EnterLinkActive() {
        if ((Reg(kRegStat) & kStatLinkActive) != 0u) return;
        SetReg(kRegStat,
               (Reg(kRegStat) & ~kStatInHibernation) | kStatLinkActive);
        SetReg(kRegInt,
               (Reg(kRegInt) & ~kIntInHibernation) | kIntLinkActive);
        PublishLine();
    }

    void EnterHibernation() {
        if ((Reg(kRegStat) & kStatInHibernation) != 0u) return;
        SetReg(kRegStat,
               (Reg(kRegStat) & ~kStatLinkActive) | kStatInHibernation);
        SetReg(kRegInt,
               (Reg(kRegInt) & ~kIntLinkActive) | kIntInHibernation);
        PublishLine();
    }

    void ResetState() {
        for (uint32_t i = 0; i < kWordCount; ++i) {
            regs_[i].store(kRegReset, std::memory_order_release);
        }
        SetReg(kRegStat, kStatInHibernation);
    }

    void DeliverClientCapability() {
        auto* client = emu_.TryGet<Msm8255MddiClient>();
        if (!client) {
            emu_.Get<Fatal>().Die(
                "msm8255 mddi host: the guest asked the link for its client "
                "capability and this board declares no mddi client");
        }
        const uint32_t rev_pa = Reg(kRegRevPtr);
        if (rev_pa == 0u) {
            emu_.Get<Fatal>().Die(
                "msm8255 mddi host: the guest asked the link for its client "
                "capability before it programmed the reverse-packet pointer");
        }

        const uint32_t rev_bytes = Reg(kRegRevSize);
        if (rev_bytes < kCapPacketBytes) {
            emu_.Get<Fatal>().Die(
                "msm8255 mddi host: the guest declared a %u-byte reverse packet "
                "buffer and the client capability packet is %u bytes",
                rev_bytes, kCapPacketBytes);
        }

        const Msm8255MddiClientCapability cap = client->Capability();
        uint32_t words[kCapWords] = {};
        words[0] = kCapPacketLength | (kCapPacketType << 16);
        words[kCapWordBitmap] =
            cap.bitmap_width | (static_cast<uint32_t>(cap.bitmap_height) << 16);
        words[kCapWordWindow] =
            cap.display_window_width |
            (static_cast<uint32_t>(cap.display_window_height) << 16);
        words[kCapWordMfr]     = static_cast<uint32_t>(cap.mfr_name) << 16;
        words[kCapWordProduct] = cap.product_code;

        auto& mem = emu_.Get<EmulatedMemory>();
        for (uint32_t i = 0; i < kCapWords; ++i) {
            mem.WriteWord(rev_pa + 4u * i, words[i]);
        }

        SetReg(kRegRevCrcErr, kRevNoCrcErrors);
        SetReg(kRegRevPktCnt, kRevPacketOne);
        SetReg(kRegInt, Reg(kRegInt) | kIntRevEncapDone);
        PublishLine();
    }

    void WriteCommand(uint32_t addr, uint32_t value) {
        switch (value) {
            case kCmdLinkActive:
                SetReg(kRegCmd, value);
                EnterLinkActive();
                return;
            case kCmdPowerdown:
            case kCmdHibernate:
            case kCmdHibernateAfterSf:
            case kCmdReset:
                SetReg(kRegCmd, value);
                EnterHibernation();
                return;
            case kCmdGetClientCap:
                SetReg(kRegCmd, value);
                DeliverClientCapability();
                return;
            case kCmdDispListen:
            case kCmdDispIgnore:
            case kCmdSendRtd:
            case kCmdPeriodicRevEncap:
                SetReg(kRegCmd, value);
                return;
            default:
                HaltUnsupportedAccess("WriteWord", addr, value);
        }
    }

    static bool IsConfigReg(uint32_t off) {
        switch (off) {
            case kRegVersion:
            case kRegBps:
            case kRegSpm:
            case kRegRevPtr:
            case kRegRevSize:
            case kRegRevRateDiv:
            case kRegTa1Len:
            case kRegTa2Len:
            case kRegDriveHi:
            case kRegDriveLo:
            case kRegDispWake:
            case kRegRevEncapSz:
            case kRegPadCtl:
            case kRegDriverStartCnt:
            case kRegPadIoCtl:
            case kRegPadCal:
                return true;
            default:
                return false;
        }
    }

    std::atomic<uint32_t> regs_[kWordCount] = {};
};

}

REGISTER_SERVICE(Msm8255MddiHost);
