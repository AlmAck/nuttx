/****************************************************************************
 * arch/arm/src/da1470x/da1470x_psram.c
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

/* A quad-SPI PSRAM on the second QSPI controller.
 *
 * The part is an AP Memory APS6404: 64 Mbit of DRAM behind a serial
 * interface, with its own refresh.  Once the controller is in its
 * automatic mode the memory is mapped at DA1470X_PSRAM_BASE and ordinary
 * loads and stores reach it -- the controller turns each one into the
 * command, address and data phases on the bus.
 *
 * Two things about it are easy to get wrong and hard to see when they
 * are.
 *
 * It refreshes itself only while its chip select is high.  A burst that
 * holds it low for longer than the part allows starves the refresh and the
 * memory silently loses data; nothing reports it.  The controller can
 * break long bursts itself (the tCEM limit below), and the limit is
 * counted in bus clocks, so it has to hold at the slowest clock this
 * system runs at, not only at the one in use when it was set.
 *
 * And the window is only safe to touch once the whole configuration is in
 * place: with the controller out of its automatic mode an access is
 * ignored, and a write with RAM mode off is a hard fault.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/mm/mm.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_qspic.h"
#include "hardware/da1470x_dcache.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"
#include "da1470x_psram.h"

#ifdef CONFIG_DA1470X_PSRAM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* APS6404 commands */

#define PSRAM_CMD_RESET_ENABLE  0x66
#define PSRAM_CMD_RESET         0x99
#define PSRAM_CMD_READ_ID       0x9f  /* SPI mode only */
#define PSRAM_CMD_ENTER_QUAD    0x35
#define PSRAM_CMD_QUAD_READ     0xeb
#define PSRAM_CMD_QUAD_WRITE    0x38

/* What Read ID answers with on a good part */

#define PSRAM_MFID_APMEMORY     0x0d
#define PSRAM_KGD_PASS          0x5d

/* Bus phase widths, as the controller encodes them */

#define QSPI_BUS_SINGLE         0
#define QSPI_BUS_QUAD           2

/* The controller clock is the system clock divided by 2^div.  Divide by
 * two: that is 80 MHz with the PLL at 160 MHz, which is what the vendor's
 * own reference design runs this part at, and comfortably inside what it
 * is rated for.  Running it at the undivided 160 MHz would not be.
 */

#define PSRAM_CLK_DIV           1

/* The longest the chip select may stay low in one burst, and the slowest
 * system clock the controller will see while in use.  Power management
 * drops to the 32 MHz crystal when idle, and a limit worked out at the
 * PLL's 160 MHz would be five times too long there -- ten microseconds
 * against a two microsecond limit.  Sizing it for the slowest clock keeps
 * it honest at every clock, at the cost of breaking bursts more often
 * than strictly needed while at full speed.
 */

#define PSRAM_TCEM_US           2
#define PSRAM_SLOWEST_SYSCLK    32000000

#define PSRAM_TCEM_CYCLES \
  (PSRAM_TCEM_US * ((PSRAM_SLOWEST_SYSCLK >> PSRAM_CLK_DIV) / 1000000))

/* Minimum chip select high time between accesses: 18 ns, which at the
 * fastest clock (80 MHz, 12.5 ns) is two cycles.  At slower clocks two
 * cycles is longer, which is on the safe side.
 */

#define PSRAM_CS_HIGH_CYCLES    2

/* Fast quad read wants six wait clocks.  The controller provides them as
 * one extra byte (two clocks in quad) plus two dummy bytes (four clocks).
 */

#define PSRAM_DUMMY_BYTES       2

/* Where in the bit period the controller samples read data.  It depends
 * on how fast the logic is running, which is why it follows the system
 * clock: the vendor's values are 7 above the 32 MHz crystal, with the
 * core at 1.2 V, and 2 at the crystal.  A wrong one does not fail loudly;
 * it samples a neighbouring bit, and the reads come back subtly wrong.
 */

#define PSRAM_RPIPE_FAST        7
#define PSRAM_RPIPE_SLOW        2

/****************************************************************************
 * Private Data
 ****************************************************************************/

static bool g_psram_ready;

#ifdef CONFIG_DA1470X_PSRAM_HEAP
static struct mm_heap_s *g_psram_heap;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: psram_cs / psram_tx / psram_rx
 *
 * Description:
 *   Manual mode transfers.  The width of the access to the data registers
 *   is what decides how many bits go out on the bus, so a command byte has
 *   to be written as a byte.
 *
 ****************************************************************************/

static inline void psram_cs(bool active)
{
  putreg32(active ? QSPIC_CTRLBUS_EN_CS : QSPIC_CTRLBUS_DIS_CS,
           DA1470X_QSPIC2_CTRLBUS);
}

static inline void psram_tx(uint8_t byte)
{
  putreg8(byte, DA1470X_QSPIC2_WRITEDATA);
}

static inline uint8_t psram_rx(void)
{
  return getreg8(DA1470X_QSPIC2_READDATA);
}

static void psram_command(uint8_t cmd)
{
  psram_cs(true);
  psram_tx(cmd);
  psram_cs(false);
}

/****************************************************************************
 * Name: psram_read_id
 ****************************************************************************/

static void psram_read_id(uint8_t *mfid, uint8_t *kgd)
{
  psram_cs(true);
  psram_tx(PSRAM_CMD_READ_ID);

  /* The command is followed by an address the part ignores */

  psram_tx(0);
  psram_tx(0);
  psram_tx(0);

  *mfid = psram_rx();
  *kgd  = psram_rx();
  psram_cs(false);
}

/****************************************************************************
 * Name: psram_test
 *
 * Description:
 *   Walk a one across the address lines and a one across the data lines.
 *   A full march test of eight megabytes would take long enough to notice
 *   at boot; this takes microseconds and finds what actually goes wrong on
 *   a new board -- a data line shorted to another or stuck, an address
 *   line that does not reach the part so that two halves of the memory
 *   alias.  The data lines matter here in particular: two of them are
 *   shared with pins the board could also give to SPI0.
 *
 ****************************************************************************/

static int psram_test_lines(void);

static int psram_test(void)
{
  uint32_t ctrl = getreg32(DA1470X_DCACHE_CTRL);
  int ret;

  /* The PSRAM cache sits in front of this controller.  It is off out of
   * reset and nothing here turns it on, but if something ever does, a
   * write followed by a read of the same word is served by the cache and
   * never reaches the memory -- and a test that passes with no memory
   * fitted is worse than no test.  Route around it for the duration, so
   * the result is about the part whatever the cache is doing.
   */

  putreg32(ctrl | DCACHE_CTRL_BYPASS, DA1470X_DCACHE_CTRL);
  ret = psram_test_lines();
  putreg32(ctrl, DA1470X_DCACHE_CTRL);

  return ret;
}

static int psram_test_lines(void)
{
  volatile uint32_t *mem = (volatile uint32_t *)DA1470X_PSRAM_BASE;
  size_t words = DA1470X_PSRAM_SIZE / sizeof(uint32_t);
  size_t off;
  uint32_t bit;

  /* Data lines: every bit on its own, at one address */

  for (bit = 1; bit != 0; bit <<= 1)
    {
      mem[0] = bit;
      if (mem[0] != bit)
        {
          syslog(LOG_ERR, "PSRAM: data line: wrote %08" PRIx32
                 ", read %08" PRIx32 "\n", bit, mem[0]);
          return -EIO;
        }
    }

  /* Address lines: tag every power of two offset with its own value, then
   * check that writing one did not land on another.
   */

  mem[0] = 0xaaaaaaaa;
  for (off = 1; off < words; off <<= 1)
    {
      mem[off] = 0xaaaaaaaa;
    }

  mem[0] = 0x55555555;
  for (off = 1; off < words; off <<= 1)
    {
      if (mem[off] != 0xaaaaaaaa)
        {
          syslog(LOG_ERR,
                 "PSRAM: address line: offset %zu aliases offset 0\n",
                 off * sizeof(uint32_t));
          return -EIO;
        }
    }

  for (off = 1; off < words; off <<= 1)
    {
      mem[off] = 0x55555555 ^ (uint32_t)off;
    }

  for (off = 1; off < words; off <<= 1)
    {
      if (mem[off] != (0x55555555 ^ (uint32_t)off))
        {
          syslog(LOG_ERR,
                 "PSRAM: address line: offset %zu reads back wrong\n",
                 off * sizeof(uint32_t));
          return -EIO;
        }
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_psram_initialize
 ****************************************************************************/

int da1470x_psram_initialize(void)
{
  irqstate_t flags;
  uint8_t mfid;
  uint8_t kgd;
  int ret;

  /* Power first.  The controller and its memory window are in PD_CTRL,
   * a domain of their own -- not PD_SYS with the flash controller and the
   * display -- and with it off every access is dropped without a fault:
   * registers read as zero, writes vanish, and the part looks absent.
   */

  ret = da1470x_pd_enable(DA1470X_PD_CTRL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "PSRAM: PD_CTRL did not come up: %d\n", ret);
      return ret;
    }

  /* Clock the controller, divided as above */

  modifyreg32(DA1470X_CRG_TOP_CLK_AMBA, CRG_TOP_CLK_AMBA_QSPIC2_DIV_MASK,
              CRG_TOP_CLK_AMBA_QSPIC2_DIV(PSRAM_CLK_DIV) |
              CRG_TOP_CLK_AMBA_QSPIC2_ENABLE);

  flags = enter_critical_section();

  /* Manual mode, single SPI, clock mode 0.  IO2 and IO3 are driven high,
   * which is what they must be while the bus is only one bit wide.
   */

  putreg32(QSPIC_CTRLMODE_IO2_OEN | QSPIC_CTRLMODE_IO2_DAT |
           QSPIC_CTRLMODE_IO3_OEN | QSPIC_CTRLMODE_IO3_DAT |
           QSPIC_CTRLMODE_RPIPE_EN |
           QSPIC_CTRLMODE_PCLK_MD(da1470x_get_sysclk_src() ==
                                  DA1470X_SYSCLK_XTAL32M ?
                                  PSRAM_RPIPE_SLOW : PSRAM_RPIPE_FAST),
           DA1470X_QSPIC2_CTRLMODE);
  putreg32(QSPIC_CTRLBUS_SET_SINGLE, DA1470X_QSPIC2_CTRLBUS);

  /* Reset, in case it was left in quad mode by anything before us, then
   * give it the time it asks for to come out of reset.
   */

  psram_command(PSRAM_CMD_RESET_ENABLE);
  psram_command(PSRAM_CMD_RESET);
  leave_critical_section(flags);

  up_udelay(100);

  /* Is it there?  Read ID only works in SPI mode, which is why this comes
   * before the switch to quad rather than after it.
   */

  flags = enter_critical_section();
  psram_read_id(&mfid, &kgd);
  leave_critical_section(flags);

  if (mfid != PSRAM_MFID_APMEMORY || kgd != PSRAM_KGD_PASS)
    {
      syslog(LOG_ERR,
             "PSRAM: none found, read id %02x %02x, expected %02x %02x\n",
             mfid, kgd, PSRAM_MFID_APMEMORY, PSRAM_KGD_PASS);
      modifyreg32(DA1470X_CRG_TOP_CLK_AMBA,
                  CRG_TOP_CLK_AMBA_QSPIC2_ENABLE, 0);
      da1470x_pd_disable(DA1470X_PD_CTRL);
      return -ENODEV;
    }

  flags = enter_critical_section();

  /* Into quad mode: the part first, then the controller.  IO2 and IO3
   * become data lines, so they are released to the controller.
   */

  psram_command(PSRAM_CMD_ENTER_QUAD);
  putreg32(QSPIC_CTRLBUS_SET_QUAD, DA1470X_QSPIC2_CTRLBUS);
  modifyreg32(DA1470X_QSPIC2_CTRLMODE,
              QSPIC_CTRLMODE_IO2_OEN | QSPIC_CTRLMODE_IO3_OEN, 0);

  /* How the controller reads in its automatic mode: fast quad read with
   * every phase four bits wide, the command sent on every access.  The
   * extra byte is written after this, because writing BURSTCMDA here
   * would clear it.
   */

  putreg32(QSPIC_BURSTCMDA_INST(PSRAM_CMD_QUAD_READ) |
           QSPIC_BURSTCMDA_INST_TX_MD(QSPI_BUS_QUAD) |
           QSPIC_BURSTCMDA_ADR_TX_MD(QSPI_BUS_QUAD) |
           QSPIC_BURSTCMDA_DMY_TX_MD(QSPI_BUS_QUAD),
           DA1470X_QSPIC2_BURSTCMDA);

  putreg32(QSPIC_BURSTCMDB_DAT_RX_MD(QSPI_BUS_QUAD) |
           QSPIC_BURSTCMDB_DMY_NUM(PSRAM_DUMMY_BYTES) |
           QSPIC_BURSTCMDB_CS_HIGH_MIN(PSRAM_CS_HIGH_CYCLES),
           DA1470X_QSPIC2_BURSTCMDB);

  /* The extra byte: two of the six wait clocks */

  modifyreg32(DA1470X_QSPIC2_BURSTCMDA,
              QSPIC_BURSTCMDA_EXT_BYTE_MASK | QSPIC_BURSTCMDA_EXT_TX_MD_MASK,
              QSPIC_BURSTCMDA_EXT_BYTE(0x00) |
              QSPIC_BURSTCMDA_EXT_TX_MD(QSPI_BUS_QUAD));
  modifyreg32(DA1470X_QSPIC2_BURSTCMDB, QSPIC_BURSTCMDB_EXT_HF_DS,
              QSPIC_BURSTCMDB_EXT_BYTE_EN);

  /* How it writes.  Only a RAM gets this: a flash is written in manual
   * mode and has no automatic write instruction.
   */

  putreg32(QSPIC_AWRITECMD_WR_INST(PSRAM_CMD_QUAD_WRITE) |
           QSPIC_AWRITECMD_WR_INST_TX_MD(QSPI_BUS_QUAD) |
           QSPIC_AWRITECMD_WR_ADR_TX_MD(QSPI_BUS_QUAD) |
           QSPIC_AWRITECMD_WR_DAT_TX_MD(QSPI_BUS_QUAD) |
           QSPIC_AWRITECMD_WR_CS_HIGH_MIN(PSRAM_CS_HIGH_CYCLES),
           DA1470X_QSPIC2_AWRITECMD);

  /* The refresh limit, and a burst length of zero: this part does not
   * need bursts aligned to any boundary of its own.
   */

  putreg32(QSPIC_MEMBLEN_MEMBLEN(0) | QSPIC_MEMBLEN_T_CEM_EN |
           QSPIC_MEMBLEN_T_CEM_CC(PSRAM_TCEM_CYCLES),
           DA1470X_QSPIC2_MEMBLEN);

  /* RAM mode, 24-bit addresses, then automatic mode.  RAM mode has to be
   * on before anything writes the window: with it off a write is a hard
   * fault, because the controller takes the device for a flash.
   */

  modifyreg32(DA1470X_QSPIC2_CTRLMODE,
              QSPIC_CTRLMODE_USE_32BA | QSPIC_CTRLMODE_CS_MD,
              QSPIC_CTRLMODE_SRAM_EN);
  modifyreg32(DA1470X_QSPIC2_CTRLMODE, 0, QSPIC_CTRLMODE_AUTO_MD);

  leave_critical_section(flags);

  ret = psram_test();
  if (ret < 0)
    {
      syslog(LOG_ERR, "PSRAM: failed its line test, not using it\n");
      return ret;
    }

  g_psram_ready = true;

#ifdef CONFIG_DA1470X_PSRAM_HEAP
  g_psram_heap = mm_initialize("psram", (void *)DA1470X_PSRAM_BASE,
                               DA1470X_PSRAM_SIZE);
  if (g_psram_heap == NULL)
    {
      return -ENOMEM;
    }
#endif

  syslog(LOG_INFO, "PSRAM: %u KiB at %08x, id %02x %02x\n",
        (unsigned)(DA1470X_PSRAM_SIZE / 1024),
        (unsigned)DA1470X_PSRAM_BASE, mfid, kgd);
  return OK;
}

/****************************************************************************
 * Name: da1470x_psram_clock_changed
 *
 * Description:
 *   Move the read sampling point to suit the system clock.  Called by the
 *   power management code around every clock switch: after lowering the
 *   clock and before raising it, so the controller is never running fast
 *   with the slow setting.
 *
 ****************************************************************************/

void da1470x_psram_clock_changed(bool fast)
{
  if (!g_psram_ready)
    {
      return;
    }

  modifyreg32(DA1470X_QSPIC2_CTRLMODE, QSPIC_CTRLMODE_PCLK_MD_MASK,
              QSPIC_CTRLMODE_PCLK_MD(fast ? PSRAM_RPIPE_FAST :
                                            PSRAM_RPIPE_SLOW));
}

#ifdef CONFIG_DA1470X_PSRAM_HEAP
/****************************************************************************
 * Name: da1470x_psram_malloc / da1470x_psram_free
 ****************************************************************************/

void *da1470x_psram_malloc(size_t size)
{
  return g_psram_heap != NULL ? mm_malloc(g_psram_heap, size) : NULL;
}

void da1470x_psram_free(void *mem)
{
  if (g_psram_heap != NULL)
    {
      mm_free(g_psram_heap, mem);
    }
}
#endif

#endif /* CONFIG_DA1470X_PSRAM */
