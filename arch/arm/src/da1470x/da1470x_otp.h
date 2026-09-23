/****************************************************************************
 * arch/arm/src/da1470x/da1470x_otp.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_OTP_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_OTP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Size of the OTP array in 32-bit words (64 KiB) */

#define DA1470X_OTP_NWORDS     (16 * 1024)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_otp_acquire
 *
 * Description:
 *   Power the OTP controller up and put it in read mode so that the array
 *   can be read through its memory window.  Each call must be matched by a
 *   call to da1470x_otp_release().  Calls nest.
 *
 *   The controller timings are valid as long as the AMBA clock is not
 *   faster than 32MHz, so the array is normally read from the early boot
 *   path, before the system clock is switched to the PLL.
 *
 * Returned Value:
 *   Zero on success, a negated errno value if the controller does not
 *   report the requested mode.
 *
 ****************************************************************************/

int da1470x_otp_acquire(void);

/****************************************************************************
 * Name: da1470x_otp_release
 *
 * Description:
 *   Undo one da1470x_otp_acquire().  The controller is put back in standby
 *   and its clock is gated when the last user releases it.
 *
 ****************************************************************************/

void da1470x_otp_release(void);

/****************************************************************************
 * Name: da1470x_otp_read32
 *
 * Description:
 *   Read one word of the OTP array.  The caller must hold the controller
 *   through da1470x_otp_acquire().
 *
 * Input Parameters:
 *   index - Word index in the array, 0..DA1470X_OTP_NWORDS - 1.
 *
 ****************************************************************************/

uint32_t da1470x_otp_read32(unsigned int index);

/****************************************************************************
 * Name: da1470x_otp_read
 *
 * Description:
 *   Read a block of words from the OTP array, acquiring and releasing the
 *   controller around the transfer.
 *
 * Input Parameters:
 *   index  - Word index of the first word to read.
 *   dest   - Where the words are stored.
 *   nwords - Number of words to read.
 *
 * Returned Value:
 *   Zero on success, a negated errno value on failure.
 *
 ****************************************************************************/

int da1470x_otp_read(unsigned int index, uint32_t *dest, size_t nwords);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_OTP_H */
