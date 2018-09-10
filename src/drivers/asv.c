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
#include <sysheader.h>
#include <efuse.h>
#include <pll.h>
#include <asv.h>

/* @brief: To Do. crate the asv tables */
struct asv_tb_info core_v_tables[] = {
	[0] = {	.ids = 10, .ro = 110,
		.mhz = ASB_FREQ_MHZ,
		.uv  = { UV(1350),UV(1350), UV(1350), UV(1300),
			UV(1250), UV(1200), UV(1175), UV(1150),
			UV(1125), UV(1100), UV(1075), UV(1075),
			UV(1075) },
	},
};

static unsigned int mtol(unsigned int data, unsigned int bits)
{
	unsigned int result = 0;
	unsigned int i, mask = 1;

	for (i = 0; i < bits ; i++) {
		if (data & (1 << i))
			result |= (mask << (bits - i - 1));
	}
	return result;
}

int asv_get_core_vol(void)
{
	unsigned int ids, ro;
	unsigned int mvol, cpu_freq;
	unsigned int reg_value, index, i;

	/* @brief: default value */
	index = ASV_TABLE_SIZE - 1;
	mvol = core_v_tables[index].uv[FREQ_ARRAY_SIZE - 1];

	/* @breif: To Do. It's sample */
	reg_value = efuse_get_ecid(1);
	ids = mtol((reg_value >> 16) & 0xFF, 8);
	ro  = mtol((reg_value >> 24) & 0xFF, 8);

	for (i = 0; i < ASV_TABLE_SIZE; i++) {
		if ((core_v_tables[i].ids == ids)
			&& (core_v_tables[i].ro == ro)) {
			index = i;
			break;
		}
	}

	cpu_freq = get_pll_freq(PLLCPU);

	for (i = 0; i < FREQ_ARRAY_SIZE; i++) {
		if (core_v_tables[index].mhz[i] == cpu_freq) {
			mvol = (core_v_tables[index].uv[i] / 1000);
			break;
		}
	}

	return mvol;
}

/* @brief: To Do */
struct asv_tb_info arm_v_tables = {
	.mhz = ASB_FREQ_MHZ,
	.uv  = { UV(1000),UV(1000), UV(1000), UV(1000),
		UV(1000), UV(1000), UV(1000), UV(1000),
		UV(1000), UV(1000), UV(1000), UV(1000),
		UV(1000) }
};


int asv_get_arm_vol(void)
{
	unsigned int mvol, cpu_freq;
	unsigned int i;

	/* @brief: default value */
	mvol = core_v_tables->mhz[FREQ_ARRAY_SIZE - 1];

	cpu_freq = get_pll_freq(PLLCPU);
	for (i = 0; i < FREQ_ARRAY_SIZE; i++) {
		if (core_v_tables->mhz[i] == cpu_freq) {
			mvol = (core_v_tables->uv[i] / 1000);
			break;
		}
	}

	return mvol;
}
