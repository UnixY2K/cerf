#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc2Base = 0xA0500000u;
constexpr uint32_t kSdc2Size = 0x00000800u;

constexpr uint32_t kSdc2ResetClock = 131u;
constexpr uint32_t kSdc2SlotIndex  = 1u;
constexpr uint32_t kSdc2IrqSource0 = 98u;
constexpr uint32_t kSdc2IrqSource1 = 99u;

class Msm8255Sdcc2
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<
          kSdc2Base, kSdc2Size, kSdc2ResetClock, kSdc2SlotIndex,
          kSdc2IrqSource0, kSdc2IrqSource1> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc2);
