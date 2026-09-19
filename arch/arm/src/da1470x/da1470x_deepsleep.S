/****************************************************************************
 * arch/arm/src/da1470x/da1470x_deepsleep.S
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

/* Going to sleep with PD_SYS switched off costs the core its registers: the
 * processor comes back through the reset vector with nothing but the RAM it
 * left behind.  So the state that matters is written to RAM first, and the
 * reset path reads it back and returns into the function that asked to
 * sleep, as though the WFI had simply taken a long time.
 *
 * What has to survive is everything the hardware does not restore itself:
 * the callee-saved registers and the stack pointers, plus the pieces of the
 * NVIC and the system control block that configure interrupts, priorities
 * and the FPU.  Anything else is either recomputed by the resume path in C
 * or lives in RAM already.
 *
 * The caller sees goto_deepsleep() return 0 if the part never actually
 * slept -- an interrupt arrived first, or the PMU refused -- and 1 if it
 * slept and came back.  The return in the second case is manufactured by
 * wakeup_from_deepsleep(), which the reset path jumps to.
 *
 * The retained block lives in .bss.  Every RAM bank is retained through
 * this state (RAM_PWR_CTRL is left at its reset value), so .bss and .data
 * come back as they were and the resume path must not re-initialise them.
 *
 * Modelled on the sequence in the vendor SDK (sdk/bsp/startup/DA1470x/GCC/
 * deepsleep.S), which is the only description of what the silicon needs.
 */

#include <nuttx/config.h>

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP

	.syntax	unified
	.arch	armv8-m.main
	.thumb

	.equ	RESET_STAT_REG,		0x500000bc
	.equ	PDC_BASE,		0x50000200
	.equ	PDC_PENDING_CM33_OFF,	0x8c
	.equ	PDC_ACKNOWLEDGE_OFF,	0x80
	.equ	PDC_LAST_ENTRY,		15

	.equ	SCB_BASE,		0xe000ed00
	.equ	SCB_SCR_OFF,		0x10
	.equ	SCB_SHPR1_OFF,		0x18
	.equ	SCB_SHPR2_OFF,		0x1c
	.equ	SCB_SHPR3_OFF,		0x20
	.equ	SCB_SHCSR_OFF,		0x24
	.equ	SCB_FAULTENA_MASK,	0x000f0000

	.equ	CPACR,			0xe000ed88
	.equ	FPCCR,			0xe000ef34
	.equ	FPDSCR,			0xe000ef3c

	.equ	NVIC_ISER,		0xe000e100
	.equ	NVIC_IPR,		0xe000e400

/****************************************************************************
 * Retained state
 ****************************************************************************/

	.section .bss
	.align	3
	.globl	g_da1470x_retained_state

	/* Room for a stacked exception frame, in case one is taken while the
	 * state is being restored and the real stack is not back yet.
	 */

	.space	64
g_da1470x_retained_state:
.stored_primask:	.space	4
.stored_r4:		.space	4 * 9	/* r4-r12 */
.stored_lr:		.space	4
.stored_msp:		.space	4
.stored_psp:		.space	4
.stored_control:	.space	4
.stored_iser:		.space	4 * 2	/* NVIC ISER[0..1] */
.stored_ipr:		.space	4 * 14	/* NVIC IPR[0..55] */
.stored_scr:		.space	4
.stored_shpr1:		.space	4
.stored_shpr2:		.space	4
.stored_shpr3:		.space	4
.stored_faultena:	.space	4
.stored_cpacr:		.space	4
.stored_fpccr:		.space	4
.stored_fpdscr:		.space	4
	.size	g_da1470x_retained_state, . - g_da1470x_retained_state

/****************************************************************************
 * Name: goto_deepsleep
 *
 * Description:
 *   Write the processor state to retained RAM and stop at WFI with the PMU
 *   allowed to switch PD_SYS off.  Returns 0 if sleep did not happen; the
 *   return of 1 is produced by wakeup_from_deepsleep() instead.
 *
 ****************************************************************************/

	.section .text
	.thumb_func
	.align	1
	.globl	goto_deepsleep
	.type	goto_deepsleep, %function

goto_deepsleep:

	/* Interrupts off for the whole sequence; PRIMASK comes back with the
	 * rest of the state so the caller's masking is preserved either way.
	 */

	mrs	r0, primask
	cpsid	i
	ldr	r3, =.stored_primask
	stm	r3!, {r0}

	/* Acknowledge every PDC entry pending for the CM33.  The PMU refuses
	 * to drop PD_SYS while one is outstanding.  Only r0-r2 are used here,
	 * the state registers are still live.
	 */

	ldr	r0, =PDC_BASE
	ldr	r1, [r0, #PDC_PENDING_CM33_OFF]
	movs	r0, #0
	b	2f
1:	adds	r0, #1
2:	cmp	r0, #PDC_LAST_ENTRY
	bgt	3f
	movs	r2, #1
	lsls	r2, r0
	tst	r1, r2
	beq	1b
	ldr	r2, =PDC_BASE
	str.w	r0, [r2, #PDC_ACKNOWLEDGE_OFF]
	b	1b
3:

	/* r4-r12 and LR */

	stm	r3!, {r4-r12, lr}

	/* Stack pointers and CONTROL */

	mrs	r0, msp
	mrs	r1, psp
	mrs	r2, control
	stm	r3!, {r0-r2}

	/* NVIC enables and priorities */

	ldr	r0, =NVIC_ISER
	ldm	r0, {r4-r5}
	stm	r3!, {r4-r5}

	ldr	r0, =NVIC_IPR
	ldm	r0!, {r2, r4-r12}
	stm	r3!, {r2, r4-r12}
	ldm	r0, {r4-r7}
	stm	r3!, {r4-r7}

	/* SCB: SCR, the exception priorities and the fault enables */

	ldr	r0, =SCB_BASE
	ldr	r4, [r0, #SCB_SCR_OFF]
	ldr	r5, [r0, #SCB_SHPR1_OFF]
	ldr	r6, [r0, #SCB_SHPR2_OFF]
	ldr	r7, [r0, #SCB_SHPR3_OFF]
	ldr	r8, [r0, #SCB_SHCSR_OFF]
	and	r8, r8, #SCB_FAULTENA_MASK
	stm	r3!, {r4-r8}

	mov	r9, r0			/* keep SCB base */
	mov	r10, r4			/* keep SCR for the no-sleep path */

	/* FPU configuration */

	ldr	r4, [r0, #(CPACR - SCB_BASE)]
	and	r4, r4, #(0xf << 20)
	ldr	r5, [r0, #(FPCCR - SCB_BASE)]
	and	r5, r5, #(0xf << 28)
	ldr	r6, [r0, #(FPDSCR - SCB_BASE)]
	stm	r3!, {r4-r6}

	/* Zero the reset status.  Nothing else writes it while we are awake,
	 * so a zero read out of the reset path means "this was a wake-up, not
	 * a reset" -- the one piece of evidence that survives PD_SYS going
	 * away.
	 */

	ldr	r1, =RESET_STAT_REG
	movs	r0, #0
	str	r0, [r1]

	/* SCR.SLEEPDEEP, then stop */

	mov	r0, r10
	orr	r0, r0, #4
	str	r0, [r9, #SCB_SCR_OFF]

	dsb
	isb
	wfi

	/* Still here: the part did not sleep after all.  Put SCR back and
	 * tell the caller nothing happened.
	 */

	str	r10, [r9, #SCB_SCR_OFF]
	movs	r0, #0
	b	goto_deepsleep_return

	.pool
	.size	goto_deepsleep, . - goto_deepsleep

/****************************************************************************
 * Name: wakeup_from_deepsleep
 *
 * Description:
 *   Entered from the reset path when it finds the reset status zeroed.
 *   Puts back everything goto_deepsleep() saved and returns 1 into its
 *   caller.
 *
 ****************************************************************************/

	.thumb_func
	.align	1
	.globl	wakeup_from_deepsleep
	.type	wakeup_from_deepsleep, %function

wakeup_from_deepsleep:
	cpsid	i

	/* The stack pointer is whatever reset left behind, so park it in the
	 * room reserved below the state block until the real one is restored.
	 */

	ldr	r0, =g_da1470x_retained_state
	mov	sp, r0

	ldr	r0, =.stored_iser

	ldm	r0!, {r4-r5}
	ldr	r1, =NVIC_ISER
	stm	r1, {r4-r5}

	ldm	r0!, {r2, r4-r12}
	ldr	r1, =NVIC_IPR
	stm	r1!, {r2, r4-r12}
	ldm	r0!, {r4-r7}
	stm	r1, {r4-r7}

	ldm	r0!, {r4-r8}
	ldr	r1, =SCB_BASE
	str	r4, [r1, #SCB_SCR_OFF]
	str	r5, [r1, #SCB_SHPR1_OFF]
	str	r6, [r1, #SCB_SHPR2_OFF]
	str	r7, [r1, #SCB_SHPR3_OFF]
	ldr	r2, [r1, #SCB_SHCSR_OFF]
	orr	r2, r2, r8
	str	r2, [r1, #SCB_SHCSR_OFF]

	/* FPU: the enables are set, the rest of the register is left alone */

	ldm	r0!, {r4-r6}
	ldr	r2, [r1, #(CPACR - SCB_BASE)]
	orr	r2, r2, r4
	str	r2, [r1, #(CPACR - SCB_BASE)]
	ldr	r2, [r1, #(FPCCR - SCB_BASE)]
	orr	r2, r2, r5
	str	r2, [r1, #(FPCCR - SCB_BASE)]
	str	r6, [r1, #(FPDSCR - SCB_BASE)]

	/* Stack pointers and CONTROL */

	ldr	r0, =.stored_msp
	ldm	r0, {r1-r3}
	msr	msp, r1
	msr	psp, r2
	msr	control, r3
	isb

	movs	r0, #1
	b	goto_deepsleep_return

	.pool
	.size	wakeup_from_deepsleep, . - wakeup_from_deepsleep

/****************************************************************************
 * Name: goto_deepsleep_return
 *
 * Description:
 *   Shared tail: restore the working registers and return to whoever
 *   called goto_deepsleep(), carrying r0 as the answer.
 *
 ****************************************************************************/

	.thumb_func
	.align	1
	.type	goto_deepsleep_return, %function

goto_deepsleep_return:
	ldr	r1, =.stored_r4
	ldm	r1, {r4-r12, lr}

	ldr	r2, [r1, #(.stored_primask - .stored_r4)]
	msr	primask, r2

	bx	lr

	.pool
	.size	goto_deepsleep_return, . - goto_deepsleep_return

#endif /* CONFIG_DA1470X_PM_EXTENDED_SLEEP */
