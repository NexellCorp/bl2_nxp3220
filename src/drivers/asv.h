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
#ifndef __ASV_H__
#define __ASV_H__

#define ASV_TABLE_SIZE			(1)	// 5
#define	FREQ_ARRAY_SIZE			(13)
#define	UV(v)				(v * 1000)


struct asv_tb_info {
	unsigned int ids;
	unsigned int ro;
	unsigned long mhz[FREQ_ARRAY_SIZE];
	unsigned long uv[FREQ_ARRAY_SIZE];
};

#define	ASB_FREQ_MHZ {	\
	[0] = 1600,	\
	[1] = 1500,	\
	[2] = 1400,	\
	[3] = 1300,	\
	[4] = 1200,	\
	[5] = 1100,	\
	[6] = 1000,	\
	[7] =  900,	\
	[8] =  800,	\
	[9] =  700,	\
	[10] =  600,	\
	[11] =  500,	\
	[12] =  400,	\
	}

/* Function Define */
 int asv_get_core_vol(void);
 int asv_get_arm_vol(void);

#endif /* #ifndef __ASV_H__ */
