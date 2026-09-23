/****************************************************************************
 * arch/arm/src/da1470x/da1470x_vad.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_VAD_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_VAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <arch/chip/vad.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_vad_register
 *
 * Description:
 *   Create the character device through which the voice activity detector
 *   is configured and its detections are read.  The block is left in
 *   standby; opening the device does not start it, a mode change does.
 *
 * Input Parameters:
 *   devpath - Where the device goes, conventionally "/dev/vad0".
 *   config  - Initial tuning, or NULL for the values the block resets to.
 *
 * Returned Value:
 *   Zero on success, a negated errno value on failure.
 *
 ****************************************************************************/

int da1470x_vad_register(const char *devpath,
                         const struct vad_config_s *config);

/****************************************************************************
 * Name: da1470x_vad_wakeup_enable
 *
 * Description:
 *   Let a detection wake the system from a sleep deep enough to have
 *   stopped the processor, by giving the power domain controller an entry
 *   that names the detector as the trigger.
 *
 *   The 3V0 rail has to stay alive in sleep for the analogue front end to
 *   keep listening; this does not arrange that.
 *
 * Input Parameters:
 *   enable - Add the entry, or take it away again.
 *
 * Returned Value:
 *   Zero on success, a negated errno value on failure.
 *
 ****************************************************************************/

int da1470x_vad_wakeup_enable(bool enable);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_VAD_H */
