# Timers and clocks

Every clock a guest can read derives from one time base. That base is the
number of guest instruction cycles the JIT has executed, at the datasheet CPU
rate of the SoC.

## Guest time

- **Guest time is a cycle count.** The JIT counts the cycles of every
  translated instruction. That count, at the CPU rate, is guest time.
- **The CPU rate is the datasheet product, never a rounded print.** A clock
  table gives a multiplier of the crystal. The rate is the crystal times the
  multiplier. SA-1110 Dev Man § 8.2 Table 8-1 gives CCF 01011 as 3,686,400 x 56
  = 206,438,400 Hz, printed "206.4". A board's public product clock selects
  the table row and nothing else. A PLL-programmed SoC takes the rate the
  guest programs, from the register's own formula.
- **A timer match is an event at a cycle count.** It is delivered on the JIT
  thread when the count reaches it. No host thread delivers a guest tick. No
  host deadline exists.
- **The emulator runs at or behind wall, never ahead.** When the guest is
  behind wall by more than a bounded window, the excess is forgiven, never
  caught up. That window is the max-fallback of the Dolphin `CoreTiming`
  model. A slow host therefore slows the whole guest uniformly. The guest's
  own clock stays exact against guest time. A host-referenced observer sees
  the replay of at most one window when a busy guest turns idle. The guest
  cannot see it.

## Idle

- **A wait-for-interrupt completes only on an interrupt.** ARM DDI 0406C
  B1.8.14: a wake-up event is a physical IRQ regardless of CPSR.I. The idle
  wait advances guest time to the next armed event, paced to wall, and never
  ahead of wall. There is no timeout. A timeout is a spurious wake, and the
  guest's idle arithmetic credits it as slept time.
- **Idle exit on SA-11xx and PXA2xx follows ICCR.DIM** (SA-1110 § 9.5.2.2,
  PXA255 § 4.2.2.3, PXA27x § 25.5.6). With DIM clear, any set pending bit
  ends idle, masked or not. With DIM set, only an unmasked one ends it. The
  wake completes the wait, and execution continues at the next instruction.
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

The OAL owns one hardware match timer. Its tick ISR credits the millisecond
counter that `GetTickCount` reads, then re-arms the match. `OEMIdle` is the
tickless idle. It arms one long match at the scheduler's deadline, waits,
and credits the slept periods on wake. The guest's arithmetic around that
timer is what makes it dangerous, and the arithmetic differs per kernel.

- **The catch-up loop.** Many SA-11xx and PXA kernels re-arm with
  `OSMR0 += period`. They loop while `OSMR0 - OSCR` is under a small margin,
  and they credit one millisecond per pass. The subtraction is unsigned. An
  entry that is late by about one period lands in a residue band with roughly
  0.5 % probability. In that band the loop walks 2^32 / period passes. At
  3.6864 MHz that is 1,165,211 passes, or 19.4 minutes of guest time inside
  one interrupt. That is the tick death. A store that lands at or behind the
  counter laps 2^32 ticks (1165 s at 3.6864 MHz). The channel is then dead
  until the guest rewrites it. On the cycle clock an entry is never late, so
  the loop runs one pass.
- **Per-pass and per-entry credit.** Some kernels credit one millisecond per
  loop pass. Some credit once per entry (the iPAQ kernels). One credits 25 ms
  per pass (SIMpad CE 4). One re-checks after the re-arm and credits again
  when the distance is small (PXA27x). One re-bases only when the re-arm
  lands close (SmartBook).
- **OEMIdle banking.** A banking kernel reads the phase since the last tick
  at idle entry. It adds an accumulator, credits the whole periods, and arms
  one long match at the scheduler deadline. On wake it credits the slept
  periods from the changed counter, or from the counter-derived quotient
  when the tick never came. Then it re-bases the match one period out. A
  non-banking kernel (the iPAQ pair) arms the long match from the counter at
  entry and loses the sub-period phase on every idle. Some kernels idle with
  IRQs masked around the banking read. On such a kernel a tick that arrives
  during the read is credited twice.
- **The guest's arithmetic is the guest's.** The iPAQ phase erasure runs its
  clock 2 % slow. The bank-and-exit double runs the PXA kernels that do it up
  to 1 % fast. An entry banks a full period at the scheduler's 1 ms deadline
  and exits without arming. The still-armed tick then credits the same
  period again. Both run on silicon and in CERF alike. Neither is a CERF defect. A
  fix that needs a kernel's handler is the wrong fix.
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

- The guest's own tick counter against guest time is the criterion. It is
  exact on every ROM the timer serves, under a starved host and then an idle
  host. The guest runs a load during the measurement, never idle. On an idle host the
  guest clock also runs at wall.
- A host-referenced tick profiler reads the throttle's replay. It is not the
  verdict. The guest's counter is.
- Nothing fast-forwards. No recovery loop iterates. No channel laps. Nothing
  fatals. Hibernation round trip and deep-sleep resume keep working on the
  same binary.

## Hibernation and deep sleep

A timer on the clock saves its live count and re-anchors it at the restored
guest time. The deep-sleep park stops guest time, so a timer stops as on
silicon. The contracts are in [hibernation.md](hibernation.md) and
[deep_sleep.md](deep_sleep.md).
