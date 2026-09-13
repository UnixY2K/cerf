#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../peripherals/peripheral_base.h"
#include "../../peripherals/peripheral_dispatcher.h"

#include <cstdint>

namespace {

constexpr uint32_t kBase = 0xAC400800u;
constexpr uint32_t kSize = 0x00002000u;

/* Linux arch/arm/mach-msm dma.c: MSM_DMOV_CHANNEL_COUNT. */
constexpr uint32_t kChannelCount = 16u;

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_RSLT_CONF, addressed as
   DMOV_ADDR(0x300, ch) = 0x300 + (ch << 2). */
constexpr uint32_t kRsltConf     = 0x300u;
constexpr uint32_t kRsltConfLast = kRsltConf + 4u * (kChannelCount - 1u);

/* Linux arch/arm/mach-msm include mach dma.h: DMOV_RSLT_CONF_IRQ_EN is bit 0
   and DMOV_RSLT_CONF_FORCE_FLUSH_RSLT is bit 1. */
constexpr uint32_t kRsltConfServed = (1u << 0) | (1u << 1);

class Msm8255Dmov : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        return emu_.Get<BoardContext>().GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioBase() const override { return kBase; }
    uint32_t MmioSize() const override { return kSize; }

    uint32_t ReadWord(uint32_t addr) override {
        HaltUnsupportedAccess("ReadWord", addr, 0);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t off = addr - kBase;
        if (IsRsltConf(off) && (value & ~kRsltConfServed) == 0u) {
            return;
        }
        HaltUnsupportedAccess("WriteWord", addr, value);
    }

private:
    static bool IsRsltConf(uint32_t off) {
        return off >= kRsltConf && off <= kRsltConfLast && (off & 3u) == 0u;
    }
};

}  // namespace

REGISTER_SERVICE(Msm8255Dmov);
