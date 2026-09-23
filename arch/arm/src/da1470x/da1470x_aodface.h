/****************************************************************************
 * arch/arm/src/da1470x/da1470x_aodface.h
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

/* The always-on face: a recipe, not a picture.  1-bit glyph masks and a
 * list of where to put them, rendered by the display driver itself once a
 * minute while everything above it sleeps.  The record is the AODF v1
 * format of the stm32_stream_bridge (see its doc/aod_face.md and
 * tools/aod_face_gen.py), byte for byte, so the same generator and the
 * same uploaded files serve both targets.
 *
 * AODF v1 addresses a 240 x 240 face with 8-bit coordinates.  On a larger
 * panel the face is drawn centred, at the origin the caller passes in.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_AODFACE_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_AODFACE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define AODFACE_MAGIC      0x46444f41u   /* 'A','O','D','F' little-endian */
#define AODFACE_VER        1
#define AODFACE_MAX_ITEMS  16
#define AODFACE_MAX_LEN    32768
#define AODFACE_BLOB_OFF   0xa0
#define AODFACE_SIZE       240           /* v1 faces are 240 x 240 */

/* Item kinds.  Seconds and date kinds are reserved and rejected: the face
 * is redrawn once a minute, so a seconds field would sit frozen.
 */

#define AODFACE_K_LITERAL  0
#define AODFACE_K_HH10     1
#define AODFACE_K_HH1      2
#define AODFACE_K_MM10     3
#define AODFACE_K_MM1      4

#define AODFACE_F_BLANK_LEADING_ZERO  0x01

/* Validation results, the same values as the bridge's FACE_E_* */

#define AODFACE_E_OK       0
#define AODFACE_E_MAGIC    1
#define AODFACE_E_VER      2
#define AODFACE_E_LEN      3
#define AODFACE_E_GEOM     4
#define AODFACE_E_ITEM     5
#define AODFACE_E_CRC      6

/****************************************************************************
 * Public Types
 ****************************************************************************/

begin_packed_struct struct aodface_item_s
{
  uint8_t kind;
  uint8_t set;       /* 0 or 1 -- lets two rows differ in style */
  uint8_t x;
  uint8_t y;
  uint8_t fg;        /* RGB222, packed b5:4 = R, b3:2 = G, b1:0 = B */
  uint8_t literal;   /* Glyph index when kind is AODFACE_K_LITERAL */
  uint8_t flags;
  uint8_t rsv;
} end_packed_struct;

begin_packed_struct struct aodface_hdr_s
{
  uint32_t magic;                              /* 0x00 */
  uint8_t  ver;                                /* 0x04 */
  uint8_t  flags;                              /* 0x05 */
  uint16_t total_len;                          /* 0x06 whole record */
  uint8_t  s0_w;                               /* 0x08 first glyph set */
  uint8_t  s0_h;
  uint8_t  s0_n;
  uint8_t  s1_w;                               /* 0x0b second set, or 0 */
  uint8_t  s1_h;
  uint8_t  s1_n;
  uint8_t  item_count;                         /* 0x0e */
  uint8_t  band_y0;                            /* 0x0f */
  uint8_t  band_y1;                            /* 0x10 exclusive */
  uint8_t  bg;                                 /* 0x11 RGB222 */
  uint8_t  rsv0[2];                            /* 0x12 */
  uint32_t crc32;                              /* 0x14 */
  uint8_t  rsv1[8];                            /* 0x18 */

  /* 0x20: the item table */

  struct aodface_item_s items[AODFACE_MAX_ITEMS];

  /* 0xa0: glyph blob, set 0 then set 1, each n * stride * h bytes with
   * stride = (w + 7) / 8, row-major, MSB first within each byte.
   */
} end_packed_struct;

/* Where to draw: an RGB565 frame buffer and the face's origin in it */

struct aodface_fb_s
{
  FAR uint8_t *mem;      /* First pixel of the panel-sized buffer */
  uint16_t     stride;   /* Bytes per line */
  uint16_t     xres;     /* Panel size in pixels */
  uint16_t     yres;
  uint16_t     ox;       /* Top left corner of the 240 x 240 face */
  uint16_t     oy;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Check a candidate record.  Returns AODFACE_E_OK or one of the other
 * AODFACE_E_* codes.  Nothing may be rendered from a record that has not
 * passed: the blitter relies on these bounds and carries no clipping.
 */

int aodface_validate(FAR const void *rec, size_t len);

/* The compiled-in face, or NULL when there is none */

FAR const struct aodface_hdr_s *aodface_builtin(void);

/* Rows the face repaints each minute, in panel coordinates, y1 exclusive */

void aodface_band(FAR const struct aodface_hdr_s *face,
                  FAR const struct aodface_fb_s *fb,
                  FAR uint16_t *y0, FAR uint16_t *y1);

/* Fill the whole panel with the face background, for the entry frame */

void aodface_clear_all(FAR const struct aodface_hdr_s *face,
                       FAR const struct aodface_fb_s *fb);

/* Repaint the band and draw the digits.  Does not touch the panel. */

void aodface_render(FAR const struct aodface_hdr_s *face,
                    FAR const struct aodface_fb_s *fb,
                    uint8_t hh, uint8_t mm);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_AODFACE_H */
