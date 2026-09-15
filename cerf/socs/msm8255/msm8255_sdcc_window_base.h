#pragma once

#include "../../peripherals/peripheral_base.h"

#include "msm8255_clock_reset.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../guest_cpu_reset.h"

#include <atomic>
#include <cstdint>

namespace cerf_msm8255_sdcc_detail {

constexpr uint32_t kPower    = 0x000u;
constexpr uint32_t kClock    = 0x004u;
constexpr uint32_t kArgument = 0x008u;
constexpr uint32_t kCommand  = 0x00Cu;
constexpr uint32_t kDataCtrl = 0x02Cu;
constexpr uint32_t kStatus   = 0x034u;
constexpr uint32_t kClear    = 0x038u;
constexpr uint32_t kMask0    = 0x03Cu;
constexpr uint32_t kMask1    = 0x040u;

constexpr uint32_t kCmdIndex    = 0x0000003Fu;
constexpr uint32_t kCmdResponse = 1u << 6;
constexpr uint32_t kCmdEnable   = 1u << 10;

constexpr uint32_t kCmdModelled = kCmdIndex | kCmdResponse | kCmdEnable;

constexpr uint32_t kStatusCmdTimeout = 1u << 2;
constexpr uint32_t kStatusCmdSent    = 1u << 7;

constexpr uint32_t kStatusLatchable = kStatusCmdTimeout | kStatusCmdSent;

constexpr uint32_t kClearStaticMask =
    (1u << 0) | (1u << 1) | (1u << 2) | (1u << 3) | (1u << 4) | (1u << 5) |
    (1u << 6) | (1u << 7) | (1u << 8) | (1u << 9) | (1u << 10) | (1u << 22) |
    (1u << 23) | (1u << 24) | (1u << 25) | (1u << 26);

constexpr uint32_t kQuiescent = 0u;

constexpr uint32_t kPowerWritable = 0x00000041u;
constexpr uint32_t kClockWritable = 0x0000FF00u;
constexpr uint32_t kMaskWritable  = 0x1FFFFFFFu;

constexpr uint32_t kMaskStorable = kMaskWritable & ~kStatusLatchable;

constexpr uint32_t kUngroundedPowerOn = 0u;

template <uint32_t kBase, uint32_t kSize, uint32_t kResetClock>
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
        case kPower: return Load(power_);
        case kClock: return Load(clock_);
        case kMask0:  return Load(mask0_);
        case kMask1:  return Load(mask1_);
        case kStatus: return Load(status_);
        default:      break;
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
            if ((value & ~kMaskStorable) == 0u) {
                Store(mask0_, value);
                return;
            }
            break;
        case kMask1:
            if ((value & ~kMaskStorable) == 0u) {
                Store(mask1_, value);
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
                const uint32_t event = (value & kCmdResponse) != 0u
                                           ? kStatusCmdTimeout
                                           : kStatusCmdSent;
                Store(status_, Load(status_) | event);
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
    }

    void RestoreState(StateReader& r) override {
        RestoreField(r, power_, kPowerWritable, kPower);
        RestoreField(r, clock_, kClockWritable, kClock);
        RestoreField(r, mask0_, kMaskStorable, kMask0);
        RestoreField(r, mask1_, kMaskStorable, kMask1);
        RestoreField(r, argument_, 0xFFFFFFFFu, kArgument);
        RestoreField(r, status_, kStatusLatchable, kStatus);
    }

private:
    static uint32_t Load(const std::atomic<uint32_t>& reg) {
        return reg.load(std::memory_order_acquire);
    }

    static void Store(std::atomic<uint32_t>& reg, uint32_t value) {
        reg.store(value, std::memory_order_release);
    }

    void ResetState() {
        Store(power_, kUngroundedPowerOn);
        Store(clock_, kUngroundedPowerOn);
        Store(mask0_, kUngroundedPowerOn);
        Store(mask1_, kUngroundedPowerOn);
        Store(argument_, kUngroundedPowerOn);
        Store(status_, 0u);
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
};

}  // namespace cerf_msm8255_sdcc_detail
