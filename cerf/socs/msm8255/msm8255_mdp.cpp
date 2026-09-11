#include "../../peripherals/peripheral_base.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../peripherals/peripheral_dispatcher.h"
#include "../../state/state_stream.h"
#include "../guest_cpu_reset.h"

#include <atomic>
#include <cstdint>

namespace {

constexpr uint32_t kMdpBase = 0xA3F00000u;
constexpr uint32_t kMdpSize = 0x00100000u;

constexpr uint32_t kWordCount = kMdpSize / 4u;

/* Linux arch/arm/mach-msm video-msm mdp.c mdp_probe reads the word at the MDP
   base as mdp_version and tests it against the packed 0x04030303, one byte per
   version field. */
constexpr uint32_t kRegVersion   = 0x00000u;
constexpr uint32_t kVersionValue = 0x04000000u;

/* Linux arch/arm/mach-msm video-msm mdp.h, the CONFIG_FB_MSM_MDP40 arm:
   MDP_EBI2_PORTMAP_MODE. */
constexpr uint32_t kRegEbi2PortmapMode = 0x00070u;

constexpr uint32_t kRegReset = 0u;

constexpr uint32_t kStateChunkWords = 1024u;
static_assert(kWordCount % kStateChunkWords == 0u,
              "the register file must divide into whole state chunks");

struct Span {
    uint32_t first;
    uint32_t last;
};

constexpr Span kWritableSpans[] = {
    {0x00028u, 0x00028u}, {0x00030u, 0x00030u}, {0x00048u, 0x00048u},
    {0x00050u, 0x00050u}, {0x00060u, 0x00060u}, {0x00068u, 0x00068u},
    {0x00070u, 0x00070u}, {0x00090u, 0x00090u}, {0x00094u, 0x00094u},
    {0x00098u, 0x00098u},
    {0x11004u, 0x11004u}, {0x21004u, 0x21004u}, {0x31004u, 0x31004u},
    {0x41004u, 0x41004u}, {0x51004u, 0x51004u}, {0x91004u, 0x91004u},
    {0x24400u, 0x24420u}, {0x24500u, 0x24508u}, {0x24580u, 0x24588u},
    {0x24600u, 0x24614u}, {0x24680u, 0x24694u},
    {0x28100u, 0x2810Cu}, {0x28200u, 0x28204u}, {0x29000u, 0x2AFFCu},
    {0x34400u, 0x34420u}, {0x34500u, 0x34508u}, {0x34580u, 0x34588u},
    {0x34600u, 0x34614u}, {0x34680u, 0x34694u},
    {0x38100u, 0x3810Cu}, {0x38200u, 0x38204u}, {0x39000u, 0x3AFFCu},
    {0x90000u, 0x90010u}, {0x90018u, 0x90020u}, {0x90040u, 0x9004Cu},
    {0x90060u, 0x90070u},
    {0x93400u, 0x93420u}, {0x93500u, 0x93508u}, {0x93580u, 0x93588u},
    {0x93600u, 0x93614u}, {0x93680u, 0x93694u},
    {0x94800u, 0x94FFCu},
    {0x95004u, 0x95008u}, {0x95010u, 0x95010u}, {0x95018u, 0x9501Cu},
    {0xC0000u, 0xC0038u}, {0xC2000u, 0xC2010u},
};

class Msm8255Mdp : public Peripheral {
public:
    using Peripheral::Peripheral;

    bool ShouldRegister() override {
        return emu_.Get<BoardContext>().GetSoc() == SocFamily::MSM8255;
    }

    void OnReady() override {
        ResetState();
        emu_.Get<GuestCpuReset>().RegisterResetListener(
            [this](ResetLineKind) { ResetState(); });
        emu_.Get<PeripheralDispatcher>().Register(this);
    }

    uint32_t MmioBase() const override { return kMdpBase; }
    uint32_t MmioSize() const override { return kMdpSize; }

    uint32_t ReadWord(uint32_t addr) override {
        const uint32_t off = addr - MmioBase();
        if (off == kRegVersion) {
            return kVersionValue;
        }
        if (off == kRegEbi2PortmapMode) {
            return Reg(off);
        }
        HaltUnsupportedAccess("ReadWord", addr, 0);
    }

    void WriteWord(uint32_t addr, uint32_t value) override {
        const uint32_t off = addr - MmioBase();
        if (!IsWritable(off)) {
            HaltUnsupportedAccess("WriteWord", addr, value);
        }
        SetReg(off, value);
    }

    void SaveState(StateWriter& w) override {
        uint32_t chunk[kStateChunkWords];
        for (uint32_t base = 0; base < kWordCount; base += kStateChunkWords) {
            for (uint32_t i = 0; i < kStateChunkWords; ++i) {
                chunk[i] = regs_[base + i].load(std::memory_order_acquire);
            }
            w.WriteBytes(chunk, sizeof(chunk));
        }
    }

    void RestoreState(StateReader& r) override {
        uint32_t chunk[kStateChunkWords];
        for (uint32_t base = 0; base < kWordCount; base += kStateChunkWords) {
            r.ReadBytes(chunk, sizeof(chunk));
            for (uint32_t i = 0; i < kStateChunkWords; ++i) {
                regs_[base + i].store(chunk[i], std::memory_order_release);
            }
        }
    }

private:
    uint32_t Reg(uint32_t off) const {
        return regs_[off / 4u].load(std::memory_order_acquire);
    }

    void SetReg(uint32_t off, uint32_t value) {
        regs_[off / 4u].store(value, std::memory_order_release);
    }

    void ResetState() {
        for (uint32_t i = 0; i < kWordCount; ++i) {
            regs_[i].store(kRegReset, std::memory_order_release);
        }
    }

    static bool IsWritable(uint32_t off) {
        for (const Span& s : kWritableSpans) {
            if (off >= s.first && off <= s.last) return true;
        }
        return false;
    }

    std::atomic<uint32_t> regs_[kWordCount] = {};
};

}

REGISTER_SERVICE(Msm8255Mdp);
