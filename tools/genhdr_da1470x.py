#!/usr/bin/env python3
"""Generate NuttX-style DA1470x register headers from the CMSIS DA1470x-00.h.

Usage: genhdr.py <DA1470x-00.h> <outdir> BLOCK[=nuttxname[:inst0,inst1,...]] ...

Naming emitted (BLK = NuttX block name, REG = register name without _REG
and without the instance prefix, FLD = bit-field name):

  DA1470X_<BLK>_<REG>_OFFSET        register offset
  DA1470X_<BLK><n>_<REG>            absolute address per instance (or
  DA1470X_<BLK>_<REG>               for single-instance blocks)
  <BLK>_<REG>_<FLD>_SHIFT / _MASK   multi-bit fields
  <BLK>_<REG>_<FLD>                 single-bit fields, (1 << n)

Comments right of code are aligned per block so nxstyle is happy.
"""

import re
import sys
import os

LICENSE = """/****************************************************************************
 * arch/arm/src/da1470x/hardware/{fname}
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_{guard}_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_{guard}_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

"""

FOOTER = """
#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_{guard}_H */
"""

MAXCOMMENT = 40

# Hand-maintained additions per NuttX block name: (code, comment) tuples,
# emitted after the generated definitions.

SUPPLEMENT = {
    "UART": [
        ("/* IIR (read) interrupt identification ****************************/", None),
        ("", None),
        ("#define UART_IIR_IID_SHIFT (0)", "Bits 0-3: Interrupt ID"),
        ("#define UART_IIR_IID_MASK (0xf << UART_IIR_IID_SHIFT)", None),
        ("#  define UART_IIR_IID_MODEM (0 << UART_IIR_IID_SHIFT)", "Modem status"),
        ("#  define UART_IIR_IID_NONE (1 << UART_IIR_IID_SHIFT)", "No interrupt pending"),
        ("#  define UART_IIR_IID_THRE (2 << UART_IIR_IID_SHIFT)", "THR empty"),
        ("#  define UART_IIR_IID_RDA (4 << UART_IIR_IID_SHIFT)", "Received data available"),
        ("#  define UART_IIR_IID_RLS (6 << UART_IIR_IID_SHIFT)", "Receiver line status"),
        ("#  define UART_IIR_IID_BUSY (7 << UART_IIR_IID_SHIFT)", "Busy detect"),
        ("#  define UART_IIR_IID_CTI (12 << UART_IIR_IID_SHIFT)", "Character timeout"),
        ("#define UART_IIR_FIFOSE_SHIFT (6)", "Bits 6-7: FIFOs enabled"),
        ("#define UART_IIR_FIFOSE_MASK (3 << UART_IIR_FIFOSE_SHIFT)", None),
        ("", None),
        ("/* FCR (write) FIFO control ***************************************/", None),
        ("", None),
        ("#define UART_FCR_FIFOE (1 << 0)", "Bit 0: FIFO enable"),
        ("#define UART_FCR_RFIFOR (1 << 1)", "Bit 1: RX FIFO reset"),
        ("#define UART_FCR_XFIFOR (1 << 2)", "Bit 2: TX FIFO reset"),
        ("#define UART_FCR_DMAM (1 << 3)", "Bit 3: DMA mode"),
        ("#define UART_FCR_TET_SHIFT (4)", "Bits 4-5: TX empty trigger"),
        ("#define UART_FCR_TET_MASK (3 << UART_FCR_TET_SHIFT)", None),
        ("#  define UART_FCR_TET_EMPTY (0 << UART_FCR_TET_SHIFT)", "FIFO empty"),
        ("#  define UART_FCR_TET_2CHAR (1 << UART_FCR_TET_SHIFT)", "2 characters"),
        ("#  define UART_FCR_TET_QUARTER (2 << UART_FCR_TET_SHIFT)", "1/4 full"),
        ("#  define UART_FCR_TET_HALF (3 << UART_FCR_TET_SHIFT)", "1/2 full"),
        ("#define UART_FCR_RT_SHIFT (6)", "Bits 6-7: RX trigger"),
        ("#define UART_FCR_RT_MASK (3 << UART_FCR_RT_SHIFT)", None),
        ("#  define UART_FCR_RT_1CHAR (0 << UART_FCR_RT_SHIFT)", "1 character"),
        ("#  define UART_FCR_RT_QUARTER (1 << UART_FCR_RT_SHIFT)", "1/4 full"),
        ("#  define UART_FCR_RT_HALF (2 << UART_FCR_RT_SHIFT)", "1/2 full"),
        ("#  define UART_FCR_RT_2LESS (3 << UART_FCR_RT_SHIFT)", "2 less than full"),
        ("", None),
        ("/* MCR modem control (UART2/UART3 only for AFCE/RTS) ***************/", None),
        ("", None),
        ("#define UART_MCR_RTS (1 << 1)", "Bit 1: Request to send"),
        ("#define UART_MCR_AFCE (1 << 5)", "Bit 5: Auto flow control enable"),
        ("", None),
        ("/* LCR data length select values ***********************************/", None),
        ("", None),
        ("#define UART_LCR_DLS_5BITS (0 << UART_LCR_DLS_SHIFT)", None),
        ("#define UART_LCR_DLS_6BITS (1 << UART_LCR_DLS_SHIFT)", None),
        ("#define UART_LCR_DLS_7BITS (2 << UART_LCR_DLS_SHIFT)", None),
        ("#define UART_LCR_DLS_8BITS (3 << UART_LCR_DLS_SHIFT)", None),
        ("", None),
        ("#define DA1470X_UART_FIFO_DEPTH 16", "TX/RX FIFO depth"),
    ],
    "GPIO": [
        ("/* Generic per-port / per-pin accessors ****************************/", None),
        ("", None),
        ("#define DA1470X_GPIO_NPORTS 3", None),
        ("#define DA1470X_GPIO_PORT0_NPINS 32", None),
        ("#define DA1470X_GPIO_PORT1_NPINS 32", None),
        ("#define DA1470X_GPIO_PORT2_NPINS 15", None),
        ("#define DA1470X_GPIO_DATA(p) (DA1470X_GPIO_BASE + 0x0000 + ((p) << 2))", None),
        ("#define DA1470X_GPIO_SET_DATA(p) (DA1470X_GPIO_BASE + 0x000c + ((p) << 2))", None),
        ("#define DA1470X_GPIO_RESET_DATA(p) (DA1470X_GPIO_BASE + 0x0018 + ((p) << 2))", None),
        ("#define DA1470X_GPIO_MODE(p, n) (DA1470X_GPIO_BASE + 0x0024 + ((p) * 0x80) + ((n) << 2))", None),
        ("#define DA1470X_GPIO_PADPWR_CTRL(p) (DA1470X_GPIO_BASE + 0x0160 + ((p) << 2))", None),
        ("#define DA1470X_GPIO_WEAK_CTRL(p) (DA1470X_GPIO_BASE + 0x0170 + ((p) << 2))", None),
        ("", None),
        ("/* Pxy_MODE register (all pins share the same layout) ***************/", None),
        ("", None),
        ("#define GPIO_MODE_PID_SHIFT (0)", "Bits 0-5: Peripheral ID"),
        ("#define GPIO_MODE_PID_MASK (0x3f << GPIO_MODE_PID_SHIFT)", None),
        ("#define GPIO_MODE_PUPD_SHIFT (8)", "Bits 8-9: Pull / direction"),
        ("#define GPIO_MODE_PUPD_MASK (3 << GPIO_MODE_PUPD_SHIFT)", None),
        ("#  define GPIO_MODE_PUPD_INPUT (0 << GPIO_MODE_PUPD_SHIFT)", "Input, no pull"),
        ("#  define GPIO_MODE_PUPD_PULLUP (1 << GPIO_MODE_PUPD_SHIFT)", "Input, pull-up"),
        ("#  define GPIO_MODE_PUPD_PULLDOWN (2 << GPIO_MODE_PUPD_SHIFT)", "Input, pull-down"),
        ("#  define GPIO_MODE_PUPD_OUTPUT (3 << GPIO_MODE_PUPD_SHIFT)", "Output"),
        ("#define GPIO_MODE_PPOD (1 << 10)", "Bit 10: Open drain output"),
    ],
    "DMA": [
        ("/* Per-channel register block accessors *****************************/", None),
        ("", None),
        ("#define DA1470X_DMA_NCHANNELS 8", None),
        ("#define DA1470X_DMA_CHAN_BASE(n) (DA1470X_DMA_BASE + ((n) * 0x20))", None),
        ("#define DA1470X_DMA_A_START_OFFSET 0x0000", None),
        ("#define DA1470X_DMA_B_START_OFFSET 0x0004", None),
        ("#define DA1470X_DMA_INT_OFFSET 0x0008", None),
        ("#define DA1470X_DMA_LEN_OFFSET 0x000c", None),
        ("#define DA1470X_DMA_CTRL_OFFSET 0x0010", None),
        ("#define DA1470X_DMA_IDX_OFFSET 0x0014", None),
        ("", None),
        ("/* DMA_REQ_MUX: 4 bits per channel pair ****************************/", None),
        ("", None),
        ("#define DMA_REQ_MUX_SHIFT(pair) ((pair) * 4)", None),
        ("#define DMA_REQ_MUX_MASK(pair) (0xf << DMA_REQ_MUX_SHIFT(pair))", None),
        ("#  define DMA_REQ_MUX_SPI 0x0", None),
        ("#  define DMA_REQ_MUX_SPI2 0x1", None),
        ("#  define DMA_REQ_MUX_UART 0x2", None),
        ("#  define DMA_REQ_MUX_UART2 0x3", None),
        ("#  define DMA_REQ_MUX_I2C 0x4", None),
        ("#  define DMA_REQ_MUX_I2C2 0x5", None),
        ("#  define DMA_REQ_MUX_USB 0x6", None),
        ("#  define DMA_REQ_MUX_UART3 0x7", None),
        ("#  define DMA_REQ_MUX_PCM 0x8", None),
        ("#  define DMA_REQ_MUX_SRC 0x9", None),
        ("#  define DMA_REQ_MUX_SPI3 0xa", None),
        ("#  define DMA_REQ_MUX_I2C3 0xb", None),
        ("#  define DMA_REQ_MUX_GPADC 0xc", None),
        ("#  define DMA_REQ_MUX_SRC2 0xd", None),
        ("#  define DMA_REQ_MUX_I3C 0xe", None),
        ("#  define DMA_REQ_MUX_NONE 0xf", None),
    ],
    "WAKEUP": [
        ("/* Per-port accessors ***********************************************/", None),
        ("", None),
        ("#define DA1470X_WAKEUP_SELECT_P(p) (DA1470X_WAKEUP_BASE + 0x0008 + ((p) << 2))", None),
        ("#define DA1470X_WAKEUP_POL_P(p) (DA1470X_WAKEUP_BASE + 0x0014 + ((p) << 2))", None),
        ("#define DA1470X_WAKEUP_STATUS_P(p) (DA1470X_WAKEUP_BASE + 0x0020 + ((p) << 2))", None),
        ("#define DA1470X_WAKEUP_CLEAR_P(p) (DA1470X_WAKEUP_BASE + 0x002c + ((p) << 2))", None),
        ("#define DA1470X_WAKEUP_SEL_GPIO_P(p) (DA1470X_WAKEUP_BASE + 0x0038 + ((p) << 2))", None),
        ("#define DA1470X_WAKEUP_SEL1_GPIO_P(p) (DA1470X_WAKEUP_BASE + 0x0044 + ((p) << 2))", None),
    ],
    "LCDC": [
        ("/* Helpers ******************************************************/", None),
        ("", None),
        ("#define DA1470X_LCDC_IDREG_MAGIC 0x87452365", None),
        ("#define LCDC_XY_PACK(x, y) ((((uint32_t)(x)) << 16) | ((uint32_t)(y) & 0xffff))", None),
        ("#define LCDC_DBIB_CFG_INTERFACE_WIDTH_QSPI (5 << LCDC_DBIB_CFG_DBIB_INTERFACE_WIDTH_SHIFT)", None),
        ("#define LCDC_DBIB_CMD_CMD_WIDTH_8 (0 << LCDC_DBIB_CMD_CMD_WIDTH_SHIFT)", None),
        ("#define LCDC_DBIB_CMD_CMD_WIDTH_16 (1 << LCDC_DBIB_CMD_CMD_WIDTH_SHIFT)", None),
        ("#define LCDC_DBIB_CMD_CMD_WIDTH_24 (2 << LCDC_DBIB_CMD_CMD_WIDTH_SHIFT)", None),
        ("#define LCDC_GPIO_OUTPUT_MODE_JDI (0 << LCDC_GPIO_GPIO_OUTPUT_MODE_SHIFT)", None),
        ("#define LCDC_GPIO_OUTPUT_MODE_DPI (1 << LCDC_GPIO_GPIO_OUTPUT_MODE_SHIFT)", None),
        ("#define LCDC_GPIO_OUTPUT_MODE_DBIB (2 << LCDC_GPIO_GPIO_OUTPUT_MODE_SHIFT)", None),
        ("#define LCDC_GPIO_OUTPUT_MODE_SPI (3 << LCDC_GPIO_GPIO_OUTPUT_MODE_SHIFT)", None),
        ("#define LCDC_OCM_8RGB565 0x05", "Output color mode RGB565"),
        ("#define LCDC_OCM_8RGB888 0x07", "Output color mode RGB888"),
        ("#define LCDC_LCM_RGB565 0x05", "Layer color mode RGB565"),
        ("#define LCDC_LCM_RGBA8888 0x0d", "Layer color mode RGBA8888"),
    ],
    "SPI": [
        ("/* Helpers ******************************************************/", None),
        ("", None),
        ("#define SPI_CONFIG_WORDLEN(bits) (((bits) - 1) << SPI_CONFIG_WORD_LENGTH_SHIFT)", None),
        ("#define SPI_CS_CONFIG_NONE 0", None),
        ("#define SPI_CS_CONFIG_CS0 1", None),
        ("#define SPI_CS_CONFIG_CS1 2", None),
        ("#define SPI_CS_CONFIG_GPIO 7", None),
        ("#define DA1470X_SPI_FIFO_DEPTH 32", None),
    ],
    "I2C": [
        ("/* Helpers ******************************************************/", None),
        ("", None),
        ("#define I2C_CON_SPEED_STANDARD (1 << I2C_CON_SPEED_SHIFT)", None),
        ("#define I2C_CON_SPEED_FAST (2 << I2C_CON_SPEED_SHIFT)", None),
        ("#define I2C_CON_SPEED_HIGH (3 << I2C_CON_SPEED_SHIFT)", None),
        ("#define DA1470X_I2C_FIFO_DEPTH 4", None),
    ],
}


def parse(hpath):
    text = open(hpath, encoding="utf-8", errors="replace").read().split("\n")
    structs = {}
    i = 0
    n = len(text)
    while i < n:
        m = re.match(r"typedef struct \{\s*/\*!< \(@ (0x[0-9A-Fa-f]+)\) (\w+) Structure", text[i])
        if m:
            blk = m.group(2)
            regs = []
            i += 1
            while i < n and not text[i].startswith("}"):
                mm = re.match(r"\s*__[IO]+M?\s+uint(\d+)_t\s+(\w+)(\[\d+\])?;\s*/\*!< \(@ (0x[0-9A-Fa-f]+)\)\s*(.*)$", text[i])
                if mm and not mm.group(2).startswith("RESERVED"):
                    desc = mm.group(5).strip()
                    desc = desc.split("*/")[0].strip()
                    regs.append((mm.group(2), int(mm.group(4), 16), desc, int(mm.group(1))))
                i += 1
            structs[blk] = regs
        i += 1

    fields = {}
    rx = re.compile(r"#define (\w+)_(Pos|Msk)\s+\((0x[0-9A-Fa-f]+|\d+)UL\)")
    for line in text:
        m = rx.match(line)
        if m:
            fields.setdefault(m.group(1), {})[m.group(2)] = int(m.group(3), 0)

    bases = {}
    for line in text:
        m = re.match(r"#define (\w+)_BASE\s+(0x[0-9A-Fa-f]+)UL", line)
        if m:
            bases[m.group(1)] = int(m.group(2), 16)
    return structs, fields, bases


def shortreg(inst, reg):
    r = reg
    if r.startswith(inst + "_"):
        r = r[len(inst) + 1:]
    if r.endswith("_REG"):
        r = r[:-4]
    if r.endswith("_OFFSET"):
        r = r[:-7] + "_OFS"
    return r


def align(lines):
    """lines: list of (code, comment).  Return aligned strings."""
    out = []
    if not lines:
        return out
    width = max(len(c) for c, _ in lines if _ is not None) if any(cm for _, cm in lines) else 0
    for code, cmt in lines:
        if code.startswith("/* ") and code.endswith("*/") and "***" in code:
            t = code.split(" **")[0][3:].rstrip()
            code = "/* " + t + " " + "*" * (78 - len(t) - 6) + "*/"
        if cmt:
            cmt = cmt.replace("*/", "").strip()
            cmt = cmt[:MAXCOMMENT].rstrip()
            out.append("%s%s /* %s */" % (code, " " * (width - len(code)), cmt))
        else:
            out.append(code)
    return out


def gen_block(structs, fields, cmsis, nuttx, insts, outdir):
    """cmsis: CMSIS type name of the primary instance (e.g. UART).
    insts: list of (cmsis_inst_name, nuttx_inst_suffix) e.g. [("UART","0"),("UART2","1")]
    or [] for single-instance blocks."""
    regs = structs[cmsis]
    fname = "da1470x_%s.h" % nuttx.lower()
    guard = "DA1470X_%s" % nuttx.upper()
    body = []
    body.append("/* Register Offsets *********************************************************/\n")
    lines = []
    for name, off, desc, _ in regs:
        sr = shortreg(cmsis, name)
        lines.append(("#define DA1470X_%s_%s_OFFSET 0x%04x" % (nuttx, sr, off), desc))
    body += align(lines)
    body.append("")
    body.append("/* Register Addresses *******************************************************/\n")
    if insts:
        for ci, suf in insts:
            lines = []
            for name, off, desc, _ in regs:
                sr = shortreg(cmsis, name)
                lines.append(("#define DA1470X_%s%s_%s (DA1470X_%s%s_BASE + DA1470X_%s_%s_OFFSET)"
                              % (nuttx, suf, sr, nuttx, suf, nuttx, sr), None))
            body += align(lines)
            body.append("")
    else:
        lines = []
        for name, off, desc, _ in regs:
            sr = shortreg(cmsis, name)
            lines.append(("#define DA1470X_%s_%s (DA1470X_%s_BASE + DA1470X_%s_%s_OFFSET)"
                          % (nuttx, sr, nuttx, nuttx, sr), None))
        body += align(lines)
        body.append("")
    body.append("/* Register Bit-field Definitions *******************************************/\n")
    for name, off, desc, width in regs:
        sr = shortreg(cmsis, name)
        prefix = "%s_%s_" % (cmsis, name)
        flds = []
        for key, pm in fields.items():
            if key.startswith(prefix) and "Pos" in pm and "Msk" in pm:
                f = key[len(prefix):]
                if f.startswith(cmsis + "_") and len(f) > len(cmsis) + 1:
                    f = f[len(cmsis) + 1:]
                flds.append((pm["Pos"], pm["Msk"], f))
        if not flds:
            continue
        flds.sort()
        body.append("/* %s register */\n" % sr)
        lines = []
        for pos, msk, fld in flds:
            nb = bin(msk >> pos).count("1")
            base = "%s_%s_%s" % (nuttx, sr, fld)
            if nb == 1:
                lines.append(("#define %s (1 << %d)" % (base, pos), "Bit %d" % pos))
            else:
                lines.append(("#define %s_SHIFT (%d)" % (base, pos), "Bits %d-%d" % (pos, pos + nb - 1)))
                lines.append(("#define %s_MASK (0x%x << %s_SHIFT)" % (base, msk >> pos, base), None))
                lines.append(("#  define %s(n) ((uint32_t)(n) << %s_SHIFT)" % (base, base), None))
        body += align(lines)
        body.append("")
    if nuttx in SUPPLEMENT:
        body += align(SUPPLEMENT[nuttx])
        body.append("")
    txt = LICENSE.format(fname=fname, guard=guard) + "\n".join(body) + FOOTER.format(guard=guard)
    open(os.path.join(outdir, fname), "w").write(txt)
    print("wrote", fname, len(regs), "regs")


def main():
    hpath, outdir = sys.argv[1], sys.argv[2]
    structs, fields, bases = parse(hpath)
    for spec in sys.argv[3:]:
        # CMSIS=NUTTX:inst0=CMSISINST0,inst1=CMSISINST1
        cmsis, _, rest = spec.partition("=")
        nuttx = cmsis
        insts = []
        if rest:
            nuttx, _, ilist = rest.partition(":")
            if ilist:
                for item in ilist.split(","):
                    suf, _, ci = item.partition("=")
                    insts.append((ci or cmsis, suf))
        gen_block(structs, fields, cmsis, nuttx, insts, outdir)


if __name__ == "__main__":
    main()
