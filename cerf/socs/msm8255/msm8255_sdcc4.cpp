#include "msm8255_sdcc_impl.h"

#include <cstdint>

namespace {

constexpr uint32_t kSdc4Base = 0xA3100000u;
constexpr uint32_t kSdc4Size = 0x00000800u;

class Msm8255Sdcc4
    : public cerf_msm8255_sdcc_detail::Msm8255SdccWindowBase<kSdc4Base,
                                                             kSdc4Size> {
public:
    using Msm8255SdccWindowBase::Msm8255SdccWindowBase;
};

}

REGISTER_SERVICE(Msm8255Sdcc4);
