#pragma once

#include "../peripherals/peripheral_base.h"

#include "guest_cpu_reset.h"

#include "../core/cerf_emulator.h"
#include "../core/tick_scale.h"
#include "../jit/guest_cycle_clock.h"
#include "../peripherals/peripheral_dispatcher.h"
#include "../state/state_stream.h"

#include <cstdint>
#include <numeric>

#if CERF_DEV_MODE
#include "../core/rate_probe.h"
#endif

template <uint32_t kOscrHz>
class IntelOsTimerBase : public Peripheral {
public:
    using Peripheral::Peripheral;

    void OnReady() override {
        clock_ = &emu_.Get<GuestCycleClock>();
        const uint64_t g = std::gcd(clock_->CpuHz(), static_cast<uint64_t>(kOscrHz));
        cyc_unit_ = clock_->CpuHz() / g;
        tk_unit_  = kOscrHz / g;
        for (int n = 0; n < 4; ++n) {
            event_[n] = clock_->Add([this, n] { OnMatch(n); });
        }
        SetAnchor(clock_->Cycles(), 0u);
        ArmAll();
        emu_.Get<GuestCpuReset>().RegisterResetListener([this](ResetLineKind) {
            OnResetLine();
        });
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioSize() const override { return 0x00001000u; }

    FastReadFn  FastReader() override { return &IntelOsTimerBase::FastReadThunk; }
    FastWriteFn FastWriter() override { return &IntelOsTimerBase::FastWriteThunk; }

    uint32_t ReadWord(uint32_t addr) override {
        const uint32_t off = addr - MmioBase();
        if (!IsKnown(off)) HaltUnsupportedAccess("ReadWord", addr, 0);
        return ReadReg(off);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t off = addr - MmioBase();
        if (!IsKnown(off)) HaltUnsupportedAccess("WriteWord", addr, value);
        WriteReg(off, value);
    }

    void SaveState(StateWriter& w) override {
        for (int n = 0; n < 4; ++n) w.Write<uint32_t>(osmr_[n]);
        w.Write<uint32_t>(ossr_);
        w.Write<uint32_t>(ower_);
        w.Write<uint32_t>(oier_);
        w.Write<uint32_t>(Oscr(clock_->Cycles()));
    }

    void RestoreState(StateReader& r) override {
        for (int n = 0; n < 4; ++n) r.Read(osmr_[n]);
        r.Read(ossr_);
        r.Read(ower_);
        r.Read(oier_);
        uint32_t oscr = 0;
        r.Read(oscr);
        SetAnchor(clock_->Cycles(), oscr);
        ArmAll();
    }

    void PostRestore() override { PushMatchLevel(); }

protected:
    /* SA-1110 §9.4.2: the OSSR status bits are routed to the interrupt
       controller. §9.4.5: OIER gates only the SET of an OSSR bit - clearing an
       enable bit does not clear a set status bit, so OIER is not in the level. */
    virtual void SetMatchLevel(uint32_t level4) = 0;

    virtual void OnResetLine() {
        ower_ = 0;
        oier_ = 0;
    }

    void ResetRegistersToZero() {
        for (int n = 0; n < 4; ++n) osmr_[n] = 0u;
        ossr_ = 0u;
        oier_ = 0u;
        SetAnchor(clock_->Cycles(), 0u);
        ArmAll();
        PushMatchLevel();
    }

    uint32_t FastRead(uint32_t off, uint32_t width) {
        if (width != 4 || !IsKnown(off)) {
            HaltUnsupportedAccess("FastRead", MmioBase() + off, 0);
        }
        return ReadReg(off);
    }

private:
    static bool IsKnown(uint32_t off) {
        return off == 0x00 || off == 0x04 || off == 0x08 || off == 0x0C ||
               off == 0x10 || off == 0x14 || off == 0x18 || off == 0x1C;
    }

    static uint32_t FastReadThunk(void* ctx, uint32_t off, uint32_t width) {
        return static_cast<IntelOsTimerBase*>(ctx)->FastRead(off, width);
    }
    static void FastWriteThunk(void* ctx, uint32_t off, uint32_t value, uint32_t width) {
        static_cast<IntelOsTimerBase*>(ctx)->FastWrite(off, value, width);
    }

    void SetAnchor(uint64_t cycles, uint32_t oscr) {
        anchor_cycles_ = cycles;
        anchor_oscr_   = oscr;
    }

    uint64_t TicksSince(uint64_t cycles) const {
        return ScaleU64(cycles - anchor_cycles_, tk_unit_, cyc_unit_);
    }

    uint64_t CyclesForTicks(uint64_t ticks) const {
        return (ticks / tk_unit_) * cyc_unit_ +
               ((ticks % tk_unit_) * cyc_unit_ + tk_unit_ - 1u) / tk_unit_;
    }

    /* SA-1110 §9.4.1: the OSCR increments on rising edges of the 3.6864-MHz
       clock. */
    uint32_t Oscr(uint64_t cycles) const {
        return anchor_oscr_ + static_cast<uint32_t>(TicksSince(cycles));
    }

    /* SA-1110 §9.4.2: each OSMR is compared against the OSCR following every
       rising edge of the 3.6864-MHz clock. */
    void ArmChannel(int n) {
        const uint64_t now   = clock_->Cycles();
        const uint64_t since = TicksSince(now);
        const uint32_t d     = osmr_[n] - (anchor_oscr_ + static_cast<uint32_t>(since));
        const uint64_t ahead = d != 0u ? d : 0x100000000ull;
        clock_->Arm(event_[n], anchor_cycles_ + CyclesForTicks(since + ahead));
    }

    void ArmAll() {
        for (int n = 0; n < 4; ++n) ArmChannel(n);
    }

    void PushMatchLevel() { SetMatchLevel(ossr_ & 0xFu); }

    void OnMatch(int n) {
        const uint32_t bit = 1u << n;
        /* SA-1110 §9.4.5: the OIER enables decide whether a match will set a
           status bit in the OSSR - for every match register, with no WME term. */
        if ((oier_ & bit) != 0u) {
            ossr_ |= bit;
            PushMatchLevel();
#if CERF_DEV_MODE
            emu_.Get<RateProbe>().Inc(RateProbe::Counter::OstFires);
#endif
        }
        ArmChannel(n);
        /* SA-1110 §9.4.3 OWER bit 0 (WME): 0 - OSMR3 matches cause an interrupt
           request; 1 - OSMR3 matches cause a reset of the SA-1110. §9.4.6 and
           PXA255 §4.4.1 enable that reset on OWER[0], with no OIER term. */
        if (n == 3 && (ower_ & 0x1u) != 0u) {
            emu_.Get<GuestCpuReset>().WatchdogReset();
        }
    }

    uint32_t ReadReg(uint32_t off) {
        switch (off) {
            case 0x00: case 0x04: case 0x08: case 0x0C:
                return osmr_[off >> 2];
            case 0x10:
#if CERF_DEV_MODE
                emu_.Get<RateProbe>().Inc(RateProbe::Counter::OstReadOscr);
#endif
                return Oscr(clock_->Cycles());
            case 0x14: return ossr_ & 0xFu;
            case 0x18: return ower_ & 0x1u;
            case 0x1C: return oier_ & 0xFu;
        }
        HaltUnsupportedAccess("ReadReg", MmioBase() + off, 0);
    }

    void WriteReg(uint32_t off, uint32_t value) {
        switch (off) {
            case 0x00: case 0x04: case 0x08: case 0x0C: {
                const int n = static_cast<int>(off >> 2);
                osmr_[n] = value;
                ArmChannel(n);
                return;
            }
            case 0x10:
                SetAnchor(clock_->Cycles(), value);
                ArmAll();
                return;
            /* SA-1110 §9.4.4: an OSSR bit is cleared by writing a one to it;
               writing zeros has no effect. */
            case 0x14:
                ossr_ &= ~(value & 0xFu);
                PushMatchLevel();
                return;
            /* SA-1110 §9.4.3: WME is a write-once bit that can only be changed
               by a hardware, software or sleep-mode reset. */
            case 0x18:
                ower_ |= (value & 0x1u);
                return;
            case 0x1C:
                oier_ = value & 0xFu;
                return;
        }
        HaltUnsupportedAccess("WriteReg", MmioBase() + off, value);
    }

    void FastWrite(uint32_t off, uint32_t value, uint32_t width) {
        if (width != 4 || !IsKnown(off)) {
            HaltUnsupportedAccess("FastWrite", MmioBase() + off, value);
        }
        WriteReg(off, value);
    }

    GuestCycleClock*        clock_    = nullptr;
    GuestCycleClock::Event* event_[4] = {};

    uint64_t cyc_unit_      = 1;
    uint64_t tk_unit_       = 1;
    uint64_t anchor_cycles_ = 0;
    uint32_t anchor_oscr_   = 0;

    uint32_t osmr_[4] = {};
    uint32_t ossr_    = 0;
    uint32_t ower_    = 0;
    uint32_t oier_    = 0;
};
