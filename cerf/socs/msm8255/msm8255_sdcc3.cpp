#include "msm8255_sdcc_window_base.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc3Base = 0xA3000000u;
constexpr uint32_t kSdc3Size = 0x00000800u;

class Msm8255Sdcc3
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<kSdc3Base,
                                                             kSdc3Size> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc3);
