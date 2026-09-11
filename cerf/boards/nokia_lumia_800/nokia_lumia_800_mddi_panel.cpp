#include "../../socs/msm8255/msm8255_mddi_client.h"

#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../board_context.h"

#include <cstdint>

namespace {

constexpr uint16_t kPanelWidth  = 480u;
constexpr uint16_t kPanelHeight = 864u;

constexpr uint16_t kMfrName    = 0u;
constexpr uint16_t kProductCode = 0u;

constexpr uint32_t kIdRegister = 0u;
constexpr uint32_t kIdValue    = 0x000100A0u;

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

    uint32_t ReadRegister(uint32_t address) override {
        if (address == kIdRegister) {
            return kIdValue;
        }
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver read client "
            "register %u, and this panel answers only register %u",
            address, kIdRegister);
    }

    void WriteRegister(uint32_t address, uint32_t value) override {
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver wrote 0x%08X to "
            "client register %u, and this panel models no register write",
            value, address);
    }
};

}

REGISTER_SERVICE_AS(NokiaLumia800MddiPanel, Msm8255MddiClient);
