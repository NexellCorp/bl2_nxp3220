/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexcel and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#include "../include/asm.h"
#include "../include/armv7.h"
	/* below instruction number is 8, 32bytes */
	.align 5

ENTRY(set_nonsecure_mode)
	mrc	p15, 0, r0, c1, c1, 0
	orr	r0, r0, #(1 << 0)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(set_nonsecure_mode)

ENTRY(set_secure_mode)
	mrc	p15, 0, r0, c1, c1, 0
	bic	r0, r0, #(1 << 0)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(set_secure_mode)

ENTRY(get_secure_status)
	mrc	p15, 0, r0, c1, c1, 0
	and	r0, r0, #(1 << 0)
	bx	lr
ENDPROC(get_secure_status)

ENTRY(armv7_get_cpuid)
	mrc	p15, 0, r0, c0, c0, 5
	ands	r0, r0, #0xF
	bx	lr
ENDPROC(armv7_get_cpuid)

ENTRY(armv7_get_mpidr)
	mrc	p15, 0, r0, c0, c0, 5						@ Get MPIDR
	bx	lr
ENDPROC(armv7_get_mpidr)

ENTRY(armv7_set_auxctrl)
	mcr	p15, 0, r0, c1, c0, 1
	bx	lr
ENDPROC(armv7_set_auxctrl)

ENTRY(armv7_get_auxctrl)
	mrc	p15, 0, r0, c1, c0, 1
	bx	lr
ENDPROC(armv7_get_auxctrl)

ENTRY(armv7_get_scr)
	mrc	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(armv7_get_scr)

ENTRY(armv7_set_scr)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(armv7_set_scr)

ENTRY(set_mon_mode)
	msr	CPSR_c,  #(MODE_MON|I_BIT)
	bx	lr
ENDPROC(set_mon_mode)

ENTRY(set_svc_mode)
	msr	CPSR_c,  #(MODE_SVC|I_BIT)
	bx	lr
ENDPROC(set_svc_mode)
