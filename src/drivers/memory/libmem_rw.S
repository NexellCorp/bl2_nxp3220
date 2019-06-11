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
