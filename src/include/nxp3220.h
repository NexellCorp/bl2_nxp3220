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
#ifndef __NXP3220_H__
#define __NXP3220_H__

#define USE_NSIH_HEADER			1

#define SRAM_BASEADDR			0xFFFF0000

#define SRAM_BL2_BASEADDR		(SRAM_BASEADDR + 0x8000)
#define SRAM_SIZE			(64 *1024)
#define BL2_SRAM_SIZE			(32 * 1024)
#define	BL2_STACK_SIZE			0x400

/* SDRAM in Boot Loader 0 File Format */
#define HEADER_BASEADDR			(SRAM_BASEADDR + SRAM_SIZE - (1 * 1024))
#define HASHSIGN_BASEADDR		(HEADER_BASEADDR   + 256)
#define BOOTKEY_BASEADDR		(HASHSIGN_BASEADDR + 256)
#define USERKEY_BASEADDR		(BOOTKEY_BASEADDR  + 256)

#define RE_HEADER_BASEADDR		USERKEY_BASEADDR

#endif /* #ifndef __NXP3220_H__ */