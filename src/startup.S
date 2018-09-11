/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#include "include/armv7.h"
#include "include/nxp3220.h"

        .align

/* arm exceptions vectors */
.global vectors
vectors:
        b       reset_handler       						/* 00 - Reset				*/
        b       .          							/* 04 - Undefined instructions		*/
        b       .                   						/* 08 - SWI instructions		*/
        b       .          							/* 0C - Instruction fetch aborts	*/
        b       .          							/* 10 - Data access aborts		*/
        b       .                  						/* 14 - Reserved (was address exception)*/
        b       .                   						/* 18 - IRQ interrupts			*/
        b       .                   						/* 1C - FIQ interrupts			*/

reset_handler:
	/* Change the ARM Mode(Secure->Non-Secure) */
	msr     CPSR_c, #(MODE_SVC|I_BIT|F_BIT)

	/* Set the BSS-Area to zero. */
        ldr     r1, =__bss_start__
        ldr     r2, =__bss_end__

        mov     r3, #0x00000000

clbss_l:
        cmp     r1, r2
        strlo   r3, [r1]
        addlo   r1, r1, #4
        blo     clbss_l

set_stack:
	/* Set Stack-Pointer (svc: 1kbyte) */
	ldr     sp, =SRAM_BL2_BASEADDR
        add     sp, sp, #BL2_SRAM_SIZE						/* Add to internal sram size.	*/
        sub	sp, sp, #0x400							/* Hader + User-Key + Booy Key	*/

	bl	armv7_pmu_init

	bl	main
	b	.
