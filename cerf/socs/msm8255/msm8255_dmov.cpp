#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../peripherals/peripheral_base.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../guest_cpu_reset.h"

#include <atomic>
#include <cstdint>

namespace {

/* Linux arch/arm/mach-msm msm_iomap-7x30.h: MSM_DMOV_PHYS. */
constexpr uint32_t kBase = 0xAC400000u;
constexpr uint32_t kSize = 0x00003000u;

/* Linux arch/arm/mach-msm dma.c: the data mover repeats its register file once
   per security domain, at 0x0000, 0x0400, 0x0800 and 0x0C00. */
constexpr uint32_t kSecurityDomain2 = 0x00000800u;

/* Linux arch/arm/mach-msm dma.c: MSM_DMOV_CHANNEL_COUNT. */
constexpr uint32_t kChannelCount = 16u;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_RSLT_CONF, addressed as
   DMOV_ADDR(0x300, ch) = 0x300 + (ch << 2), whose bit 0 it names
   DMOV_RSLT_CONF_IRQ_EN. */
constexpr uint32_t kRsltConf      = kSecurityDomain2 + 0x300u;
constexpr uint32_t kRsltConfLast  = kRsltConf + 4u * (kChannelCount - 1u);

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
        if (IsRsltConf(off)) {
            return rslt_conf_[Channel(off)].load(std::memory_order_acquire);
        }
        HaltUnsupportedAccess("ReadWord", addr, 0);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t off = addr - kBase;
        if (IsRsltConf(off)) {
            rslt_conf_[Channel(off)].store(value, std::memory_order_release);
            return;
        }
        HaltUnsupportedAccess("WriteWord", addr, value);
    }

    void SaveState(StateWriter& w) override {
        for (const auto& r : rslt_conf_) {
            w.Write<uint32_t>(r.load(std::memory_order_acquire));
        }
    }

    void RestoreState(StateReader& r) override {
        for (auto& reg : rslt_conf_) {
            uint32_t v = 0;
            r.Read(v);
            reg.store(v, std::memory_order_release);
        }
    }

private:
    static bool IsRsltConf(uint32_t off) {
        return off >= kRsltConf && off <= kRsltConfLast && (off & 3u) == 0u;
    }

    static uint32_t Channel(uint32_t off) { return (off - kRsltConf) / 4u; }

    void ResetState() {
        for (auto& r : rslt_conf_) {
            r.store(0u, std::memory_order_release);
        }
    }

    std::atomic<uint32_t> rslt_conf_[kChannelCount] = {};
};

}  // namespace

REGISTER_SERVICE(Msm8255Dmov);
