#pragma once

#include "../../peripherals/peripheral_base.h"

#include "msm8255_clock_reset.h"

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

namespace cerf_msm8255_sdcc_detail {

constexpr uint32_t kPower     = 0x000u;
constexpr uint32_t kClock     = 0x004u;
constexpr uint32_t kArgument  = 0x008u;
constexpr uint32_t kCommand   = 0x00Cu;
constexpr uint32_t kResponse0 = 0x014u;
constexpr uint32_t kResponse1 = 0x018u;
constexpr uint32_t kResponse2 = 0x01Cu;
constexpr uint32_t kResponse3 = 0x020u;
constexpr uint32_t kDataCtrl  = 0x02Cu;
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
constexpr uint32_t kStatusProgDone    = 1u << 23;

constexpr uint32_t kStatusLatchable =
    kStatusCmdTimeout | kStatusCmdRespEnd | kStatusCmdSent | kStatusProgDone;

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
          uint32_t kSlotIndex, uint32_t kIrqSource0, uint32_t kIrqSource1>
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
    }

    uint32_t MmioBase() const override { return kBase; }
    uint32_t MmioSize() const override { return kSize; }

    uint32_t ReadWord(uint32_t addr) override {
        switch (addr - kBase) {
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
        case kDataCtrl:
            if (value == kQuiescent) {
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
        if (auto* card = CardForSlot()) card->RestoreState(r);
    }

    void PostRestore() override {
        if (auto* card = CardForSlot()) card->PostRestore();
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
                return;
            }
            break;
        case MmcCommandResult::Long:
            if (wants_response && wants_long) {
                for (uint32_t i = 0; i < 4u; ++i) Store(response_[i], resp[i]);
                LatchStatus(done);
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
};

}  // namespace cerf_msm8255_sdcc_detail
