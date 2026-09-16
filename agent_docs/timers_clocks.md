# Timers and clocks

Every clock a guest can read derives from one time base. That base is the
number of guest instruction cycles the JIT has executed, at the datasheet CPU
rate of the SoC.

## Guest time

- **Guest time is a cycle count.** The JIT counts the cycles of every
  translated instruction. That count, at the CPU rate, is guest time.
- **The CPU rate is the datasheet product, never a rounded print.** A clock
  table gives a multiplier of the crystal. The rate is the crystal times the
  multiplier. SA-1110 Dev Man § 8.2 Table 8-1 gives CCF 01010 as 3,686,400 x 56
  = 206,438,400 Hz, printed "206.4". A PLL-programmed SoC takes the rate the
  guest programs, from the register's own formula.
- **A timer match is an event at a cycle count.** The JIT thread delivers the
  match when the cycle count reaches that event. A guest tick never comes
  from a host thread, and never from a host deadline.
- **The emulator runs at or behind wall, never ahead.** When the guest is
  behind wall by more than a bounded window, the emulator forgives the excess
  and never replays it. That window is the max-fallback of the Dolphin
  `CoreTiming` model. A slow host therefore slows the whole guest uniformly.
  A host-referenced observer sees the replay of at most one window when a
  busy guest becomes idle, and the guest cannot see it.

## Idle

- **A wait-for-interrupt completes only on an interrupt.** ARM DDI 0406C
  B1.8.14: a wake-up event is a physical IRQ regardless of CPSR.I. The idle
  wait advances guest time to the next armed event, paced to wall, and never
  ahead of wall. There is no timeout. A timeout is a spurious wake, and the
  guest's idle arithmetic credits it as slept time.
- **Idle exit on SA-11xx and PXA2xx follows ICCR.DIM** (SA-1110 § 9.5.2.2,
  PXA255 printed 4-21, PXA27x § 25.5.6). With DIM clear, any enabled
  interrupt ends idle, masked or not. With DIM set, only an unmasked one
  ends it. The wake completes the wait, and execution continues at the next
  instruction.
  An exception follows only through the ordinary interrupt gate.
- **Every idle shape reaches the same wait.** The SA-1110 sequence is
  `c15,c2,2`, an uncached load, then `c15,c8,2` (§ 9.5.2.1). The `c2,2`
  write only disables clock switching (§ 8.2.1). The wait is `c8,2` alone.
  XScale idles through a PWRMODE write. ARM920T idles through cp15
  `c7,c0,4`. The S3C2410 idles through a clock-control bit. An idle that
  does not reach the wait spins, and the guest's idle credit arithmetic then
  runs on nothing.

## Why the wall clock was abandoned

A timer that expired on a host clock set its status bit late by a host
thread's latency. Every CE kernel then needed its own rule for what its
handler did with the late tick. A cycle-count match cannot be late relative
to the guest's own instructions, so the mechanism needs nothing from any
kernel.

**A timer left on a host clock while the CPU is paced by cycles loses
ticks.** Its expiries continue while the guest sleeps in the throttle or
runs a host-side device operation. The next expiry lands on an
interrupt-controller pending bit that is already set, and it is gone. The
guest's clock then falls behind wall.

## What a timer is in Windows CE

The OAL drives one hardware timer channel as the system tick, and its ISR
credits the millisecond counter that `GetTickCount` reads. Everything else
about that channel is per-SoC and per-kernel. The guest's arithmetic around
that timer is what makes it dangerous.

- **The catch-up loop.** Many SA-11xx and PXA kernels re-arm with
  `OSMR0 += period`. The kernel repeats that add while `OSMR0 - OSCR` is
  under a small margin. The loop therefore walks one period per pass until
  the match is ahead of the counter again. **Whether the millisecond counter
  is credited inside that loop or once after it is a per-kernel choice, and
  it decides what lateness costs.**
  A kernel that credits inside the loop credits every period the loop walks.
  A kernel that credits once after it credits one period however many the
  loop walked. The subtraction is unsigned. An entry that is late by about
  one period lands in a narrow residue band whose values are less than the
  exit margin. In that band the loop walks 2^32 / period passes. At
  3.6864 MHz that is 1,165,211 passes, or 19.4 minutes of guest time inside
  one interrupt. That is the tick death. A store that lands at or behind the
  counter laps 2^32 ticks (1165 s at 3.6864 MHz). The channel is then dead
  until the guest rewrites it. On the cycle clock an entry is never late, so
  the loop runs one pass.
- **Per-pass and per-entry credit.** A kernel credits either once per loop
  pass or once per entry, in units of its own tick period, and some re-check
  the distance after the re-arm and credit again.
- **OEMIdle banking.** A banking kernel reads the phase since the last tick
  at idle entry. It adds an accumulator, credits the whole periods, and arms
  one long match at the scheduler deadline. On wake it credits the slept
  periods from the changed counter, or from the counter-derived quotient
  when the tick never came. Then it re-bases the match one period out. Every
  such re-base to the counter discards the phase between the last match and
  that counter. A kernel that advances the match by whole periods instead
  keeps that phase.
- **The bank-and-exit double.** An entry banks a full period at the
  scheduler's 1 ms deadline and exits without arming. The still-armed tick
  then credits the same period again.
- **A guest clock that does not keep time is a CERF defect until the
  arithmetic says otherwise.** The device kept time on silicon. An attribution
  to the guest's own tick arithmetic holds only when that arithmetic is
  computed from the reload count, the prescaler and the programmed clock. The
  result must also land on the measured ratio. A named mechanism that does not
  produce the measured magnitude is an open defect, not an explanation. A fix
  that needs a kernel's handler is the wrong fix.
- **The same shapes on other silicon.** The i.MX EPIT is a compare-register
  down-counter. Its tick handler re-reads the counter, clamps, and stores the
  compare a few instructions later. A host stretch of that window past the
  margin lands the compare behind the counter, and the channel laps. That is
  the SA-11xx death on a different IP. The S3C2410 PWM timer is an
  auto-reload down-counter with one interrupt per reload and no loop. It
  loses ticks instead of dying. MIPS CP0 Count / Compare is the OSCR / OSMR
  shape in a coprocessor register.

The time path is never shaped by what a kernel does. It carries no per-ROM
and no per-kernel branch.

## How a timer change is judged

- The guest's own tick counter against guest time is the criterion, measured
  under a starved host and then an idle host, with the guest under load. Any
  ratio other than one is a defect until the guest's own tick arithmetic is
  computed and lands on it.
- A host-referenced measurement reads the throttle's replay, so it is never
  the verdict. The guest's counter is.
- Nothing fast-forwards. No recovery loop iterates. No channel laps. Nothing
  fatals. Hibernation round trip and deep-sleep resume keep working on the
  same binary.

## Hibernation and deep sleep

A timer on the clock saves its live count and re-anchors it at the restored
guest time. The deep-sleep park stops guest time, so a timer stops as on
silicon. The contracts are in [hibernation.md](hibernation.md) and
[deep_sleep.md](deep_sleep.md).
