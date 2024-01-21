/****************************************************************************
 * arch/arm/src/imxrt/hardware/rt5xx/imxrt5xx_dmamux.h
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

#ifndef __ARCH_ARM_SRC_IMXRT_HARDWARE_RT5XX_IMXRT5XX_DMAMUX_H
#define __ARCH_ARM_SRC_IMXRT_HARDWARE_RT5XX_IMXRT5XX_DMAMUX_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

/* Peripheral DMA request channels */
#define IMXRT_DMACHAN_FLEXCOMM0_RX                  0  /* FLEXCOMM0 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM0_TX                  1  /* FLEXCOMM0 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM1_RX                  2  /* FLEXCOMM1 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM1_TX                  3  /* FLEXCOMM1 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM2_RX                  4  /* FLEXCOMM2 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM2_TX                  5  /* FLEXCOMM2 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM3_RX                  6  /* FLEXCOMM3 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM3_TX                  7  /* FLEXCOMM3 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM4_RX                  8  /* FLEXCOMM4 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM4_TX                  9  /* FLEXCOMM4 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM5_RX                 10  /* FLEXCOMM5 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM5_TX                 11  /* FLEXCOMM5 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM6_RX                 12  /* FLEXCOMM6 RX enable */
#define IMXRT_DMACHAN_FLEXCOMM6_TX                 13  /* FLEXCOMM6 TX enable */
#define IMXRT_DMACHAN_FLEXCOMM7_RX                 14  /* FLEXCOMM7 RX enable */
#define IMXRT_DMACHAN_FLEXCOMM7_TX                 15  /* FLEXCOMM7 TX enable */
#define IMXRT_DMACHAN_DMIC0_CH0_FLEXCOMM8_RX       16  /* DMIC0 channel 0 / FLEXCOMM8 RX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH1_FLEXCOMM8_TX       17  /* DMIC0 channel 1 / FLEXCOMM8 TX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH2_FLEXCOMM9_RX       18  /* DMIC0 channel 2 / FLEXCOMM9 RX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH3_FLEXCOMM9_TX       19  /* DMIC0 channel 3 / FLEXCOMM9 TX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH4_FLEXCOMM10_RX      20  /* DMIC0 channel 4 / FLEXCOMM10 RX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH5_FLEXCOMM10_TX      21  /* DMIC0 channel 5 / FLEXCOMM10 TX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH6_FLEXCOMM13_RX      22  /* DMIC0 channel 6 / FLEXCOMM10 RX DMA enable */
#define IMXRT_DMACHAN_DMIC0_CH7_FLEXCOMM13_TX      23  /* DMIC0 channel 7 / FLEXCOMM13 TX DMA enable */
#define IMXRT_DMACHAN_I3C0_RX                      24  /* I3C RX enable set */
#define IMXRT_DMACHAN_I3C0_TX                      25  /* I3C TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM14_RX                26  /* FLEXCOMM14 RX enable set */
#define IMXRT_DMACHAN_FLEXCOMM14_TX                27  /* FLEXCOMM14 TX enable set */
#define IMXRT_DMACHAN_FLEXCOMM16_RX                28  /* FLEXCOMM16 RX enable */
#define IMXRT_DMACHAN_FLEXCOMM16_TX                29  /* FLEXCOMM16 RX enable */
#define IMXRT_DMACHAN_I3C1_RX                      30  /* I3C1_RX enable */
#define IMXRT_DMACHAN_I3C1_TX                      31  /* I3C1_TX enable */
#define IMXRT_DMACHAN_FLEXCOMM11_RX                32  /* FLEXCOMM11_RX */
#define IMXRT_DMACHAN_FLEXCOMM11_TX                33  /* FLEXCOMM11_TX */
#define IMXRT_DMACHAN_FLEXCOMM12_RX                34  /* FLEXCOMM12_RX */
#define IMXRT_DMACHAN_FLEXCOMM12_TX                35  /* FLEXCOMM12_TX */
#define IMXRT_DMACHAN_HASHCRYPT_IN                 36  /* HASHCRYPT_IN */

#define IMXRT_DMA_NCHANNELS                        37  /* Includes reserved channels */

#endif /* __ARCH_ARM_SRC_IMXRT_HARDWARE_RT5XX_IMXRT5XX_DMAMUX_H */
