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
#ifndef __PMS_H__
#define __PMS_H__

#define PLL_P_BITPOS			0
#define PLL_M_BITPOS			16
#define PLL_S_BITPOS			0
#define PLL_K_BITPOS			16
#define PLL_VAL_BITMASK			0xFFFF

#define PLL_SSCG_ENB_BITPOS		16

#define	_GET_PLLCFG0_2555X(_MHz, _val)							\
	_val = (unsigned int)((PLL2555_PMS_##_MHz##MHZ_P << PLL_P_BITPOS) |		\
		     (PLL2555_PMS_##_MHz##MHZ_M << PLL_M_BITPOS));

#define	_GET_PLLCFG0_2651X(_MHz, _val)							\
	_val = (unsigned int)((PLL2555_PMS_##_MHz##MHZ_P << PLL_P_BITPOS) |		\
		     (PLL2555_PMS_##_MHz##MHZ_M << PLL_M_BITPOS));

#define	_GET_PLLCFG1_2555X(_MHz, _val)							\
	_val = (unsigned int)((PLL2555_PMS_##_MHz##MHZ_S << PLL_S_BITPOS));

#define	_GET_PLLCFG1_2651X(_MHz, _val)							\
	_val = (unsigned int)((PLL2555_PMS_##_MHz##MHZ_S << PLL_S_BITPOS) |		\
		     (PLL2555_PMS_##_MHz##MHZ_K << PLL_K_BITPOS));

#define GET_PLLCFG0_2555X(STR, PM)	_GET_PLLCFG0_2555X (STR, PM)
#define GET_PLLCFG1_2555X(STR, S)	_GET_PLLCFG1_2555X (STR, S)
#define GET_PLLCFG0_2651X(STR, PM)	_GET_PLLCFG0_2651X (STR, PM)
#define GET_PLLCFG1_2651X(STR, SK)	_GET_PLLCFG1_2651X (STR, SK)

/* PMS for PLL2555X*/
#define PLL2555_PMS_1600MHZ_P		3
#define PLL2555_PMS_1600MHZ_M		200
#define PLL2555_PMS_1600MHZ_S		0
#define PLL2555_PMS_1600MHZ_K		0

#define PLL2555_PMS_1000MHZ_P		3
#define PLL2555_PMS_1000MHZ_M		250
#define PLL2555_PMS_1000MHZ_S		1
#define PLL2555_PMS_1000MHZ_K		0

#define PLL2555_PMS_800MHZ_P		3
#define PLL2555_PMS_800MHZ_M		200
#define PLL2555_PMS_800MHZ_S		1
#define PLL2555_PMS_800MHZ_K		0

#define PLL2555_PMS_400MHZ_P		3
#define PLL2555_PMS_400MHZ_M		200
#define PLL2555_PMS_400MHZ_S		2
#define PLL2555_PMS_400MHZ_K		0

#define PLL2555_PMS_333MHZ_P		6
#define PLL2555_PMS_333MHZ_M		333
#define PLL2555_PMS_333MHZ_S		2
#define PLL2555_PMS_333MHZ_K		0

#define PLL2555_PMS_125MHZ_P		3
#define PLL2555_PMS_125MHZ_M		250
#define PLL2555_PMS_125MHZ_S		4
#define PLL2555_PMS_125MHZ_K		0

/* PMS for PLL2651X */
#define PLL2555_PMS_1600MHZ_P		3
#define PLL2555_PMS_1600MHZ_M		200
#define PLL2555_PMS_1600MHZ_S		0
#define PLL2555_PMS_1600MHZ_K		0

#define PLL2651_PMS_1000MHZ_P		3
#define PLL2651_PMS_1000MHZ_M		250
#define PLL2651_PMS_1000MHZ_S		1
#define PLL2651_PMS_1000MHZ_K		0

#define PLL2651_PMS_800MHZ_P		3
#define PLL2651_PMS_800MHZ_M		200
#define PLL2651_PMS_800MHZ_S		1
#define PLL2651_PMS_800MHZ_K		0

#define PLL2651_PMS_630MHZ_P		4
#define PLL2651_PMS_630MHZ_M		210
#define PLL2651_PMS_630MHZ_S		1
#define PLL2651_PMS_630MHZ_K		0

#define PLL2651_PMS_400MHZ_P		3
#define PLL2651_PMS_400MHZ_M		200
#define PLL2651_PMS_400MHZ_S		2
#define PLL2651_PMS_400MHZ_K		0

#endif // #ifndef __PMS_H__
