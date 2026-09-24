/****************************************************************************
 * arch/arm/src/da1470x/snc/snc_start.S
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

/* Start-up of the sensor node controller firmware: vector table, image
 * header and reset handler.  The M33 copies the whole image, initialised
 * data included, to the controller's address zero before releasing it, so
 * nothing needs copying here; only .bss is cleared.
 */

/* The shared headers keep their C parts out of assembly */

#ifndef __ASSEMBLY__
#  define __ASSEMBLY__
#endif

#include "../da1470x_snc_ipc.h"

	.syntax	unified
	.cpu	cortex-m0plus
	.thumb

/* Vector table: 16 core entries, then the 32 interrupts of the controller
 * (IRQ 0 is SYS2SNC, the M33's doorbell).  Every interrupt handler is weak
 * and falls back to snc_unexpected.
 */

	.macro	vector name
	.weak	\name
	.thumb_set \name, snc_unexpected
	.word	\name
	.endm

	.section .vectors, "a"
	.global	snc_vectors
snc_vectors:
	.word	_snc_stack_top
	.word	snc_reset
	.word	snc_nmi
	.word	snc_hardfault
	.word	0, 0, 0, 0, 0, 0, 0
	vector	snc_svcall
	.word	0, 0
	vector	snc_pendsv
	vector	snc_systick
	vector	snc_irq_sys2snc			/*  0 */
	vector	snc_irq_cmac2snc		/*  1 */
	vector	snc_irq_pdc			/*  2 */
	vector	snc_irq_key_wkup_gpio		/*  3 */
	vector	snc_irq_gpio_p0			/*  4 */
	vector	snc_irq_gpio_p1			/*  5 */
	vector	snc_irq_gpio_p2			/*  6 */
	vector	snc_irq_timer			/*  7 */
	vector	snc_irq_timer3			/*  8 */
	vector	snc_irq_timer4			/*  9 */
	vector	snc_irq_timer5			/* 10 */
	vector	snc_irq_timer6			/* 11 */
	vector	snc_irq_rtc			/* 12 */
	vector	snc_irq_rtc_event		/* 13 */
	vector	snc_irq_captimer		/* 14 */
	vector	snc_irq_adc			/* 15 */
	vector	snc_irq_uart			/* 16 */
	vector	snc_irq_uart2			/* 17 */
	vector	snc_irq_uart3			/* 18 */
	vector	snc_irq_spi			/* 19 */
	vector	snc_irq_spi2			/* 20 */
	vector	snc_irq_spi3			/* 21 */
	vector	snc_irq_i2c			/* 22 */
	vector	snc_irq_i2c2			/* 23 */
	vector	snc_irq_i2c3			/* 24 */
	vector	snc_irq_i3c			/* 25 */
	vector	snc_irq_pcm			/* 26 */
	vector	snc_irq_src_in			/* 27 */
	vector	snc_irq_src_out			/* 28 */
	vector	snc_irq_src2_in			/* 29 */
	vector	snc_irq_src2_out		/* 30 */
	vector	snc_irq_vad			/* 31 */

/* The header the M33 checks before loading the image */

	.section .snc_header, "a"
	.global	snc_image_header
snc_image_header:
	.word	SNC_IPC_MAGIC
	.word	SNC_IPC_VERSION
	.word	_snc_image_end
	.word	0

	.text

/* Reset: clear .bss and run the application */

	.thumb_func
	.global	snc_reset
snc_reset:
	ldr	r0, =_snc_sbss
	ldr	r1, =_snc_ebss
	movs	r2, #0
1:
	cmp	r0, r1
	bhs	2f
	str	r2, [r0]
	adds	r0, #4
	b	1b
2:
	bl	main
3:
	b	3b

/* A fault is reported to the M33 through the shared area: the stacked PC
 * and LR go to snc_fault(), which records them and rings the doorbell.
 * Only the main stack is used, so the frame is at MSP.
 */

	.thumb_func
	.global	snc_hardfault
snc_hardfault:
	mrs	r0, msp
	bl	snc_fault
4:
	b	4b

	.thumb_func
	.global	snc_nmi
snc_nmi:
	mrs	r0, msp
	bl	snc_fault
5:
	b	5b

	.thumb_func
	.global	snc_unexpected
snc_unexpected:
	mrs	r0, msp
	bl	snc_fault
6:
	b	6b

	.end
