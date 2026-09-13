#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc1Base = 0xA0400000u;
constexpr uint32_t kSdc1Size = 0x00000800u;

class Msm8255Sdcc1
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<kSdc1Base,
                                                             kSdc1Size> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc1);
