/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpu.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_GPU_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_GPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <arch/da1470x/gpu.h>

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
 * Name: da1470x_gpu_initialize
 *
 * Description:
 *   Power and clock the GPU, configure it for register-mode operation and
 *   attach its interrupt.  Returns OK or a negated errno.
 *
 ****************************************************************************/

int da1470x_gpu_initialize(void);

/****************************************************************************
 * Name: da1470x_gpu_register
 *
 * Description:
 *   Register the GPU as a character device (e.g. /dev/gpu0).
 *
 ****************************************************************************/

int da1470x_gpu_register(const char *devpath);

/****************************************************************************
 * Name: da1470x_gpu_fill / da1470x_gpu_blit
 *
 * Description:
 *   In-kernel entry points for the two primitives.  Both block until the
 *   GPU has finished.
 *
 ****************************************************************************/

int da1470x_gpu_fill(const struct da1470x_gpu_fill_s *op);
int da1470x_gpu_blit(const struct da1470x_gpu_blit_s *op);

/****************************************************************************
 * Name: da1470x_gpu_revision
 ****************************************************************************/

uint32_t da1470x_gpu_revision(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_GPU_H */
