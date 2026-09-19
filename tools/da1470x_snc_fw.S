/* Minimal Cortex-M0+ firmware for the DA1470x sensor node controller.
 *
 * Addresses are the SNC's own view: RAM1 (its code) is at 0, RAM2 (its
 * data) at 0x8000, which the Cortex-M33 sees at 0x20000000 and 0x20008000.
 *
 * Writes a signature so the M33 can tell "the core ran" from "the RAM
 * happened to hold something", then counts for ever so that liveness is
 * visible as a changing value and the core's own current draw is
 * measurable.
 */

	.syntax	unified
	.cpu	cortex-m0plus
	.thumb
	.section .vectors, "ax"

	.word	0x00010000		/* initial SP: top of RAM2 */
	.word	reset + 1		/* reset handler, thumb bit set */

	.text
	.thumb_func
reset:
	ldr	r0, =0x00008000		/* signature word */
	ldr	r1, =0x5DC00001
	str	r1, [r0]

	ldr	r0, =0x00008004		/* counter */
	movs	r1, #0
loop:
	adds	r1, #1
	str	r1, [r0]
	b	loop
