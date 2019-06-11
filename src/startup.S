/* Copyright (C) 2018  Nexell Co., Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the Nexell nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL,SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
