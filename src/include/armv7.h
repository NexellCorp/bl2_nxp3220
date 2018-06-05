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
