/****************************************************************************
 * include/nuttx/power/max20303.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ***************************************************************************
 */

#ifndef __INCLUDE_NUTTX_POWER_MAX20303_H
#define __INCLUDE_NUTTX_POWER_MAX20303_H

/****************************************************************************
 * Included Files
 ***************************************************************************
 */

#include <nuttx/i2c/i2c_master.h>
#include <nuttx/fs/ioctl.h>

/****************************************************************************
 * Public Types
 ***************************************************************************
 */

/* One haptic sample: drive the motor at `amplitude` percent of full scale
 * for `duration_ms`. An array of these written to the haptic device plays
 * synchronously (the write blocks for the pattern's total duration).
 * amplitude 0 = pause segment.
 */

begin_packed_struct struct max20303_haptic_sample_s
{
  uint8_t amplitude;      /* 0..100 % of full scale */
  uint16_t duration_ms;
} end_packed_struct;

/* ETRG (externally triggered stored pattern) -- the hardware envelope.
 *
 * The PMIC plays overdrive -> active -> brake autonomously once triggered,
 * so the host does not stay in the loop and nothing blocks for the pattern
 * duration. This is the mode to use for short alerts; the write() path
 * above (RTI2C) remains for long, arbitrary multi-segment patterns.
 *
 * Why an overdrive and a brake at all: an LRA is a spring-mass system. It
 * takes tens of ms to spin up and rings down for tens of ms after drive
 * stops. A flat pulse therefore feels like a mushy blob. Overdrive beats
 * the inertia on the way in; the brake (reverse phase) stops the mass dead
 * on the way out. The result is both crisper AND perceptually gentler than
 * a longer flat drive.
 *
 * Fields are RAW CHIP UNITS, deliberately not percent/ms, because the
 * quantisation is coarse enough that hiding it would mislead:
 *
 *   *_amp : bit7 = direction (1 = reverse, which is what makes a brake a
 *           brake), bits6:0 = amplitude, LSB = 0.78 % of VFS.
 *           So code ~= percent * 128 / 100.
 *   od_dur, brk_dur : LSB = 5 ms
 *   act_dur         : LSB = 10 ms   <-- note: NOT the same as the others
 *
 * Minimum expressible envelope is therefore 5 + 10 + 5 = 20 ms.
 */

begin_packed_struct struct max20303_haptic_etrg_s
{
  uint8_t od_amp;         /* overdrive: sign + 0.78 %VFS/LSB */
  uint8_t od_dur;         /* overdrive duration, 5 ms/LSB    */
  uint8_t act_amp;        /* active drive amplitude          */
  uint8_t act_dur;        /* active duration, 10 ms/LSB      */
  uint8_t brk_amp;        /* brake amplitude (set bit7!)     */
  uint8_t brk_dur;        /* brake duration, 5 ms/LSB        */
} end_packed_struct;

/* Vendor ioctls on the haptic device.
 *
 * Numbered high in the battery ioctl space so upstream additions to
 * battery_ioctl.h cannot collide.
 */

/* Arg: const struct max20303_haptic_etrg_s *. Loads the envelope into the
 * PMIC (one AP command). Persists until changed -- set it once at init.
 */

#define MAX20303_HAPTIC_IOC_SETETRG   _BATIOC(0x00a0)

/* Arg: none. Fires the stored envelope and RETURNS IMMEDIATELY -- the PMIC
 * plays it autonomously. Safe to call from a latency-sensitive thread,
 * unlike write().
 */

#define MAX20303_HAPTIC_IOC_TRIGGER   _BATIOC(0x00a1)

/* RAMHP -- the PMIC's 256 x 24-bit pattern RAM.
 *
 * Where ETRG holds exactly one envelope, RAMHP holds a whole vocabulary at
 * once: patterns live at addresses in the RAM and are played by address, so
 * switching between them costs no reprogramming. It is also the only mode
 * that can express a GAP or a REPEAT, which is what makes a double-tap
 * possible at all (ETRG's single overdrive/active/brake envelope cannot).
 *
 * Fields are logical, not pre-packed: the driver owns the 24-bit wire layout
 * (Table 3) so no caller hand-packs bits.
 *
 *   amp  : bit7 = direction (1 = reverse => braking),
 *          bits6:0 = 0.78 %VFS/LSB
 *   dur  : 5 ms/LSB, 0..31  (0..155 ms) -- drive time
 *   wait : 5 ms/LSB, 0..31  (0..155 ms) -- silence AFTER this sample
 *   rpt  : 0..15 extra repeats of this sample, or of the WHOLE pattern when
 *          nls == MAX20303_HAPTIC_NLS_LOOP
 *   nls  : see the MAX20303_HAPTIC_NLS_* codes
 *
 * `wait` and `rpt` living inside a sample is what keeps patterns cheap: a
 * double tap costs the same samples as a single one, and a rhythmic buzz
 * costs one sample rather than one per beat.
 */

#define MAX20303_HAPTIC_NLS_LAST   0x0  /* last sample in the pattern       */
#define MAX20303_HAPTIC_NLS_MORE   0x1  /* another sample follows           */
#define MAX20303_HAPTIC_NLS_INTERP 0x2  /* interpolate with the next sample */
#define MAX20303_HAPTIC_NLS_LOOP   0x3  /* last + repeat whole pattern rptx */

#define MAX20303_HAPTIC_RAM_SAMPLES 256
#define MAX20303_HAPTIC_AMP_REVERSE 0x80

begin_packed_struct struct max20303_haptic_ram_sample_s
{
  uint8_t amp;
  uint8_t dur;
  uint8_t wait;
  uint8_t rpt;
  uint8_t nls;
} end_packed_struct;

begin_packed_struct struct max20303_haptic_ramload_s
{
  uint8_t addr;                                        /* 0..255           */
  uint8_t count;
  FAR const struct max20303_haptic_ram_sample_s *samples;
} end_packed_struct;

/* Arg: const struct max20303_haptic_ramload_s *. Uploads samples into the
 * pattern RAM and VERIFIES each one by readback -- a half-written bank is
 * otherwise indistinguishable from a badly-designed pattern. The RAM is
 * volatile and is lost on PMIC brownout, not just on host reset, so callers
 * must be prepared to reload.
 */

#define MAX20303_HAPTIC_IOC_RAMLOAD   _BATIOC(0x00a3)

/* Arg: const struct max20303_haptic_ramplay_s *. Fires the pattern at
 * `addr` and returns immediately -- the PMIC sequences it autonomously.
 *
 * `duration_ms` is how long the pattern runs. The driver does not need it to
 * play, only to schedule the mandatory end-of-vibration teardown: the chip
 * must be returned to mode 0 once a pattern finishes, or HptDrvTmo
 * eventually aborts the "still running" vibration and latches HptLock,
 * after which every play silently does nothing. Pass the real duration; too
 * short tears down mid-pattern, too long risks the timeout.
 */

begin_packed_struct struct max20303_haptic_ramplay_s
{
  uint8_t  addr;
  uint16_t duration_ms;
} end_packed_struct;

#define MAX20303_HAPTIC_IOC_RAMPLAY   _BATIOC(0x00a4)

/* Arg: unsigned long seconds (clamped 1..63). The hardware aborts any
 * vibration outlasting this and latches HptLock -- it is the backstop
 * against a runaway. Keep it just above the longest pattern; widening it
 * to fit a long pattern trades away the safety net.
 */

#define MAX20303_HAPTIC_IOC_SETTIMEOUT _BATIOC(0x00a5)

/* Arg: none. Real abort -- stops an in-flight pattern. Note the LRA rings
 * down rather than stopping dead, because braking would need a trigger and a
 * trigger is what we are cancelling.
 */

#define MAX20303_HAPTIC_IOC_STOP      _BATIOC(0x00a6)

/* Arg: none. Clears a latched HptLock (set by a timeout abort). The lock
 * survives a host reset, so without this a single runaway leaves haptics
 * dead across reboots with no obvious cause.
 */

#define MAX20303_HAPTIC_IOC_UNLOCK    _BATIOC(0x00a7)

/* Arg: FAR int *. Measured LRA resonant frequency in Hz, from the chip's
 * back-EMF tracker (0 = never locked). Drives the motor briefly to take
 * the measurement, so it blocks for ~200 ms; call it at bring-up, not on
 * the hot path. Meaningful only in LRA mode.
 */

#define MAX20303_HAPTIC_IOC_RESONANCE _BATIOC(0x00a2)

/****************************************************************************
 * Public Function Prototypes
 ***************************************************************************
 */

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: max20303_register
 *
 * Description:
 *   Register the MAX20303 wearable PMIC:
 *
 *   - battery fuel gauge (ModelGauge, second I2C address 0x36) as a
 *     standard battery_gauge device at `batdev` (e.g. "/dev/bat0"):
 *     BATIOC_STATE / BATIOC_VOLTAGE / BATIOC_CAPACITY.
 *   - Li+ charger configured (4.20 V, autostop/autorestart, 200 mA input
 *     limit) and enabled.
 *   - haptic driver as a character device at `hapticdev` (e.g.
 *     "/dev/haptic0"): write() an array of max20303_haptic_sample_s to
 *     play a vibration pattern synchronously (RTI2C direct drive).
 *
 *   Power rails (buck/boost) are deliberately NOT touched: on a working
 *   board the host runs from them. LDO1/LDO2 are programmed only when
 *   CONFIG_MAX20303_LDO1_MV / _LDO2_MV are non-zero.
 *
 *   Probes HARDWARE_ID (expect 0x02) at the fixed PMIC address 0x28;
 *   returns -ENODEV when absent.
 *
 ***************************************************************************
 */

int max20303_register(FAR struct i2c_master_s *i2c,
                      FAR const char *batdev, FAR const char *hapticdev);

#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_NUTTX_POWER_MAX20303_H */
