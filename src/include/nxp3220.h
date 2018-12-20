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
#ifndef __NXP3220_H__
#define __NXP3220_H__

#define USE_NSIH_HEADER			1

#define SRAM_BASEADDR			0xFFFF0000

#define SRAM_BL2_BASEADDR		(SRAM_BASEADDR + 0x9000)
#define SRAM_SIZE			(64 * 1024)
#define BL2_SRAM_SIZE			(27 * 1024)
#define	BL2_STACK_SIZE			0x400

/* SDRAM in Boot Loader 0 File Format */
#define HEADER_BASEADDR			(SRAM_BASEADDR + SRAM_SIZE - (1 * 1024))
#define HASHSIGN_BASEADDR		(HEADER_BASEADDR   + 256)
#define BOOTKEY_BASEADDR		(HASHSIGN_BASEADDR + 256)
#define USERKEY_BASEADDR		(BOOTKEY_BASEADDR  + 256)

#define RE_HEADER_BASEADDR		USERKEY_BASEADDR

#endif /* #ifndef __NXP3220_H__ */