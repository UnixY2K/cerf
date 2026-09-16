#include "../../peripherals/mmc/emmc_card_base.h"

#include "../../core/cerf_emulator.h"
#include "../board_context.h"

namespace {

constexpr uint32_t kEmmcSlotIndex = 0u;

constexpr SdCardCid kCid = {
    0x00u,
    0x01u,
    0x00u,
    'C', 'E', 'R', 'F', 'M', 'C',
    0x10u,
    0x00u, 0x00u, 0x00u, 0x01u,
    0xBEu,
    0x01u,
};

class NokiaLumia800Emmc : public EmmcCardBase {
public:
    using EmmcCardBase::EmmcCardBase;

    bool ShouldRegister() override {
        auto* bd = emu_.TryGet<BoardContext>();
        return bd && bd->GetBoard() == Board::NokiaLumia800;
    }

    uint32_t SlotIndex() const override { return kEmmcSlotIndex; }

protected:
    SdCardCid Cid() const override { return kCid; }
};

}  // namespace

REGISTER_SERVICE_AS(NokiaLumia800Emmc, MmcCard);
