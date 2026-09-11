#include "../../socs/msm8255/msm8255_mddi_client.h"

#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../state/state_stream.h"
#include "../board_context.h"

#include <cstdint>

namespace {

constexpr uint16_t kPanelWidth  = 480u;
constexpr uint16_t kPanelHeight = 864u;

constexpr uint16_t kMfrName    = 0u;
constexpr uint16_t kProductCode = 0u;

constexpr uint32_t kIdRegister = 0u;
constexpr uint32_t kIdValue    = 0x000100A0u;

constexpr uint32_t kRegTrigger    = 2336u;
constexpr uint32_t kRegPacket     = 2340u;
constexpr uint32_t kRegPayloadLo  = 2344u;
constexpr uint32_t kRegPayloadHi  = 2348u;
constexpr uint32_t kRegReadResult = 2548u;

constexpr uint32_t kDsiShortWrite0 = 0x05u;
constexpr uint32_t kDsiRead        = 0x06u;
constexpr uint32_t kDsiShortWrite1 = 0x15u;
constexpr uint32_t kDsiLongWrite   = 0x39u;

constexpr uint32_t kPacketTypeMask  = 0xFFu;
constexpr uint32_t kPacketCountShift = 16u;
constexpr uint32_t kByteMask        = 0xFFu;

constexpr uint32_t kMaxPayloadBytes = 8u;

constexpr uint32_t kTriggerShortPacket = 1u;
constexpr uint32_t kTriggerLongPacket  = 13u;

constexpr uint32_t kDcsExitSleepMode       = 0x11u;
constexpr uint32_t kDcsEnterPartialMode    = 0x12u;
constexpr uint32_t kDcsEnterNormalMode     = 0x13u;
constexpr uint32_t kDcsSetDisplayOn        = 0x29u;
constexpr uint32_t kDcsSetTearOn           = 0x35u;
constexpr uint32_t kDcsSetAddressMode      = 0x36u;
constexpr uint32_t kDcsSetPixelFormat      = 0x3Au;
constexpr uint32_t kDcsSetTearScanline     = 0x44u;
constexpr uint32_t kDcsWriteBrightness     = 0x51u;
constexpr uint32_t kDcsWriteControlDisplay = 0x53u;
constexpr uint32_t kDcsWritePowerSave      = 0x55u;
constexpr uint32_t kDcsReadId1             = 0xDAu;
constexpr uint32_t kDcsReadId3             = 0xDCu;

constexpr uint32_t kPixelFormat16Bpp = 0x05u;

constexpr uint32_t kId1Value = 0x0100FE21u;
constexpr uint32_t kId3Value = 0x27009621u;

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
        if (address == kRegReadResult && read_armed_) {
            read_armed_ = false;
            return read_result_;
        }
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver read client "
            "register %u, and this panel answers only register %u and an armed "
            "command result", address, kIdRegister);
    }

    void WriteRegister(uint32_t address, uint32_t value) override {
        switch (address) {
            case kRegPacket:    packet_     = value; return;
            case kRegPayloadLo: payload_lo_ = value; return;
            case kRegPayloadHi: payload_hi_ = value; return;
            case kRegTrigger:   ExecutePacket(value); return;
            default:            break;
        }
        if (IsConfiguredRegister(address)) {
            return;
        }
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver wrote 0x%08X to "
            "client register %u, and this panel models no such register",
            value, address);
    }

    void SaveState(StateWriter& w) override {
        w.Write<uint32_t>(packet_);
        w.Write<uint32_t>(payload_lo_);
        w.Write<uint32_t>(payload_hi_);
        w.Write<uint32_t>(read_result_);
        w.Write<uint32_t>(read_armed_ ? 1u : 0u);
        w.Write<uint32_t>(brightness_);
        w.Write<uint32_t>(display_on_ ? 1u : 0u);
        w.Write<uint32_t>(sleeping_ ? 1u : 0u);
    }

    void RestoreState(StateReader& r) override {
        uint32_t armed = 0;
        uint32_t on    = 0;
        uint32_t sleep = 0;
        r.Read(packet_);
        r.Read(payload_lo_);
        r.Read(payload_hi_);
        r.Read(read_result_);
        r.Read(armed);
        r.Read(brightness_);
        r.Read(on);
        r.Read(sleep);
        read_armed_ = armed != 0u;
        display_on_ = on != 0u;
        sleeping_   = sleep != 0u;
    }

private:
    uint32_t PayloadByte(uint32_t index) const {
        const uint32_t word = index < 4u ? payload_lo_ : payload_hi_;
        return (word >> (8u * (index & 3u))) & kByteMask;
    }

    uint32_t PayloadBytes(uint32_t type) {
        switch (type) {
            case kDsiShortWrite0: return 1u;
            case kDsiRead:        return 1u;
            case kDsiShortWrite1: return 2u;
            case kDsiLongWrite:   break;
            default:
                emu_.Get<Fatal>().Die(
                    "nokia lumia 800 mddi panel: the display driver triggered a "
                    "packet of data type 0x%02X, and this panel models only the "
                    "display command set", type);
        }
        const uint32_t declared = packet_ >> kPacketCountShift;
        if (declared == 0u || declared > kMaxPayloadBytes) {
            emu_.Get<Fatal>().Die(
                "nokia lumia 800 mddi panel: the display driver declared a "
                "%u-byte long packet, and this panel stages %u bytes",
                declared, kMaxPayloadBytes);
        }
        return declared;
    }

    void ExecutePacket(uint32_t trigger) {
        const uint32_t type  = packet_ & kPacketTypeMask;
        const uint32_t bytes = PayloadBytes(type);

        const uint32_t expected = type == kDsiLongWrite ? kTriggerLongPacket
                                                        : kTriggerShortPacket;
        if (trigger != expected) {
            emu_.Get<Fatal>().Die(
                "nokia lumia 800 mddi panel: the display driver triggered a "
                "data type 0x%02X packet with 0x%02X, and this panel models "
                "0x%02X for it", type, trigger, expected);
        }

        const uint32_t command = PayloadByte(0);
        if (type == kDsiRead) {
            ArmRead(command);
            return;
        }
        ExecuteDisplayCommand(command, bytes - 1u);
    }

    void RequireParameters(uint32_t command, uint32_t have, uint32_t want) {
        if (have == want) {
            return;
        }
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver issued command "
            "0x%02X with %u parameters, and this panel models it with %u",
            command, have, want);
    }

    void ExecuteDisplayCommand(uint32_t command, uint32_t parameters) {
        switch (command) {
            case kDcsExitSleepMode:
                RequireParameters(command, parameters, 0u);
                sleeping_ = false;
                return;
            case kDcsSetDisplayOn:
                RequireParameters(command, parameters, 0u);
                display_on_ = true;
                return;
            case kDcsEnterPartialMode:
            case kDcsEnterNormalMode:
                RequireParameters(command, parameters, 0u);
                return;
            case kDcsWriteBrightness:
                RequireParameters(command, parameters, 1u);
                brightness_ = PayloadByte(1u);
                return;
            case kDcsSetTearOn:
            case kDcsSetAddressMode:
            case kDcsWriteControlDisplay:
            case kDcsWritePowerSave:
                RequireParameters(command, parameters, 1u);
                return;
            case kDcsSetTearScanline:
                RequireParameters(command, parameters, 2u);
                return;
            case kDcsSetPixelFormat:
                RequireParameters(command, parameters, 1u);
                if (PayloadByte(1u) != kPixelFormat16Bpp) {
                    emu_.Get<Fatal>().Die(
                        "nokia lumia 800 mddi panel: the display driver set "
                        "pixel format 0x%02X, and this panel presents 16 bits "
                        "per pixel", PayloadByte(1u));
                }
                return;
            default:
                break;
        }
        emu_.Get<Fatal>().Die(
            "nokia lumia 800 mddi panel: the display driver issued display "
            "command 0x%02X with %u parameters, and this panel models no such "
            "command", command, parameters);
    }

    void ArmRead(uint32_t command) {
        switch (command) {
            case kDcsReadId1: read_result_ = kId1Value; break;
            case kDcsReadId3: read_result_ = kId3Value; break;
            default:
                emu_.Get<Fatal>().Die(
                    "nokia lumia 800 mddi panel: the display driver read "
                    "display command 0x%02X, and this panel answers only its "
                    "identification commands", command);
        }
        read_armed_ = true;
    }

    static bool IsConfiguredRegister(uint32_t address) {
        switch (address) {
            case    8u: case   32u: case   36u: case   40u: case   64u:
            case  272u: case  276u: case  292u: case  320u: case  324u:
            case  328u: case 2304u: case 2320u: case 2324u: case 2544u:
            case 6156u: case 6404u: case 7172u: case 7184u: case 7188u:
            case 7192u: case 7196u: case 7200u: case 7204u: case 7208u:
            case 7212u: case 7216u: case 7220u: case 7224u:
                return true;
            default:
                return false;
        }
    }

    uint32_t packet_      = 0;
    uint32_t payload_lo_  = 0;
    uint32_t payload_hi_  = 0;
    uint32_t read_result_ = 0;
    bool     read_armed_  = false;

    uint32_t brightness_ = 0;
    bool     display_on_ = false;
    bool     sleeping_   = true;
};

}

REGISTER_SERVICE_AS(NokiaLumia800MddiPanel, Msm8255MddiClient);
