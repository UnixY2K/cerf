#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc3Base = 0xA3000000u;
constexpr uint32_t kSdc3Size = 0x00000800u;

constexpr uint32_t kSdc3ResetClock = 132u;
constexpr uint32_t kSdc3SlotIndex  = 2u;
constexpr uint32_t kSdc3IrqSource0 = 96u;
constexpr uint32_t kSdc3IrqSource1 = 97u;
constexpr uint32_t kSdc3Crci       = 12u;

class Msm8255Sdcc3
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<
          kSdc3Base, kSdc3Size, kSdc3ResetClock, kSdc3SlotIndex,
          kSdc3IrqSource0, kSdc3IrqSource1, kSdc3Crci> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc3);
