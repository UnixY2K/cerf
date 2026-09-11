#include "../../socs/msm8255/msm8255_mddi_client.h"

#include "../../core/cerf_emulator.h"
#include "../board_context.h"

#include <cstdint>

namespace {

constexpr uint16_t kPanelWidth  = 480u;
constexpr uint16_t kPanelHeight = 864u;

constexpr uint16_t kMfrName    = 0u;
constexpr uint16_t kProductCode = 0u;

class NokiaLumia800MddiPanel : public Msm8255MddiClient {
public:
    using Msm8255MddiClient::Msm8255MddiClient;

    bool ShouldRegister() override {
        auto* bd = emu_.TryGet<BoardContext>();
        return bd && bd->GetBoard() == Board::NokiaLumia800;
    }

    Msm8255MddiClientCapability Capability() const override {
        return {kPanelWidth, kPanelHeight, kPanelWidth, kPanelHeight,
                kMfrName,    kProductCode};
    }
};

}

REGISTER_SERVICE_AS(NokiaLumia800MddiPanel, Msm8255MddiClient);
