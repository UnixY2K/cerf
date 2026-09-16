#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc4Base = 0xA3100000u;
constexpr uint32_t kSdc4Size = 0x00000800u;

constexpr uint32_t kSdc4ResetClock = 133u;
constexpr uint32_t kSdc4SlotIndex  = 3u;
constexpr uint32_t kSdc4IrqSource0 = 100u;
constexpr uint32_t kSdc4IrqSource1 = 101u;

class Msm8255Sdcc4
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<
          kSdc4Base, kSdc4Size, kSdc4ResetClock, kSdc4SlotIndex,
          kSdc4IrqSource0, kSdc4IrqSource1> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc4);
