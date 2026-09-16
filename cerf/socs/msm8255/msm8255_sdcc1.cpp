#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc1Base = 0xA0400000u;
constexpr uint32_t kSdc1Size = 0x00000800u;

constexpr uint32_t kSdc1ResetClock = 130u;
constexpr uint32_t kSdc1SlotIndex  = 0u;
constexpr uint32_t kSdc1IrqSource0 = 94u;
constexpr uint32_t kSdc1IrqSource1 = 95u;
constexpr uint32_t kSdc1Crci       = 6u;

class Msm8255Sdcc1
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<
          kSdc1Base, kSdc1Size, kSdc1ResetClock, kSdc1SlotIndex,
          kSdc1IrqSource0, kSdc1IrqSource1, kSdc1Crci> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc1);
