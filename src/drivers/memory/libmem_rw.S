/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: hans <hans@nexell.co.kr>
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
#include "../../include/asm.h"
#include "../../include/armv7.h"

@ void wrtest(u32 *dst, u32 *src);
ENTRY(rwtest)
	push {r0-r9, lr}
	ldm r1!, {r2-r9}
	stm r0!, {r2-r9}
	pop  {r0-r9, lr}
	bx	lr
ENDPROC(rwtest)
@ void burstread8(u32 *src, u32 *buf);
ENTRY(burstread8)
	push {r0-r9, lr}
	ldm r0!, {r2-r9}
	stm r1!, {r2-r9}
	pop  {r0-r9, lr}
	bx	lr
ENDPROC(burstread8)
@ void burstwrite8(u32 *dst, u32 *buf);
ENTRY(burstwrite8)
	push {r0-r9, lr}
	ldm r1!, {r2-r9}
	stm r0!, {r2-r9}
	pop  {r0-r9, lr}
	bx	lr
ENDPROC(burstwrite8)
@ void burstread4(u32 *src, u32 *buf);
ENTRY(burstread4)
	push {r0-r5, lr}
	ldm r0!, {r2-r5}
	stm r1!, {r2-r5}
	pop  {r0-r5, lr}
	bx	lr
ENDPROC(burstread4)
@ void burstwrite4(u32 *dst, u32 *buf);
ENTRY(burstwrite4)
	push {r0-r5, lr}
	ldm r1!, {r2-r5}
	stm r0!, {r2-r5}
	pop  {r0-r5, lr}
	bx	lr
ENDPROC(burstwrite4)
