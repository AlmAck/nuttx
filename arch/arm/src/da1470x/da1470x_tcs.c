/****************************************************************************
 * arch/arm/src/da1470x/da1470x_tcs.c
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

/* Every die leaves the factory with a configuration script burnt into its
 * one-time-programmable memory.  The script is a flat list of words.  The
 * boot ROM walks it and applies the plain register address and value pairs
 * it finds; the entries tagged for the firmware it leaves alone, and those
 * are what this file collects.
 *
 * An entry tagged for the firmware is a header word followed by its
 * payload.  The header carries a group identifier, the payload length in
 * words and a set identifier; only set zero is used.  A group either holds
 * register address and value pairs, which are written as they are, or a
 * list of trim values whose meaning depends on the group.
 *
 * The values matter well beyond convenience.  The radio and synthesiser
 * groups hold the per-die calibration of the transmitter and the receiver:
 * without them the controller runs on reset defaults, which costs tens of
 * decibels of link budget.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/param.h>

#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include "arm_internal.h"
#include "da1470x_otp.h"
#include "da1470x_tcs.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Word offset of the configuration script in the OTP array, and how far
 * beyond it the script may reach.
 */

#define CS_OTP_WORD            (0x00000c00 / 4)
#define CS_MAX_WORDS           256

/* The start marker is expected at the first word of the script, but some
 * parts carry it one word further in, so a few words are searched.
 */

#define CS_START_SEARCH        4

/* Markers found in the script */

#define CS_START_CMD           0xa5a5a5a5  /* Script starts here */
#define CS_STOP_CMD            0x00000000  /* Script ends here */
#define CS_EMPTY_VAL           0xffffffff  /* Never programmed */
#define CS_BOOTER_VAL          0xe6000000  /* Value for the boot ROM */
#define CS_MIN_FW_VAL          0xeb000000  /* Minimum firmware version */
#define CS_SDK_VAL             0xe9000000  /* Entry for the firmware */

#define CS_SDK_VAL_MASK        0xff000000
#define CS_SDK_GID(v)          ((v) & 0xff)
#define CS_SDK_LEN(v)          (((v) >> 8) & 0xff)
#define CS_SDK_SETID(v)        (((v) >> 16) & 0xff)

/* A word this small or smaller is a register address, and the word after
 * it is the value the boot ROM writes there.
 */

#define CS_MAX_REG_ADDR        0x5100155c

/* Registers the firmware is allowed to write when it applies a group.
 * The radio blocks sit at the bottom of the range and the system
 * peripherals at the top.
 */

#define CS_MIN_APPLY_ADDR      0x40000000
#define CS_MAX_APPLY_ADDR      CS_MAX_REG_ADDR

/* Size of the value array.  The script cannot be longer than the window
 * that is searched, so this is an upper bound with room to spare.
 */

#ifndef CONFIG_DA1470X_TCS_DATA_WORDS
#  define CONFIG_DA1470X_TCS_DATA_WORDS 256
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* One attribute word per group, indexed by group identifier, and the value
 * array the attributes point into.  The Bluetooth controller is handed
 * both, so their layout is fixed by its firmware.
 */

static uint16_t g_tcs_attr[DA1470X_TCS_GROUP_MAX];
static uint32_t g_tcs_data[CONFIG_DA1470X_TCS_DATA_WORDS];
static uint16_t g_tcs_used;

/* Group identifiers whose payload is register address and value pairs */

static const uint8_t g_tcs_reg_pair[] =
{
  DA1470X_TCS_PD_SYS, DA1470X_TCS_PD_SNC, DA1470X_TCS_PD_MEM,
  DA1470X_TCS_PD_TMR, DA1470X_TCS_PD_AUDIO, DA1470X_TCS_PD_RAD,
  DA1470X_TCS_PD_SYNTH, DA1470X_TCS_PD_GPU, DA1470X_TCS_PD_CTRL,
  DA1470X_TCS_SD_ADC_SINGLE, DA1470X_TCS_SD_ADC_DIFF,
  DA1470X_TCS_PD_RAD_MODE1, DA1470X_TCS_PD_SYNTH_MODE1,
  DA1470X_TCS_PD_RAD_MODE2, DA1470X_TCS_PD_SYNTH_MODE2
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tcs_type / tcs_start / tcs_size
 *
 * Description:
 *   Read back the fields of one attribute entry.
 *
 ****************************************************************************/

static inline uint8_t tcs_type(uint8_t gid)
{
  return (g_tcs_attr[gid] & TCS_ATTR_TYPE_MASK) >> TCS_ATTR_TYPE_SHIFT;
}

static inline uint16_t tcs_start(uint8_t gid)
{
  return (g_tcs_attr[gid] & TCS_ATTR_START_MASK) >> TCS_ATTR_START_SHIFT;
}

static inline uint8_t tcs_size(uint8_t gid)
{
  return (g_tcs_attr[gid] & TCS_ATTR_SIZE_MASK) >> TCS_ATTR_SIZE_SHIFT;
}

/****************************************************************************
 * Name: tcs_set_start / tcs_set_size
 ****************************************************************************/

static inline void tcs_set_start(uint8_t gid, uint16_t start)
{
  g_tcs_attr[gid] &= ~TCS_ATTR_START_MASK;
  g_tcs_attr[gid] |= (start << TCS_ATTR_START_SHIFT) & TCS_ATTR_START_MASK;
}

static inline void tcs_set_size(uint8_t gid, uint8_t size)
{
  g_tcs_attr[gid] &= ~TCS_ATTR_SIZE_MASK;
  g_tcs_attr[gid] |= (size << TCS_ATTR_SIZE_SHIFT) & TCS_ATTR_SIZE_MASK;
}

/****************************************************************************
 * Name: tcs_reset
 *
 * Description:
 *   Mark every group absent and tag the groups that hold register pairs.
 *
 ****************************************************************************/

static void tcs_reset(void)
{
  unsigned int i;

  memset(g_tcs_attr, 0, sizeof(g_tcs_attr));
  memset(g_tcs_data, 0, sizeof(g_tcs_data));
  g_tcs_used = 0;

  for (i = 0; i < DA1470X_TCS_GROUP_MAX; i++)
    {
      tcs_set_start(i, TCS_ATTR_START_EMPTY);
    }

  for (i = 0; i < nitems(g_tcs_reg_pair); i++)
    {
      g_tcs_attr[g_tcs_reg_pair[i]] |= TCS_ATTR_TYPE_REG_PAIR <<
                                       TCS_ATTR_TYPE_SHIFT;
    }
}

/****************************************************************************
 * Name: tcs_find_start
 *
 * Description:
 *   Locate the start marker of the configuration script.  The caller must
 *   hold the OTP controller.
 *
 * Returned Value:
 *   The word index of the marker, or zero when there is no script.
 *
 ****************************************************************************/

static unsigned int tcs_find_start(void)
{
  unsigned int i;

  for (i = 0; i < CS_START_SEARCH; i++)
    {
      if (da1470x_otp_read32(CS_OTP_WORD + i) == CS_START_CMD)
        {
          return CS_OTP_WORD + i;
        }
    }

  return 0;
}

/****************************************************************************
 * Name: tcs_measure
 *
 * Description:
 *   First pass over the script.  Work out how many words each group needs,
 *   which is the sum of its entries for a register pair group and the
 *   length of its last entry for a trim group.
 *
 * Input Parameters:
 *   first - Word index of the start marker.
 *
 * Returned Value:
 *   The total number of words the groups need.
 *
 ****************************************************************************/

static uint16_t tcs_measure(unsigned int first)
{
  unsigned int index = first + 1;
  unsigned int last  = first + CS_MAX_WORDS;
  uint16_t total = 0;

  while (index < last)
    {
      uint32_t value = da1470x_otp_read32(index);
      uint8_t gid;
      uint8_t len;

      if (value == CS_STOP_CMD || value == CS_EMPTY_VAL)
        {
          break;
        }

      if (value == CS_BOOTER_VAL || value == CS_MIN_FW_VAL ||
          value <= CS_MAX_REG_ADDR)
        {
          /* A value for the boot ROM, or a register address: in both
           * cases the word after it belongs to the entry.
           */

          index += 2;
          continue;
        }

      if ((value & CS_SDK_VAL_MASK) != CS_SDK_VAL)
        {
          index++;
          continue;
        }

      gid = CS_SDK_GID(value);
      len = CS_SDK_LEN(value);

      if (gid >= DA1470X_TCS_GROUP_MAX || CS_SDK_SETID(value) != 0)
        {
          index += len + 1;
          continue;
        }

      if (len > (TCS_ATTR_SIZE_MASK >> TCS_ATTR_SIZE_SHIFT))
        {
          /* Too long for the size field: the group is left out */
        }
      else if (tcs_type(gid) == TCS_ATTR_TYPE_TRIM)
        {
          /* Several entries of the same trim group may appear with
           * different lengths.  Only the last one counts.
           */

          if (tcs_size(gid) != len)
            {
              total -= tcs_size(gid);
              total += len;
              tcs_set_size(gid, len);
            }
        }
      else if ((len & 1) != 0)
        {
          /* A register pair group must have an even payload */
        }
      else if (tcs_size(gid) + len <= (TCS_ATTR_SIZE_MASK >>
                                       TCS_ATTR_SIZE_SHIFT))
        {
          total += len;
          tcs_set_size(gid, tcs_size(gid) + len);
        }

      index += len + 1;
    }

  return total;
}

/****************************************************************************
 * Name: tcs_store
 *
 * Description:
 *   Copy the payload of one entry into the slot reserved for its group.
 *   A register pair group may be spread over several entries, so its free
 *   slot is searched; a trim group is written from its start.
 *
 * Input Parameters:
 *   header - Word index of the entry header.
 *   gid    - Group identifier.
 *   len    - Payload length in words.
 *
 ****************************************************************************/

static void tcs_store(unsigned int header, uint8_t gid, uint8_t len)
{
  uint16_t index = tcs_start(gid);
  uint16_t limit;
  uint8_t i;

  if (index == TCS_ATTR_START_EMPTY)
    {
      return;
    }

  limit = index + tcs_size(gid);

  if (tcs_type(gid) == TCS_ATTR_TYPE_TRIM)
    {
      if (len != tcs_size(gid))
        {
          return;
        }
    }
  else
    {
      while (index < limit && g_tcs_data[index] != 0)
        {
          index += 2;
        }
    }

  if (index + len > limit)
    {
      return;
    }

  for (i = 0; i < len; i++)
    {
      g_tcs_data[index + i] = da1470x_otp_read32(header + 1 + i);
    }
}

/****************************************************************************
 * Name: tcs_collect
 *
 * Description:
 *   Second pass over the script, copying the payload of every entry whose
 *   group was given room by the first pass.
 *
 ****************************************************************************/

static void tcs_collect(unsigned int first)
{
  unsigned int index = first + 1;
  unsigned int last  = first + CS_MAX_WORDS;

  while (index < last)
    {
      uint32_t value = da1470x_otp_read32(index);
      uint8_t len;

      if (value == CS_STOP_CMD || value == CS_EMPTY_VAL)
        {
          break;
        }

      if (value == CS_BOOTER_VAL || value == CS_MIN_FW_VAL ||
          value <= CS_MAX_REG_ADDR)
        {
          index += 2;
          continue;
        }

      if ((value & CS_SDK_VAL_MASK) != CS_SDK_VAL)
        {
          index++;
          continue;
        }

      len = CS_SDK_LEN(value);

      if (CS_SDK_GID(value) < DA1470X_TCS_GROUP_MAX &&
          CS_SDK_SETID(value) == 0)
        {
          tcs_store(index, CS_SDK_GID(value), len);
        }

      index += len + 1;
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_tcs_initialize
 ****************************************************************************/

void da1470x_tcs_initialize(void)
{
  unsigned int first;
  uint16_t offset;
  uint16_t total;
  unsigned int gid;
  int ret;

  tcs_reset();

  ret = da1470x_otp_acquire();
  if (ret < 0)
    {
      /* No trim values.  Nothing can be reported from here, the console
       * does not exist yet; the Bluetooth controller logs the count of
       * values it was given, which is then zero.
       */

      return;
    }

  first = tcs_find_start();
  if (first == 0)
    {
      da1470x_otp_release();
      return;
    }

  total = tcs_measure(first);
  if (total == 0)
    {
      da1470x_otp_release();
      return;
    }

  /* Turn the per group sizes into offsets into the value array.  A group
   * that does not fit is left marked absent.
   */

  offset = 0;
  for (gid = 0; gid < DA1470X_TCS_GROUP_MAX; gid++)
    {
      uint8_t size = tcs_size(gid);

      if (size == 0)
        {
          continue;
        }

      if (offset + size > CONFIG_DA1470X_TCS_DATA_WORDS)
        {
          tcs_set_size(gid, 0);
          continue;
        }

      tcs_set_start(gid, offset);
      offset += size;
    }

  g_tcs_used = offset;

  tcs_collect(first);
  da1470x_otp_release();
}

/****************************************************************************
 * Name: da1470x_tcs_apply
 ****************************************************************************/

int da1470x_tcs_apply(uint8_t gid)
{
  const uint32_t *pair;
  uint16_t index;
  uint8_t size;
  int written = 0;

  if (gid >= DA1470X_TCS_GROUP_MAX)
    {
      return -EINVAL;
    }

  if (tcs_type(gid) != TCS_ATTR_TYPE_REG_PAIR)
    {
      return -EINVAL;
    }

  index = tcs_start(gid);
  size  = tcs_size(gid);

  if (index == TCS_ATTR_START_EMPTY || size == 0)
    {
      return 0;
    }

  for (pair = &g_tcs_data[index]; size >= 2; size -= 2, pair += 2)
    {
      if (pair[0] < CS_MIN_APPLY_ADDR || pair[0] > CS_MAX_APPLY_ADDR)
        {
          _warn("WARNING: TCS group 0x%02x: address 0x%08" PRIx32
                " out of range\n", gid, pair[0]);
          continue;
        }

      putreg32(pair[1], pair[0]);
      written++;
    }

  return written;
}

/****************************************************************************
 * Name: da1470x_tcs_values
 ****************************************************************************/

int da1470x_tcs_values(uint8_t gid, const uint32_t **values,
                       uint8_t *count)
{
  if (gid >= DA1470X_TCS_GROUP_MAX ||
      tcs_start(gid) == TCS_ATTR_START_EMPTY || tcs_size(gid) == 0)
    {
      return -ENOENT;
    }

  *values = &g_tcs_data[tcs_start(gid)];
  *count  = tcs_size(gid);
  return OK;
}

/****************************************************************************
 * Name: da1470x_tcs_attributes
 ****************************************************************************/

const uint16_t *da1470x_tcs_attributes(size_t *count)
{
  *count = DA1470X_TCS_GROUP_MAX;
  return g_tcs_attr;
}

/****************************************************************************
 * Name: da1470x_tcs_data
 ****************************************************************************/

const uint32_t *da1470x_tcs_data(size_t *count)
{
  *count = g_tcs_used;
  return g_tcs_data;
}

/****************************************************************************
 * Name: da1470x_tcs_bdaddr
 ****************************************************************************/

int da1470x_tcs_bdaddr(uint8_t addr[6])
{
  const uint32_t *values;
  uint8_t count;
  int ret;

  ret = da1470x_tcs_values(DA1470X_TCS_BD_ADDR, &values, &count);
  if (ret < 0)
    {
      return ret;
    }

  if (count < 2)
    {
      return -ENOENT;
    }

  memcpy(addr, values, 6);
  return OK;
}
