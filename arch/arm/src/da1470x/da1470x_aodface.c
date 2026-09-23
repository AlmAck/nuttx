/****************************************************************************
 * arch/arm/src/da1470x/da1470x_aodface.c
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

/* Validate and render an always-on face.  Ported from the
 * stm32_stream_bridge's face.c, whose record format this shares.
 *
 * The renderer is deliberately simple: read a bit, set a pixel.  Every
 * bound was already checked by aodface_validate(), which is what lets the
 * inner loop carry no clipping test.  That is a contract, not an
 * optimisation: if validation is ever loosened, the blitter has to gain the
 * checks back in the same change.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include "da1470x_aodface.h"

#ifdef CONFIG_DA1470X_LCDC_AOD_BUILTIN_FACE
#  include "da1470x_aodface_sample.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The header is the on-the-wire layout, so its size is load-bearing */

static_assert(sizeof(struct aodface_hdr_s) == AODFACE_BLOB_OFF,
              "AODF header must be 160 bytes");

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* CRC-32, reflected, polynomial 0xedb88320, a nibble at a time: 64 bytes of
 * table, and it only runs when a face is installed.
 */

static const uint32_t g_crc_nibble[16] =
{
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* The CRC field itself (bytes 0x14..0x17) counts as zero, so a record can
 * be checked in place.
 */

static uint32_t aodface_crc32(FAR const uint8_t *p, size_t len)
{
  uint32_t crc = 0xffffffff;
  size_t i;

  for (i = 0; i < len; i++)
    {
      crc ^= (i >= 0x14 && i < 0x18) ? 0 : p[i];
      crc  = (crc >> 4) ^ g_crc_nibble[crc & 0x0f];
      crc  = (crc >> 4) ^ g_crc_nibble[crc & 0x0f];
    }

  return ~crc;
}

static uint32_t set_stride(uint8_t w)
{
  return ((uint32_t)w + 7) / 8;
}

static uint32_t set_bytes(uint8_t w, uint8_t h, uint8_t n)
{
  return set_stride(w) * h * n;
}

/* RGB222 to RGB565.  In idle mode the panel shows eight colours, one bit a
 * channel, so only the top bit of each level is likely to survive there;
 * the full value is still written so a face looks right outside it.
 */

static uint16_t rgb222_to_565(uint8_t c)
{
  uint32_t r = (c >> 4) & 3;
  uint32_t g = (c >> 2) & 3;
  uint32_t b = c & 3;

  return (uint16_t)(((r * 31 + 1) / 3) << 11 |
                    (g * 21) << 5 |
                    ((b * 31 + 1) / 3));
}

static void fill_rows(FAR const struct aodface_fb_s *fb, uint16_t y0,
                      uint16_t y1, uint16_t color)
{
  uint16_t x;
  uint16_t y;

  for (y = y0; y < y1; y++)
    {
      FAR uint16_t *line = (FAR uint16_t *)(fb->mem +
                                            (size_t)y * fb->stride);

      for (x = 0; x < fb->xres; x++)
        {
          line[x] = color;
        }
    }
}

static FAR const uint8_t *set_blob(FAR const struct aodface_hdr_s *face,
                                   uint8_t set)
{
  FAR const uint8_t *base = (FAR const uint8_t *)face + AODFACE_BLOB_OFF;

  return set ? base + set_bytes(face->s0_w, face->s0_h, face->s0_n) : base;
}

static void blit_glyph(FAR const struct aodface_hdr_s *face,
                       FAR const struct aodface_fb_s *fb,
                       FAR const struct aodface_item_s *it, uint8_t idx)
{
  uint8_t w = it->set ? face->s1_w : face->s0_w;
  uint8_t h = it->set ? face->s1_h : face->s0_h;
  uint32_t stride = set_stride(w);
  FAR const uint8_t *m = set_blob(face, it->set) + idx * stride * h;
  uint16_t color = rgb222_to_565(it->fg);
  uint32_t row;
  uint32_t col;

  for (row = 0; row < h; row++)
    {
      FAR const uint8_t *bits = m + row * stride;
      FAR uint16_t *line = (FAR uint16_t *)
        (fb->mem + (size_t)(fb->oy + it->y + row) * fb->stride) +
        fb->ox + it->x;

      for (col = 0; col < w; col++)
        {
          /* MSB first within each byte.  Clear bits are left alone: the
           * band fill already put the background there.
           */

          if (bits[col >> 3] & (0x80 >> (col & 7)))
            {
              line[col] = color;
            }
        }
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: aodface_validate
 ****************************************************************************/

int aodface_validate(FAR const void *rec, size_t len)
{
  FAR const struct aodface_hdr_s *face = rec;
  uint32_t need;
  int i;

  /* Nothing past a field is read until that field has been checked */

  if (len < AODFACE_BLOB_OFF)
    {
      return AODFACE_E_LEN;
    }

  if (face->magic != AODFACE_MAGIC)
    {
      return AODFACE_E_MAGIC;
    }

  if (face->ver != AODFACE_VER)
    {
      return AODFACE_E_VER;
    }

  if (face->total_len != len || len > AODFACE_MAX_LEN)
    {
      return AODFACE_E_LEN;
    }

  if (face->s0_w == 0 || face->s0_h == 0 || face->s0_n == 0 ||
      face->s0_w > AODFACE_SIZE || face->s0_h > AODFACE_SIZE ||
      face->s1_w > AODFACE_SIZE || face->s1_h > AODFACE_SIZE ||
      face->item_count > AODFACE_MAX_ITEMS ||
      face->band_y0 >= face->band_y1 || face->band_y1 > AODFACE_SIZE)
    {
      return AODFACE_E_GEOM;
    }

  need = AODFACE_BLOB_OFF + set_bytes(face->s0_w, face->s0_h, face->s0_n) +
         (face->s1_w ? set_bytes(face->s1_w, face->s1_h, face->s1_n) : 0);
  if (need != len)
    {
      return AODFACE_E_LEN;
    }

  for (i = 0; i < face->item_count; i++)
    {
      FAR const struct aodface_item_s *it = &face->items[i];
      uint8_t w;
      uint8_t h;
      uint8_t n;

      if (it->kind > AODFACE_K_MM1 || it->set > 1 ||
          (it->set == 1 && face->s1_w == 0))
        {
          return AODFACE_E_ITEM;
        }

      w = it->set ? face->s1_w : face->s0_w;
      h = it->set ? face->s1_h : face->s0_h;
      n = it->set ? face->s1_n : face->s0_n;

      if (it->kind == AODFACE_K_LITERAL && it->literal >= n)
        {
          return AODFACE_E_ITEM;
        }

      /* A digit index reaches 9 (and hours' tens 2), so a set that is
       * drawn from by a digit item must hold all ten.
       */

      if (it->kind != AODFACE_K_LITERAL && n < 10)
        {
          return AODFACE_E_ITEM;
        }

      /* Every glyph must fit the face and the declared band.  The band is
       * all that is repainted each minute, so a glyph outside it would be
       * drawn into the buffer and never, or only half, reach the panel.
       */

      if ((uint32_t)it->x + w > AODFACE_SIZE || it->y < face->band_y0 ||
          (uint32_t)it->y + h > face->band_y1)
        {
          return AODFACE_E_ITEM;
        }
    }

  if (aodface_crc32(rec, len) != face->crc32)
    {
      return AODFACE_E_CRC;
    }

  return AODFACE_E_OK;
}

/****************************************************************************
 * Name: aodface_builtin
 ****************************************************************************/

FAR const struct aodface_hdr_s *aodface_builtin(void)
{
#ifdef CONFIG_DA1470X_LCDC_AOD_BUILTIN_FACE
  /* Validated like an uploaded face: skipping that would prove the
   * renderer and nothing else.
   */

  if (aodface_validate(g_aodface_sample, sizeof(g_aodface_sample)) ==
      AODFACE_E_OK)
    {
      return (FAR const struct aodface_hdr_s *)g_aodface_sample;
    }
#endif

  return NULL;
}

/****************************************************************************
 * Name: aodface_band
 ****************************************************************************/

void aodface_band(FAR const struct aodface_hdr_s *face,
                  FAR const struct aodface_fb_s *fb,
                  FAR uint16_t *y0, FAR uint16_t *y1)
{
  *y0 = fb->oy + face->band_y0;
  *y1 = fb->oy + face->band_y1;
}

/****************************************************************************
 * Name: aodface_clear_all
 ****************************************************************************/

void aodface_clear_all(FAR const struct aodface_hdr_s *face,
                       FAR const struct aodface_fb_s *fb)
{
  fill_rows(fb, 0, fb->yres, rgb222_to_565(face->bg));
}

/****************************************************************************
 * Name: aodface_render
 ****************************************************************************/

void aodface_render(FAR const struct aodface_hdr_s *face,
                    FAR const struct aodface_fb_s *fb,
                    uint8_t hh, uint8_t mm)
{
  uint16_t y0;
  uint16_t y1;
  int i;

  aodface_band(face, fb, &y0, &y1);
  fill_rows(fb, y0, y1, rgb222_to_565(face->bg));

  for (i = 0; i < face->item_count; i++)
    {
      FAR const struct aodface_item_s *it = &face->items[i];
      uint8_t idx;

      switch (it->kind)
        {
          case AODFACE_K_LITERAL:
            idx = it->literal;
            break;

          case AODFACE_K_HH10:
            idx = hh / 10;
            break;

          case AODFACE_K_HH1:
            idx = hh % 10;
            break;

          case AODFACE_K_MM10:
            idx = mm / 10;
            break;

          case AODFACE_K_MM1:
            idx = mm % 10;
            break;

          default:
            continue;
        }

      if ((it->flags & AODFACE_F_BLANK_LEADING_ZERO) != 0 &&
          it->kind == AODFACE_K_HH10 && idx == 0)
        {
          continue;
        }

      blit_glyph(face, fb, it, idx);
    }
}
