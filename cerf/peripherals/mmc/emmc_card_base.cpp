#include "emmc_card_base.h"

#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../socs/guest_cpu_reset.h"
#include "../../state/state_stream.h"

using namespace cerf_mmc;

namespace {

constexpr uint32_t kCsdStructure   = 3u;
constexpr uint32_t kSpecVers       = 4u;
constexpr uint32_t kTaac           = 0x0Eu;
constexpr uint32_t kNsac           = 0x00u;
constexpr uint32_t kTranSpeed      = 0x32u;
constexpr uint32_t kCcc            = 0x0F5u;
constexpr uint32_t kReadBlLen      = 0x09u;
constexpr uint32_t kWriteBlLen     = 0x09u;
constexpr uint32_t kCSizeSaturated = 0xFFFu;
constexpr uint32_t kCSizeMult      = 7u;
constexpr uint32_t kEraseGrpSize   = 31u;
constexpr uint32_t kEraseGrpMult   = 31u;
constexpr uint32_t kR2wFactor      = 4u;

constexpr uint32_t kExtCsdBytes          = 512u;
constexpr uint32_t kExtCsdRev            = 192u;
constexpr uint32_t kExtCsdStructure      = 194u;
constexpr uint32_t kExtCsdCardType       = 196u;
constexpr uint32_t kExtCsdSecCount       = 212u;
constexpr uint8_t  kExtCsdRevValue       = 3u;
constexpr uint8_t  kExtCsdStructureValue = 2u;
constexpr uint8_t  kExtCsdCardTypeValue  = 1u;

void PutBits(uint32_t out[4], uint32_t start, uint32_t width, uint32_t value) {
    const uint32_t mask  = (width < 32u) ? ((1u << width) - 1u) : 0xFFFFFFFFu;
    const uint32_t off   = 3u - (start / 32u);
    const uint32_t shift = start & 31u;
    value &= mask;
    out[off] |= value << shift;
    if (width + shift > 32u) {
        out[off - 1u] |= value >> (32u - shift);
    }
}

// JEDEC JESD84-A43 section 10.2
uint8_t Crc7(const uint8_t* data, uint32_t length) {
    uint8_t crc = 0u;
    for (uint32_t i = 0; i < length; ++i) {
        uint8_t byte = data[i];
        for (uint32_t bit = 0; bit < 8u; ++bit) {
            const uint8_t in = static_cast<uint8_t>((byte >> 7) & 1u);
            const uint8_t out = static_cast<uint8_t>((crc >> 6) & 1u);
            crc = static_cast<uint8_t>((crc << 1) & 0x7Fu);
            if (in ^ out) crc ^= 0x09u;
            byte = static_cast<uint8_t>(byte << 1);
        }
    }
    return crc;
}

// JEDEC JESD84-A43 Table 32, Table 34
void SealCrc7(uint32_t out[4]) {
    uint8_t bytes[15];
    for (uint32_t i = 0; i < 15u; ++i) {
        bytes[i] = static_cast<uint8_t>(out[i / 4u] >> (8u * (3u - (i % 4u))));
    }
    out[3] = (out[3] & 0xFFFFFF00u) |
             static_cast<uint32_t>((Crc7(bytes, 15u) << 1) | 1u);
}

}  // namespace

void EmmcCardBase::OnReady() {
    emu_.Get<GuestCpuReset>().RegisterResetListener(
        [this](ResetLineKind) { Reset(); });
}

MmcCommandResult EmmcCardBase::Command(uint8_t index, uint32_t argument,
                                       uint32_t response[4]) {
    const MmcState before  = state_;
    const uint16_t arg_rca = static_cast<uint16_t>(argument >> 16);

    read_data_.clear();

    switch (index) {
    case kCmdGoIdleState:
        state_ = MmcState::Idle;
        rca_   = 0u;
        return MmcCommandResult::NoResponse;

    case kCmdIoRwDirect:
        return MmcCommandResult::NoResponse;

    case kCmdSleepAwake:
    case kCmdAppCmd:
        if (before != MmcState::Idle && before != MmcState::Ready &&
            before != MmcState::Ident) {
            break;
        }
        return MmcCommandResult::NoResponse;

    case kCmdSendOpCond:
        if (before != MmcState::Idle) break;
        state_      = MmcState::Ready;
        response[0] =
            kOcrBusy | kOcrSectorAddr | kOcrVoltage | kOcrVoltageDual;
        return MmcCommandResult::Short;

    case kCmdAllSendCid:
        if (before != MmcState::Ready) break;
        state_ = MmcState::Ident;
        BuildCid(response);
        return MmcCommandResult::Long;

    case kCmdSetRelativeAddr:
        if (before != MmcState::Ident) break;
        rca_        = arg_rca;
        state_      = MmcState::Stby;
        response[0] = StatusWord(before);
        return MmcCommandResult::Short;

    case kCmdSendCsd:
        if (before != MmcState::Stby) break;
        if (arg_rca != rca_) return MmcCommandResult::NoResponse;
        BuildCsd(response);
        return MmcCommandResult::Long;

    case kCmdSendCid:
        if (before != MmcState::Stby) break;
        if (arg_rca != rca_) return MmcCommandResult::NoResponse;
        BuildCid(response);
        return MmcCommandResult::Long;

    case kCmdSelectCard:
        if (before == MmcState::Stby && arg_rca == rca_ && rca_ != 0u) {
            state_      = MmcState::Tran;
            response[0] = StatusWord(before);
            return MmcCommandResult::Short;
        }
        if (before == MmcState::Tran && arg_rca != rca_) {
            state_ = MmcState::Stby;
            return MmcCommandResult::NoResponse;
        }
        break;

    case kCmdSendStatus:
        if (before != MmcState::Stby && before != MmcState::Tran) break;
        if (arg_rca != rca_) return MmcCommandResult::NoResponse;
        response[0] = StatusWord(before);
        return MmcCommandResult::Short;

    case kCmdSwitch:
        if (before != MmcState::Tran) break;
        ApplySwitch(argument);
        response[0] = StatusWord(before);
        return MmcCommandResult::Short;

    case kCmdSendExtCsd:
        if (before == MmcState::Idle) return MmcCommandResult::NoResponse;
        if (before != MmcState::Tran) break;
        BuildExtCsd();
        response[0] = StatusWord(before);
        return MmcCommandResult::Short;

    default:
        break;
    }

    HaltUnmodelledCommand(index, argument);
}

void EmmcCardBase::Reset() {
    state_     = MmcState::Idle;
    rca_       = 0u;
    hs_timing_ = 0u;
    read_data_.clear();
}

void EmmcCardBase::BuildExtCsd() {
    read_data_.assign(kExtCsdBytes, 0u);
    const uint32_t sectors = SectorCount();
    for (uint32_t i = 0; i < 4u; ++i) {
        read_data_[kExtCsdSecCount + i] =
            static_cast<uint8_t>(sectors >> (8u * i));
    }
    read_data_[kExtCsdRev]       = kExtCsdRevValue;
    read_data_[kExtCsdStructure] = kExtCsdStructureValue;
    read_data_[kExtCsdCardType]  = kExtCsdCardTypeValue;
    read_data_[kExtCsdHsTiming]  = hs_timing_;
}

uint32_t EmmcCardBase::StatusWord(MmcState before) const {
    return kR1ReadyForData | (static_cast<uint32_t>(before) << kR1StateShift);
}

void EmmcCardBase::BuildCid(uint32_t out[4]) const {
    const SdCardCid cid = Cid();
    for (uint32_t i = 0; i < 4u; ++i) {
        out[i] = (static_cast<uint32_t>(cid[i * 4u + 0u]) << 24) |
                 (static_cast<uint32_t>(cid[i * 4u + 1u]) << 16) |
                 (static_cast<uint32_t>(cid[i * 4u + 2u]) << 8) |
                  static_cast<uint32_t>(cid[i * 4u + 3u]);
    }
    SealCrc7(out);
}

void EmmcCardBase::BuildCsd(uint32_t out[4]) const {
    out[0] = out[1] = out[2] = out[3] = 0u;
    PutBits(out, 126u, 2u,  kCsdStructure);
    PutBits(out, 122u, 4u,  kSpecVers);
    PutBits(out, 112u, 8u,  kTaac);
    PutBits(out, 104u, 8u,  kNsac);
    PutBits(out,  96u, 8u,  kTranSpeed);
    PutBits(out,  84u, 12u, kCcc);
    PutBits(out,  80u, 4u,  kReadBlLen);
    PutBits(out,  62u, 12u, kCSizeSaturated);
    PutBits(out,  47u, 3u,  kCSizeMult);
    PutBits(out,  42u, 5u,  kEraseGrpSize);
    PutBits(out,  37u, 5u,  kEraseGrpMult);
    PutBits(out,  26u, 3u,  kR2wFactor);
    PutBits(out,  22u, 4u,  kWriteBlLen);
    SealCrc7(out);
}

void EmmcCardBase::ApplySwitch(uint32_t argument) {
    const uint32_t access =
        (argument >> kSwitchAccessShift) & kSwitchAccessMask;
    const uint32_t index = (argument >> kSwitchIndexShift) & kSwitchByteMask;
    const uint32_t value = (argument >> kSwitchValueShift) & kSwitchByteMask;

    if (access != kSwitchWriteByte) {
        emu_.Get<Fatal>().Die(
            "eMMC card in slot %u: SWITCH access mode %u is not modeled",
            SlotIndex(), access);
    }
    if (index == kExtCsdBusWidth) {
        if (value > kBusWidth8Bit) {
            emu_.Get<Fatal>().Die(
                "eMMC card in slot %u: SWITCH selects bus mode %u, which is "
                "reserved", SlotIndex(), value);
        }
        return;
    }
    if (index == kExtCsdHsTiming) {
        if (value > kHsTimingHighSpeed) {
            emu_.Get<Fatal>().Die(
                "eMMC card in slot %u: SWITCH selects interface timing %u, "
                "which is not a value this card accepts", SlotIndex(), value);
        }
        hs_timing_ = static_cast<uint8_t>(value);
        return;
    }
    emu_.Get<Fatal>().Die(
        "eMMC card in slot %u: SWITCH writes extended CSD byte %u, which is "
        "not modeled", SlotIndex(), index);
}

void EmmcCardBase::HaltUnmodelledCommand(uint8_t index, uint32_t argument) {
    emu_.Get<Fatal>().Die(
        "eMMC card in slot %u: CMD%u with argument 0x%08X in card state %u is "
        "not modeled", SlotIndex(), static_cast<unsigned>(index), argument,
        static_cast<unsigned>(state_));
}

void EmmcCardBase::SaveState(StateWriter& w) {
    w.Write<uint32_t>(static_cast<uint32_t>(state_));
    w.Write<uint32_t>(rca_);
    w.Write<uint32_t>(hs_timing_);
}

void EmmcCardBase::RestoreState(StateReader& r) {
    uint32_t state     = 0u;
    uint32_t rca       = 0u;
    uint32_t hs_timing = 0u;
    r.Read(state);
    r.Read(rca);
    r.Read(hs_timing);
    if (hs_timing > kHsTimingHighSpeed) {
        emu_.Get<Fatal>().Die(
            "eMMC card in slot %u: restored interface timing %u is not a value "
            "this card can hold", SlotIndex(), hs_timing);
    }
    hs_timing_ = static_cast<uint8_t>(hs_timing);
    if (state > static_cast<uint32_t>(MmcState::Tran) || rca > 0xFFFFu) {
        emu_.Get<Fatal>().Die(
            "eMMC card in slot %u: restored state %u rca 0x%X is not a state "
            "this card can reach", SlotIndex(), state, rca);
    }
    state_ = static_cast<MmcState>(state);
    rca_   = static_cast<uint16_t>(rca);
}
