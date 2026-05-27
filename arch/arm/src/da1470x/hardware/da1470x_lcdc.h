/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_lcdc.h
 *
 * LCDC register definitions for DA1470x.
 *
 * The LCDC sits at 0x30030000 in the GPU memory region (accessible only
 * once PD_GPU is up). It supports DPI parallel RGB, MIPI DBI Type-B
 * (parallel and 3/4-wire SPI variants), MIPI DSI command/video, and the
 * Sharp Memory JDI interface. Two compositing layers, a 256-entry
 * palette, and built-in gamma.
 *
 * Clock is gated by CRG_SYS.CLK_SYS_REG.LCD_ENABLE (not by CRG_GPU,
 * which controls the GPU core itself).
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_LCDC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_LCDC_H

#include <nuttx/config.h>

/* Register offsets *********************************************************/

#define DA1470_LCDC_MODE_OFFSET             0x0000
#define DA1470_LCDC_CLKCTRL_OFFSET          0x0004
#define DA1470_LCDC_BGCOLOR_OFFSET          0x0008
#define DA1470_LCDC_RESXY_OFFSET            0x000C
#define DA1470_LCDC_FRONTPORCHXY_OFFSET     0x0014
#define DA1470_LCDC_BLANKINGXY_OFFSET       0x0018
#define DA1470_LCDC_BACKPORCHXY_OFFSET      0x001C
#define DA1470_LCDC_STARTXY_OFFSET          0x0024
#define DA1470_LCDC_DBIB_CFG_OFFSET         0x0028
#define DA1470_LCDC_GPIO_OFFSET             0x002C
#define DA1470_LCDC_LAYER0_MODE_OFFSET      0x0030
#define DA1470_LCDC_LAYER0_STARTXY_OFFSET   0x0034
#define DA1470_LCDC_LAYER0_SIZEXY_OFFSET    0x0038
#define DA1470_LCDC_LAYER0_BASEADDR_OFFSET  0x003C
#define DA1470_LCDC_LAYER0_STRIDE_OFFSET    0x0040
#define DA1470_LCDC_LAYER0_RESXY_OFFSET     0x0044
#define DA1470_LCDC_LAYER1_MODE_OFFSET      0x0050
#define DA1470_LCDC_LAYER1_STARTXY_OFFSET   0x0054
#define DA1470_LCDC_LAYER1_SIZEXY_OFFSET    0x0058
#define DA1470_LCDC_LAYER1_BASEADDR_OFFSET  0x005C
#define DA1470_LCDC_LAYER1_STRIDE_OFFSET    0x0060
#define DA1470_LCDC_LAYER1_RESXY_OFFSET     0x0064
#define DA1470_LCDC_DBIB_CMD_OFFSET         0x00E8
#define DA1470_LCDC_DBIB_RDAT_OFFSET        0x00EC
#define DA1470_LCDC_CONF_OFFSET             0x00F0
#define DA1470_LCDC_IDREG_OFFSET            0x00F4
#define DA1470_LCDC_INTERRUPT_OFFSET        0x00F8
#define DA1470_LCDC_STATUS_OFFSET           0x00FC
#define DA1470_LCDC_COLMOD_OFFSET           0x0100

/* Absolute addresses *******************************************************/

#define DA1470_LCDC_BASE                    0x30030000

#define DA1470_LCDC_MODE                    (DA1470_LCDC_BASE + DA1470_LCDC_MODE_OFFSET)
#define DA1470_LCDC_DBIB_CFG                (DA1470_LCDC_BASE + DA1470_LCDC_DBIB_CFG_OFFSET)
#define DA1470_LCDC_DBIB_CMD                (DA1470_LCDC_BASE + DA1470_LCDC_DBIB_CMD_OFFSET)
#define DA1470_LCDC_DBIB_RDAT               (DA1470_LCDC_BASE + DA1470_LCDC_DBIB_RDAT_OFFSET)
#define DA1470_LCDC_IDREG                   (DA1470_LCDC_BASE + DA1470_LCDC_IDREG_OFFSET)
#define DA1470_LCDC_STATUS                  (DA1470_LCDC_BASE + DA1470_LCDC_STATUS_OFFSET)
#define DA1470_LCDC_INTERRUPT               (DA1470_LCDC_BASE + DA1470_LCDC_INTERRUPT_OFFSET)

/* Magic value at LCDC_IDREG that confirms the block is alive and the
 * power domain / clock are up.
 */

#define DA1470_LCDC_IDREG_MAGIC             0x87452365UL

/* LCDC_MODE_REG bits *******************************************************/

#define LCDC_MODE_EN                        (1U << 31)
#define LCDC_MODE_VSYNC_POL                 (1U << 28)
#define LCDC_MODE_HSYNC_POL                 (1U << 27)
#define LCDC_MODE_DE_POL                    (1U << 26)
#define LCDC_MODE_FORCE_BLANK               (1U << 19)
#define LCDC_MODE_OUT_MODE_SHIFT            5
#define LCDC_MODE_OUT_MODE_MASK             (0xFU << 5)
#define LCDC_MODE_DBIB_OFF                  (1U << 4)

/* LCDC_DBIB_CFG_REG bits ***************************************************/

#define LCDC_DBIB_CFG_INTERFACE_EN          (1U << 31)
#define LCDC_DBIB_CFG_CSX_CFG_EN            (1U << 30)
#define LCDC_DBIB_CFG_CSX_CFG               (1U << 29)
#define LCDC_DBIB_CFG_TE_DISABLE            (1U << 28)
#define LCDC_DBIB_CFG_FORCE_IDLE            (1U << 26)
#define LCDC_DBIB_CFG_RESX_OUT_EN           (1U << 25)
#define LCDC_DBIB_CFG_SPI3_EN               (1U << 23)
#define LCDC_DBIB_CFG_SPI4_EN               (1U << 22)
#define LCDC_DBIB_CFG_SPI_CLK_PHASE         (1U << 20)
#define LCDC_DBIB_CFG_SPI_CLK_POLARITY      (1U << 19)
#define LCDC_DBIB_CFG_QUAD_SPI_EN           (1U << 10)
#define LCDC_DBIB_CFG_DUAL_SPI_EN           (1U <<  9)
#define LCDC_DBIB_CFG_INTERFACE_WIDTH_SHIFT 6
#define LCDC_DBIB_CFG_INTERFACE_WIDTH_MASK  (0xFU << 6)

/* LCDC_DBIB_CMD_REG bits ***************************************************/

#define LCDC_DBIB_CMD_PART_UPDATE           (1U << 31)
#define LCDC_DBIB_CMD_DBIB_CMD_SEND         (1U << 30)
#define LCDC_DBIB_CMD_CMD_WIDTH_SHIFT       28
#define LCDC_DBIB_CMD_CMD_WIDTH_MASK        (0x3U << 28)
#define LCDC_DBIB_CMD_RD_MODE_EN            (1U << 26)
#define LCDC_DBIB_CMD_ST_INT_CMD_TYPE       (1U << 24)
#define LCDC_DBIB_CMD_DBIB_CMD_VAL_MASK     0xFFU

/* LCDC_STATUS_REG bits *****************************************************/

#define LCDC_STATUS_SPI_RD_WR_OP            (1U << 16)
#define LCDC_STATUS_DBIB_CMD_FIFO_FULL      (1U << 15)
#define LCDC_STATUS_DBI_SPI_CS              (1U << 14)
#define LCDC_STATUS_FRAME_END               (1U << 13)
#define LCDC_STATUS_DBIB_OUT_TRANS_PENDING  (1U << 12)
#define LCDC_STATUS_DBIB_CMD_PENDING        (1U << 11)
#define LCDC_STATUS_DBIB_DATA_PENDING       (1U << 10)
#define LCDC_STATUS_DBIB_TE                 (1U <<  8)
#define LCDC_STATUS_UNDERFLOW               (1U <<  6)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_LCDC_H */
