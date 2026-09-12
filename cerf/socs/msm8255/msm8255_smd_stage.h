#pragma once

#include "../../core/service.h"

#include <cstdint>

class Msm8255SmdStage : public Service {
public:
    using Service::Service;

    bool ShouldRegister() override;
    void OnReady() override;

    uint32_t BasePa() const { return base_pa_; }

    uint32_t Linearize(uint32_t fifo_pa, uint32_t half, uint32_t tail,
                       uint32_t avail);

private:
    static constexpr uint32_t kStageBytes = 0x1000u;

    uint32_t base_pa_             = 0u;
    uint8_t  buf_[kStageBytes]    = {};
};
