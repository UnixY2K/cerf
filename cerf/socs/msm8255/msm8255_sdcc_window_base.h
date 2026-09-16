#pragma once

#include "../../peripherals/peripheral_base.h"

#include "msm8255_clock_reset.h"
#include "msm8255_crci_bus.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../irq_controller.h"
#include "../../peripherals/mmc/mmc_card.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../guest_cpu_reset.h"

#include <atomic>
#include <cstdint>
#include <vector>

namespace cerf_msm8255_sdcc_detail {

constexpr uint32_t kPower     = 0x000u;
constexpr uint32_t kClock     = 0x004u;
constexpr uint32_t kArgument  = 0x008u;
constexpr uint32_t kCommand   = 0x00Cu;
constexpr uint32_t kResponse0 = 0x014u;
constexpr uint32_t kResponse1 = 0x018u;
constexpr uint32_t kResponse2 = 0x01Cu;
constexpr uint32_t kResponse3 = 0x020u;
constexpr uint32_t kDataTimer  = 0x024u;
constexpr uint32_t kDataLength = 0x028u;
constexpr uint32_t kDataCtrl  = 0x02Cu;
constexpr uint32_t kFifo      = 0x080u;
constexpr uint32_t kFifoBytes = 16u * 4u;
constexpr uint32_t kStatus    = 0x034u;
constexpr uint32_t kClear     = 0x038u;
constexpr uint32_t kMask0     = 0x03Cu;
constexpr uint32_t kMask1     = 0x040u;

constexpr uint32_t kCmdIndex    = 0x0000003Fu;
constexpr uint32_t kCmdResponse = 1u << 6;
constexpr uint32_t kCmdLongRsp  = 1u << 7;
constexpr uint32_t kCmdEnable   = 1u << 10;
constexpr uint32_t kCmdProgEna  = 1u << 11;

constexpr uint32_t kCmdModelled =
    kCmdIndex | kCmdResponse | kCmdLongRsp | kCmdEnable | kCmdProgEna;

constexpr uint32_t kStatusCmdTimeout  = 1u << 2;
constexpr uint32_t kStatusCmdRespEnd  = 1u << 6;
constexpr uint32_t kStatusCmdSent     = 1u << 7;
constexpr uint32_t kStatusDataEnd     = 1u << 8;
constexpr uint32_t kStatusProgDone    = 1u << 23;

constexpr uint32_t kStatusLatchable =
    kStatusCmdTimeout | kStatusCmdRespEnd | kStatusCmdSent | kStatusProgDone |
    kStatusDataEnd;

constexpr uint32_t kDataCtrlEnable    = 1u << 0;
constexpr uint32_t kDataCtrlDirection = 1u << 1;
constexpr uint32_t kDataCtrlDmaEnable = 1u << 3;
constexpr uint32_t kDataCtrlBlockSize      = 0xFFF0u;
constexpr uint32_t kDataCtrlBlockSizeShift = 4u;

constexpr uint32_t kDataCtrlModelled =
    kDataCtrlEnable | kDataCtrlDirection | kDataCtrlDmaEnable |
    kDataCtrlBlockSize;

constexpr uint32_t kClearStaticMask =
    (1u << 0) | (1u << 1) | (1u << 2) | (1u << 3) | (1u << 4) | (1u << 5) |
    (1u << 6) | (1u << 7) | (1u << 8) | (1u << 9) | (1u << 10) | (1u << 22) |
    (1u << 23) | (1u << 24) | (1u << 25) | (1u << 26);

constexpr uint32_t kQuiescent = 0u;

constexpr uint32_t kPowerWritable = 0x00000041u;
constexpr uint32_t kClockWritable = 0x0000FF00u;
constexpr uint32_t kMaskWritable  = 0x1FFFFFFFu;

constexpr uint32_t kUngroundedPowerOn = 0u;

template <uint32_t kBase, uint32_t kSize, uint32_t kResetClock,
          uint32_t kSlotIndex, uint32_t kIrqSource0, uint32_t kIrqSource1,
          uint32_t kCrci>
class Msm8255SdccWindowBase : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        auto* bd = emu_.TryGet<BoardContext>();
        return bd && bd->GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        emu_.Get<GuestCpuReset>().RegisterResetListener(
            [this](ResetLineKind) { ResetState(); });
        emu_.Get<Msm8255ClockReset>().RegisterListener(
            kResetClock, [this] { ResetState(); });
        emu_.Get<PeripheralDispatcher>().Register(this);
        emu_.Get<Msm8255CrciBus>().DeclareFifo(kCrci, kBase + kFifo,
                                              kFifoBytes);
    }

    uint32_t MmioBase() const override { return kBase; }
    uint32_t MmioSize() const override { return kSize; }

    uint32_t ReadWord(uint32_t addr) override {
        const uint32_t off = addr - kBase;
        if (off >= kFifo && off < kFifo + kFifoBytes) return ReadFifo();
        switch (off) {
        case kPower:     return Load(power_);
        case kClock:     return Load(clock_);
        case kMask0:     return Load(mask0_);
        case kMask1:     return Load(mask1_);
        case kStatus:    return Load(status_);
        case kResponse0: return Load(response_[0]);
        case kResponse1: return Load(response_[1]);
        case kResponse2: return Load(response_[2]);
        case kResponse3: return Load(response_[3]);
        default:         break;
        }
        HaltUnsupportedAccess("ReadWord", addr, 0u);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        switch (addr - kBase) {
        case kPower:
            if ((value & ~kPowerWritable) == 0u) {
                Store(power_, value);
                return;
            }
            break;
        case kClock:
            if ((value & ~kClockWritable) == 0u) {
                Store(clock_, value);
                return;
            }
            break;
        case kMask0:
            if ((value & ~kMaskWritable) == 0u) {
                Store(mask0_, value);
                UpdateIrq();
                return;
            }
            break;
        case kMask1:
            if ((value & ~kMaskWritable) == 0u) {
                Store(mask1_, value);
                UpdateIrq();
                return;
            }
            break;
        case kArgument:
            Store(argument_, value);
            return;
        case kCommand:
            if (value == kQuiescent) {
                return;
            }
            if ((value & kCmdEnable) != 0u && (value & ~kCmdModelled) == 0u) {
                IssueCommand(value);
                return;
            }
            break;
        case kDataTimer:
            Store(data_timer_, value);
            return;
        case kDataLength:
            Store(data_length_, value);
            return;
        case kDataCtrl:
            if ((value & ~kDataCtrlModelled) == 0u) {
                StartDataPhase(value);
                return;
            }
            break;
        case kClear:
            if ((value & ~kClearStaticMask) == 0u) {
                Store(status_, Load(status_) & ~value);
                UpdateIrq();
                return;
            }
            break;
        default:
            break;
        }
        HaltUnsupportedAccess("WriteWord", addr, value);
    }

    void SaveState(StateWriter& w) override {
        w.Write<uint32_t>(Load(power_));
        w.Write<uint32_t>(Load(clock_));
        w.Write<uint32_t>(Load(mask0_));
        w.Write<uint32_t>(Load(mask1_));
        w.Write<uint32_t>(Load(argument_));
        w.Write<uint32_t>(Load(status_));
        for (auto& word : response_) w.Write<uint32_t>(Load(word));
        w.Write<uint32_t>(Load(data_timer_));
        w.Write<uint32_t>(Load(data_length_));
        w.Write<uint32_t>(Load(data_ctrl_));
        w.Write<uint32_t>(read_pos_);
        w.Write<uint32_t>(static_cast<uint32_t>(read_data_.size()));
        for (uint8_t b : read_data_) w.Write<uint8_t>(b);
        if (auto* card = CardForSlot()) card->SaveState(w);
    }

    void RestoreState(StateReader& r) override {
        RestoreField(r, power_, kPowerWritable, kPower);
        RestoreField(r, clock_, kClockWritable, kClock);
        RestoreField(r, mask0_, kMaskWritable, kMask0);
        RestoreField(r, mask1_, kMaskWritable, kMask1);
        RestoreField(r, argument_, 0xFFFFFFFFu, kArgument);
        RestoreField(r, status_, kStatusLatchable, kStatus);
        for (uint32_t i = 0; i < 4u; ++i) {
            RestoreField(r, response_[i], 0xFFFFFFFFu, kResponse0 + i * 4u);
        }
        RestoreField(r, data_timer_, 0xFFFFFFFFu, kDataTimer);
        RestoreField(r, data_length_, 0xFFFFFFFFu, kDataLength);
        RestoreField(r, data_ctrl_, kDataCtrlModelled, kDataCtrl);
        uint32_t pos = 0u;
        uint32_t staged = 0u;
        r.Read(pos);
        r.Read(staged);
        read_data_.resize(staged);
        for (uint32_t i = 0; i < staged; ++i) r.Read(read_data_[i]);
        if (pos > staged || (pos & 3u) != 0u) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: restored fifo cursor %u is not a word "
                "offset inside the %u staged bytes", kBase, pos, staged);
        }
        read_pos_ = pos;
        if (auto* card = CardForSlot()) card->RestoreState(r);
    }

    void PostRestore() override {
        if (auto* card = CardForSlot()) card->PostRestore();
        DriveCrci();
        UpdateIrq();
    }

private:
    static uint32_t Load(const std::atomic<uint32_t>& reg) {
        return reg.load(std::memory_order_acquire);
    }

    static void Store(std::atomic<uint32_t>& reg, uint32_t value) {
        reg.store(value, std::memory_order_release);
    }

    MmcCard* CardForSlot() {
        auto* card = emu_.TryGet<MmcCard>();
        return (card != nullptr && card->SlotIndex() == kSlotIndex) ? card
                                                                    : nullptr;
    }

    void LatchStatus(uint32_t event) {
        Store(status_, Load(status_) | event);
        UpdateIrq();
    }

    void UpdateIrq() {
        const uint32_t status = Load(status_);
        auto& vic = emu_.Get<IrqController>();
        DriveLine(vic, kIrqSource0, (status & Load(mask0_)) != 0u);
        DriveLine(vic, kIrqSource1, (status & Load(mask1_)) != 0u);
    }

    static void DriveLine(IrqController& vic, uint32_t source, bool high) {
        if (high) {
            vic.AssertIrq(source);
        } else {
            vic.DeAssertIrq(source);
        }
    }


    void IssueCommand(uint32_t value) {
        const bool wants_response = (value & kCmdResponse) != 0u;
        const bool wants_long     = (value & kCmdLongRsp) != 0u;
        const bool wants_progena  = (value & kCmdProgEna) != 0u;
        const uint32_t done = wants_progena
                                  ? (kStatusCmdRespEnd | kStatusProgDone)
                                  : kStatusCmdRespEnd;

        MmcCard* card = CardForSlot();
        if (card == nullptr) {
            if (wants_progena) HaltProgEnaWithoutResponse(value);
            LatchStatus(wants_response ? kStatusCmdTimeout : kStatusCmdSent);
            return;
        }

        uint32_t resp[4] = {0u, 0u, 0u, 0u};
        const MmcCommandResult result = card->Command(
            static_cast<uint8_t>(value & kCmdIndex), Load(argument_), resp);

        switch (result) {
        case MmcCommandResult::NoResponse:
            if (wants_progena) HaltProgEnaWithoutResponse(value);
            LatchStatus(wants_response ? kStatusCmdTimeout : kStatusCmdSent);
            return;
        case MmcCommandResult::Short:
            if (wants_response && !wants_long) {
                Store(response_[0], resp[0]);
                LatchStatus(done);
                BindDataPhase(*card);
                return;
            }
            break;
        case MmcCommandResult::Long:
            if (wants_response && wants_long) {
                for (uint32_t i = 0; i < 4u; ++i) Store(response_[i], resp[i]);
                LatchStatus(done);
                BindDataPhase(*card);
                return;
            }
            break;
        }

        emu_.Get<Fatal>().Die(
            "Peripheral at 0x%08X: CMD%u answered with response class %u while "
            "the command word 0x%08X asked for a different one",
            kBase, static_cast<unsigned>(value & kCmdIndex),
            static_cast<unsigned>(result), value);
    }

    void StartDataPhase(uint32_t value) {
        Store(data_ctrl_, value);
        if ((value & kDataCtrlEnable) == 0u) {
            read_data_.clear();
            read_pos_ = 0u;
            DriveCrci();
            return;
        }
        if ((value & kDataCtrlDirection) == 0u) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: data control 0x%08X starts a "
                "host-to-card data phase, which is not modeled", kBase, value);
        }
        if (CardForSlot() == nullptr) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: data control 0x%08X starts a data phase "
                "with no card in the slot", kBase, value);
        }
        const uint32_t block =
            (value & kDataCtrlBlockSize) >> kDataCtrlBlockSizeShift;
        const uint32_t want = Load(data_length_);
        if (block != want) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: data control 0x%08X carries a %u byte "
                "block over a %u byte transfer, and the per-block boundary this "
                "controller reports is not modeled", kBase, value, block, want);
        }
        read_data_.clear();
        read_pos_ = 0u;
        DriveCrci();
    }

    void BindDataPhase(MmcCard& card) {
        const std::vector<uint8_t>& staged = card.ReadData();
        if (staged.empty()) return;
        if ((Load(data_ctrl_) & kDataCtrlEnable) == 0u) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: the card answered with %u bytes while no "
                "data phase is armed",
                kBase, static_cast<unsigned>(staged.size()));
        }
        const uint32_t want = Load(data_length_);
        if (staged.size() != want) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: data length %u does not match the %u "
                "bytes the card answered with",
                kBase, want, static_cast<unsigned>(staged.size()));
        }
        read_data_ = staged;
        read_pos_  = 0u;
        DriveCrci();
    }

    void DriveCrci() {
        auto& lines = emu_.Get<Msm8255CrciBus>();
        if (read_pos_ < read_data_.size() &&
            (Load(data_ctrl_) & kDataCtrlDmaEnable) != 0u) {
            lines.Assert(kCrci);
        } else {
            lines.Deassert(kCrci);
        }
    }

    uint32_t ReadFifo() {
        if (read_pos_ + 4u > read_data_.size()) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: fifo read at byte %u passes the %u bytes "
                "of the data phase in progress",
                kBase, read_pos_, static_cast<unsigned>(read_data_.size()));
        }
        uint32_t v = 0u;
        for (uint32_t i = 0; i < 4u; ++i) {
            v |= static_cast<uint32_t>(read_data_[read_pos_ + i]) << (8u * i);
        }
        read_pos_ += 4u;
        if (read_pos_ == read_data_.size()) {
            DriveCrci();
            LatchStatus(kStatusDataEnd);
        }
        return v;
    }

    [[noreturn]] void HaltProgEnaWithoutResponse(uint32_t value) {
        emu_.Get<Fatal>().Die(
            "Peripheral at 0x%08X: CMD%u asks for programming-done but no card "
            "answers it (command word 0x%08X)",
            kBase, static_cast<unsigned>(value & kCmdIndex), value);
    }

    void ResetState() {
        Store(power_, kUngroundedPowerOn);
        Store(clock_, kUngroundedPowerOn);
        Store(mask0_, kUngroundedPowerOn);
        Store(mask1_, kUngroundedPowerOn);
        Store(argument_, kUngroundedPowerOn);
        Store(status_, 0u);
        for (auto& word : response_) Store(word, 0u);
        Store(data_timer_, kUngroundedPowerOn);
        Store(data_length_, kUngroundedPowerOn);
        Store(data_ctrl_, kUngroundedPowerOn);
        read_data_.clear();
        read_pos_ = 0u;
        DriveCrci();
        UpdateIrq();
    }

    void RestoreField(StateReader& r, std::atomic<uint32_t>& reg,
                      uint32_t writable, uint32_t offset) {
        uint32_t value = kUngroundedPowerOn;
        r.Read(value);
        if ((value & ~writable) != 0u) {
            emu_.Get<Fatal>().Die(
                "Peripheral at 0x%08X: restored +0x%03X value 0x%08X carries "
                "bits the guest never writes", kBase, offset, value);
        }
        Store(reg, value);
    }

    std::atomic<uint32_t> power_{kUngroundedPowerOn};
    std::atomic<uint32_t> clock_{kUngroundedPowerOn};
    std::atomic<uint32_t> mask0_{kUngroundedPowerOn};
    std::atomic<uint32_t> mask1_{kUngroundedPowerOn};
    std::atomic<uint32_t> argument_{kUngroundedPowerOn};
    std::atomic<uint32_t> status_{0};
    std::atomic<uint32_t> response_[4]{};
    std::atomic<uint32_t> data_timer_{kUngroundedPowerOn};
    std::atomic<uint32_t> data_length_{kUngroundedPowerOn};
    std::atomic<uint32_t> data_ctrl_{kUngroundedPowerOn};
    std::vector<uint8_t>  read_data_;
    uint32_t              read_pos_ = 0u;
};

}  // namespace cerf_msm8255_sdcc_detail
