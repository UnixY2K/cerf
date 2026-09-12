#include "msm8255_smd_stage.h"

#include "msm8255_ram_partitions.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../cpu/emulated_memory.h"

#include <cstdint>

namespace {

constexpr uint32_t kStageAlign = 0x100000u;

}

REGISTER_SERVICE(Msm8255SmdStage);

bool Msm8255SmdStage::ShouldRegister() {
    auto* bd = emu_.TryGet<BoardContext>();
    return bd && bd->GetSoc() == SocFamily::MSM8255;
}

void Msm8255SmdStage::OnReady() {
    auto& parts = emu_.Get<Msm8255RamPartitions>();
    uint32_t top = 0u;
    for (uint32_t i = 0; i < parts.PartitionCount(); ++i) {
        const Msm8255RamPartition part = parts.Partition(i);
        const uint32_t end = part.start + part.size;
        if (end > top) top = end;
    }
    base_pa_ = (top + kStageAlign - 1u) & ~(kStageAlign - 1u);
    emu_.Get<EmulatedMemory>().AddRegion(base_pa_, kStageBytes,
                                         PAGE_READWRITE);
}

/* Linux arch/arm/mach-msm smd.c ch_read_buffer answers with the run from tail
   to the end of the fifo when tail is above head, and ch_read loops so a read
   that spans the end lands contiguous in the caller's buffer. */
uint32_t Msm8255SmdStage::Linearize(uint32_t fifo_pa, uint32_t half,
                                    uint32_t tail, uint32_t avail) {
    auto& mem = emu_.Get<EmulatedMemory>();

    const uint32_t staged = avail < kStageBytes ? avail : kStageBytes;
    const uint32_t run    = half - tail;
    const uint32_t first  = staged < run ? staged : run;

    mem.CopyOut(fifo_pa + tail, buf_, first);
    if (staged > first) {
        mem.CopyOut(fifo_pa, buf_ + first, staged - first);
    }
    mem.CopyIn(base_pa_, buf_, staged);
    return staged;
}
