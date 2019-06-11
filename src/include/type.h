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
#ifndef __TYPE_H__
#define __TYPE_H__

#define BIT(nr)				(1 << (nr))
#define BIT_N(val, nr)			(val << (nr))

#ifdef aarch32
#define PTR unsigned int
#else
#define PTR unsigned long long
#endif

typedef char				int8_t;					//  8bit signed integer(s.7) value
typedef short				int16_t;				// 16bit signed integer(s.15) value
typedef int				int32_t;				// 32bit signed integer(s.31) value
typedef long long			int64_t;				// 64bit signed integer(s.63) value
typedef unsigned char			uint8_t;				//  8bit unsigned integer value
typedef unsigned short			uint16_t;				// 16bit unsigned integer value
typedef unsigned int			uint32_t;				// 32bit unsigned integer value
typedef unsigned long long		uint64_t;				// 64bit unsigned integer vlaue
typedef unsigned int			size_t;

#define u8				unsigned char
#define u16				unsigned short
#define u32				unsigned int
#define u64				unsigned long long

#define s8				char
#define s16				short
#define s32				int
#define s64				long long

#define true				0
#define false				-1

#define TRUE				1
#define FALSE				0

#define IO_ADDRESS(x)			(x)

#define mmio_read_32(addr)		(*(volatile unsigned int  *)(addr))
#define mmio_read_16(addr)		(*(volatile unsigned short*)(addr))
#define mmio_read_8(addr)		(*(volatile unsigned char *)(addr))

#define mmio_write_32(addr,data)	(*(volatile unsigned int  *)(addr))  =  ((unsigned int  )(data))
#define mmio_write_16(addr,data)	(*(volatile unsigned short*)(addr))  =  ((unsigned short)(data))
#define mmio_write_8(addr,data)		(*(volatile unsigned char *)(addr))  =  ((unsigned char )(data))

#define mmio_set_32(addr,data)		(*(volatile unsigned int  *)(addr)) |=  ((unsigned int  )(data))
#define mmio_set_16(addr,data)		(*(volatile unsigned short*)(addr)) |=  ((unsigned short)(data))
#define mmio_set_8(addr,data)		(*(volatile unsigned char *)(addr)) |=  ((unsigned char )(data))

#define mmio_clear_32(addr,data)	(*(volatile unsigned int  *)(addr)) &= ~((unsigned int  )(data))
#define mmio_clear_16(addr,data)	(*(volatile unsigned short*)(addr)) &= ~((unsigned short)(data))
#define mmio_clear_8(addr,data)		(*(volatile unsigned char *)(addr)) &= ~((unsigned char )(data))

#define clearb(addr, data)		mmio_clear_8(addr,data)
#define clearw(addr, data)		mmio_clear_16(addr,data)
#define clearl(addr, data)		mmio_clear_32(addr,data)

#define setb(addr, data)		mmio_set_8(addr,data)
#define setw(addr, data)		mmio_set_16(addr,data)
#define setl(addr, data)		mmio_set_32(addr,data)

#define readb(addr)			mmio_read_8(addr)
#define readw(addr)			mmio_read_16(addr)
#define readl(addr)			mmio_read_32(addr)

#if 0
#define writeb(data, addr)		({u8  *_v = (u8 *)addr;  mmio_write_8(_v, data);})
#define writew(data, addr)		({unsigned short *_v = (unsigned short *)addr; mmio_write_16(_v, data);})
#define writel(data, addr)		({unsigned int *_v = (unsigned int *)addr; mmio_write_32(_v, data);})
#else
#define writeb(addr, data)		mmio_write_8(addr,data)
#define writew(addr, data)		mmio_write_16(addr,data)
#define writel(addr, data)		mmio_write_32(addr,data)
#endif

#endif	// __TYPE_H__
