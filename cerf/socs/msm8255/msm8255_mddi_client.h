#pragma once

#include "../../core/service.h"

#include <cstdint>

struct Msm8255MddiClientCapability {
    uint16_t bitmap_width;
    uint16_t bitmap_height;
    uint16_t display_window_width;
    uint16_t display_window_height;
    uint16_t mfr_name;
    uint16_t product_code;
};

class Msm8255MddiClient : public Service {
public:
    using Service::Service;

    virtual Msm8255MddiClientCapability Capability() const = 0;
};
