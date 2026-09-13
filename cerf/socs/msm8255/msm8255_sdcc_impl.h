#pragma once

#include "../../peripherals/peripheral_base.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../peripherals/peripheral_dispatcher.h"

#include <cstdint>

namespace cerf_msm8255_sdcc_detail {

/* Linux drivers/mmc/host msmsdcc.h: MMCICOMMAND, MMCIDATACTRL, MMCICLEAR,
   MMCIMASK0 and MMCIMASK1. */
constexpr uint32_t kCommand  = 0x00Cu;
constexpr uint32_t kDataCtrl = 0x02Cu;
constexpr uint32_t kClear    = 0x038u;
constexpr uint32_t kMask0    = 0x03Cu;
constexpr uint32_t kMask1    = 0x040u;

/* Linux drivers/mmc/host msmsdcc.h: MCI_CLEAR_STATIC_MASK names every bit a
   write to MMCICLEAR is defined to clear. */
constexpr uint32_t kClearStaticMask =
    (1u << 0) | (1u << 1) | (1u << 2) | (1u << 3) | (1u << 4) | (1u << 5) |
    (1u << 6) | (1u << 7) | (1u << 8) | (1u << 9) | (1u << 10) | (1u << 22) |
    (1u << 23) | (1u << 24) | (1u << 25) | (1u << 26);

constexpr uint32_t kQuiescent = 0u;

template <uint32_t kBase, uint32_t kSize>
class Msm8255SdccWindowBase : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        auto* bd = emu_.TryGet<BoardContext>();
        return bd && bd->GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioBase() const override { return kBase; }
    uint32_t MmioSize() const override { return kSize; }

    uint32_t ReadWord(uint32_t addr) override {
        HaltUnsupportedAccess("ReadWord", addr, 0u);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        switch (addr - kBase) {
        case kCommand:
        case kDataCtrl:
        case kMask0:
        case kMask1:
            if (value == kQuiescent) {
                return;
            }
            break;
        case kClear:
            if ((value & ~kClearStaticMask) == 0u) {
                return;
            }
            break;
        default:
            break;
        }
        HaltUnsupportedAccess("WriteWord", addr, value);
    }
};

}  // namespace cerf_msm8255_sdcc_detail
