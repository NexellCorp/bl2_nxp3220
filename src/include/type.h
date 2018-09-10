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
