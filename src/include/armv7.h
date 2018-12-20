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
#ifndef __ARMV7_H__
#define __ARMV7_H__

#define MODE_USR		0x10
#define MODE_FIQ		0x11
#define MODE_IRQ		0x12
#define MODE_SVC		0x13
#define MODE_MON		0x16
#define MODE_ABT		0x17
#define MODE_UNDEF		0x1B
#define MODE_SYS		0x1F

#define A_BIT			(1 << 8)					/* A bit is set, Abort is disabled		*/
#define I_BIT			(1 << 7)					/* I bit is set, IRQ is disabled		*/
#define F_BIT			(1 << 6)					/* F bit is set, FIQ is disabled		*/

/* Control register 1 format bit definition */
#define BIT1_XP			(1 <<  23)					/* Subpage AP					*/
#define BIT1_U			(1 <<  22)					/* Enables unaligned data access		*/
#define BIT1_L4			(1 <<  15)					/* for ARM4V architecture			*/
#define BIT1_RR			(1 <<  14)					/* Replacement strategy for ICache & DCache	*/
#define BIT1_V			(1 <<  13)					/* Loaction of exception vectors		*/
#define BIT1_I			(1 <<  12)					/* ICache enable/disable			*/
#define BIT1_Z			(1 <<  11)					/* Branch prediction				*/
#define BIT1_R			(1 <<   9)					/* ROM protection				*/
#define BIT1_S			(1 <<   8)					/* system protection				*/
#define BIT1_B			(1 <<   7)					/* Endianness(Bin/Little : 1/0)			*/
#define BIT1_C			(1 <<   2)					/* DCache enable/disable			*/
#define BIT1_A			(1 <<   1)					/* Alignment fault enable/disable		*/
#define BIT1_M			(1 <<   0)					/* MMU enable/disable				*/

#endif // #ifndef __ARMV7_H__
