/****************************************************************************
 * arch/arm/include/da1470x/gpu.h
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

#ifndef __ARCH_ARM_INCLUDE_DA1470X_GPU_H
#define __ARCH_ARM_INCLUDE_DA1470X_GPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <nuttx/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Pixel formats understood by the 2D GPU (D/AVE 2D encodings) */

#define DA1470X_GPU_FMT_ALPHA8      0
#define DA1470X_GPU_FMT_RGB565      1
#define DA1470X_GPU_FMT_ARGB8888    2
#define DA1470X_GPU_FMT_ARGB4444    3
#define DA1470X_GPU_FMT_ARGB1555    4
#define DA1470X_GPU_FMT_RGBA8888    6
#define DA1470X_GPU_FMT_RGBA4444    7

/* ioctl commands of /dev/gpuN.  Numbers are chosen above the generic
 * character-driver commands to avoid clashes.
 */

#define GPUIOC_REVISION   _DIOC(0x80)  /* OUT: uint32_t D2_HWREVISION */
#define GPUIOC_FILL       _DIOC(0x81)  /* IN:  struct da1470x_gpu_fill_s */
#define GPUIOC_BLIT       _DIOC(0x82)  /* IN:  struct da1470x_gpu_blit_s */
#define GPUIOC_WAIT       _DIOC(0x83)  /* Wait for the GPU to go idle */
#define GPUIOC_TEXBOX     _DIOC(0x84)  /* IN:  struct da1470x_gpu_texbox_s */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* A surface in memory reachable by the GPU (SYSRAM or PSRAM) */

struct da1470x_gpu_surface_s
{
  uintptr_t addr;      /* Address of pixel (0,0) */
  uint16_t  width;     /* Width in pixels */
  uint16_t  height;    /* Height in pixels */
  uint16_t  pitch;     /* Line pitch in pixels */
  uint8_t   format;    /* DA1470X_GPU_FMT_* */
};

/* Solid rectangle fill */

struct da1470x_gpu_fill_s
{
  struct da1470x_gpu_surface_s dst;
  uint16_t x;          /* Top-left corner in dst */
  uint16_t y;
  uint16_t w;          /* Size in pixels */
  uint16_t h;
  uint32_t color;      /* ARGB8888 */
};

/* Rectangle copy with format conversion */

struct da1470x_gpu_blit_s
{
  struct da1470x_gpu_surface_s src;
  struct da1470x_gpu_surface_s dst;
  uint16_t sx;         /* Top-left corner in src */
  uint16_t sy;
  uint16_t dx;         /* Top-left corner in dst */
  uint16_t dy;
  uint16_t w;          /* Size in pixels */
  uint16_t h;
  uint8_t  alpha;      /* Opacity applied on top of the source alpha:
                        * 255 leaves the source alone, 0 draws nothing */
};

/* A half-plane limiter: the pixel (x, y) of the destination box is
 * inside when start + x * xadd + y * yadd >= 0, all in 16.16 fixed
 * point and evaluated at pixel centres (the caller folds the 0.5
 * offsets into start).  Up to four limiters bound a convex quad.
 */

struct da1470x_gpu_limiter_s
{
  int32_t start;
  int32_t xadd;
  int32_t yadd;
};

/* Texture-mapped box: every destination pixel of the w x h box at
 * (x, y) samples the source at (u, v) = (ustart + x * uxadd + y * uyadd,
 * vstart + x * vxadd + y * vyadd), 16.16 fixed point in source pixels,
 * clamped to the source; optionally bilinear filtered and bounded by
 * limiters.  This is a rotation, a scaling or any affine transform.
 */

#define DA1470X_GPU_MAX_LIMITERS 4

struct da1470x_gpu_texbox_s
{
  struct da1470x_gpu_surface_s src;
  struct da1470x_gpu_surface_s dst;
  uint16_t x;          /* Destination box */
  uint16_t y;
  uint16_t w;
  uint16_t h;
  uint8_t  alpha;      /* As for a blit */
  uint8_t  filter;     /* Non-zero: bilinear filtering */
  uint8_t  nlimiters;  /* 0..4 */
  int32_t  ustart;     /* Mapping at the box origin, 16.16 */
  int32_t  uxadd;
  int32_t  uyadd;
  int32_t  vstart;
  int32_t  vxadd;
  int32_t  vyadd;
  struct da1470x_gpu_limiter_s lim[DA1470X_GPU_MAX_LIMITERS];
};

#endif /* __ARCH_ARM_INCLUDE_DA1470X_GPU_H */
