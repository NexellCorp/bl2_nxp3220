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
#include "../include/asm.h"
#include "../include/armv7.h"

	.align	4

ENTRY(bl1_get_fnptr)
	mrc	p15, 0, r0, c13, c0, 4						@ TPIDRPRW pl1 only thread id rw reg
	bx	lr
ENDPROC(bl1_get_fnptr)

ENTRY(bl1_set_fnptr)
	mcr	p15, 0, r0, c13, c0, 4
	bx	lr
ENDPROC(bl1_set_fnptr)
