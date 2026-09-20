/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpadc.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
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
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_GPADC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_GPADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The channels.
 *
 * The first group are rails the part can measure with no pin involved.
 * Each sits behind a scaler of its own, which is why a raw count from this
 * converter means nothing until it is known which channel produced it --
 * and why this driver reports millivolts rather than counts.
 *
 * The last group are the four pins the converter can reach.  Putting a pin
 * into its analogue mode is the board's business, not this driver's.
 */

#define DA1470X_ADC_VBAT   0   /* Battery, through an internal 0.189 scaler */
#define DA1470X_ADC_VSYS   1   /* System supply, 0.157 */
#define DA1470X_ADC_VBUS   2   /* Charger input, 0.164 */
#define DA1470X_ADC_V30    3   /* 3.0 V rail */
#define DA1470X_ADC_V18F   4   /* 1.8 V flash rail */
#define DA1470X_ADC_V18P   5   /* 1.8 V peripheral rail */
#define DA1470X_ADC_V18    6   /* 1.8 V rail */
#define DA1470X_ADC_V14    7   /* 1.4 V rail */
#define DA1470X_ADC_V12    8   /* 1.2 V core rail */
#define DA1470X_ADC_P0_5   9   /* ADC0 on P0[5] */
#define DA1470X_ADC_P0_6   10  /* ADC1 on P0[6] */
#define DA1470X_ADC_P0_27  11  /* ADC2 on P0[27] */
#define DA1470X_ADC_P0_30  12  /* ADC3 on P0[30] */

#define DA1470X_ADC_NCHANNELS 13

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * ACCURACY: read this before believing a number.
 *
 * Readings are stable and repeatable to a few millivolts, and they do
 * track reality -- the battery channel follows the supply switch and
 * rises the moment a charge starts.  RELATIVE changes can be trusted.
 *
 * ABSOLUTE values cannot, yet, and the error depends on the attenuator.
 * Measured on a DA14706-00HZDEVKT-P against the one hard reference
 * available, a charger regulating the battery in constant voltage at a
 * programmed 4200 mV:
 *
 *   attenuator 0   the battery reads 3658 mV for a true 4200, 14.8% low
 *   attenuator 1   the same battery reads 2505 mV,             40% low
 *   attenuator 2   a 1.8 V rail reads 1772 mV,                 1.6% low
 *
 * The reported value ought not to depend on the attenuator at all,
 * since the conversion divides it back out.  That it does means the
 * part does not attenuate by the (attn + 1) the vendor's own driver
 * assumes; working backwards from the two battery readings, the step
 * from 0 to 1 is about 2.9 rather than 2.
 *
 * So the attenuator model here is wrong, and the factory calibration
 * this part keeps in one-time programmable memory is not read or
 * applied either.  Chopping is enabled, so the converter's offset
 * cancels, but nothing corrects its gain.  Settling this needs a meter
 * on the board rather than more reasoning: until then treat an absolute
 * reading as indicative, and do not compute a state of charge from it.
 *
 * Name: da1470x_gpadc_initialize
 *
 * Description:
 *   Register the general purpose converter as a NuttX ADC device,
 *   conventionally "/dev/adc0".
 *
 *   A read of the device returns one sample per channel in chanlist, in
 *   that order, each time a conversion is asked for with ANIOC_TRIGGER.
 *   The value is in MILLIVOLTS, not in counts: every internal channel has
 *   a different scaler in front of it, so a count is not comparable with
 *   the count from any other channel and is not much use on its own.
 *
 * Input Parameters:
 *   devpath  - where to register it
 *   chanlist - the channels to convert, from the DA1470X_ADC_* above
 *   nchan    - how many
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.
 *
 ****************************************************************************/

int da1470x_gpadc_initialize(const char *devpath,
                             const uint8_t *chanlist, int nchan);

/****************************************************************************
 * Name: da1470x_gpadc_sample_mv
 *
 * Description:
 *   Convert one channel and return it in millivolts.  Synchronous: a
 *   conversion with the oversampling this driver asks for takes well under
 *   a millisecond, so this spins rather than sleeping, and can be called
 *   before there is anything to yield to.
 *
 *   It shares the converter with the character device and takes the same
 *   lock, so a caller must be able to wait.
 *
 * Input Parameters:
 *   channel - one of DA1470X_ADC_*
 *   mv      - where to put the answer
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.
 *
 ****************************************************************************/

int da1470x_gpadc_sample_mv(uint8_t channel, int *mv);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_GPADC_H */
