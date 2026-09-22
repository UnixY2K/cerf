#include "../../peripherals/mmc/emmc_card_base.h"

#include "../../boot/rom_parser_service.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../board_context.h"
#include "nokia_lumia_800_id.h"

#include <cstring>

namespace {

constexpr uint32_t kEmmcSlotIndex  = 0u;
constexpr uint32_t kSynthesisedSectorCount = 0x01D5C000u;

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
        return bd && bd->GetBoardId() == BoardId::NokiaLumia800;
    }

    uint32_t SlotIndex() const override { return kEmmcSlotIndex; }

protected:
    SdCardCid Cid() const override { return kCid; }

    uint32_t SectorCount() const override { return kSynthesisedSectorCount; }

    void ReadBlock(uint32_t sector, uint8_t* out) override {
        const ParsedRom& rom = emu_.Get<RomParserService>().Primary();
        if (!rom.is_wmstore) {
            emu_.Get<Fatal>().Die(
                "eMMC card in slot %u: the loaded firmware package is not a "
                "_wmstore container, so no sector content is modeled",
                kEmmcSlotIndex);
        }
        const uint64_t rel = uint64_t(sector) * cerf_mmc::kBlockBytes;
        if (rel + cerf_mmc::kBlockBytes > rom.wmstore_payload_bytes) {
            emu_.Get<Fatal>().Die(
                "eMMC card in slot %u: sector %u lies past the %zu payload bytes "
                "the firmware package carries, and the content of sectors "
                "outside the package is not modeled",
                kEmmcSlotIndex, sector, rom.wmstore_payload_bytes);
        }
        std::memcpy(out, rom.raw.data() + rom.wmstore_payload_off + size_t(rel),
                    cerf_mmc::kBlockBytes);
    }
};

}  // namespace

REGISTER_SERVICE_AS(NokiaLumia800Emmc, MmcCard);
