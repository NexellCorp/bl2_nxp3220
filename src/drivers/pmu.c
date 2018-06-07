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
#include <sysheader.h>

/* SMC Call ID */
#define PMU_BLK_PWRUP			0x82000003

/* external function define */
extern int smc_call(unsigned int r0, unsigned int r1,
			unsigned int r2, unsigned int r3);

int pmu_blk_pwrup(unsigned int domain)
{
	return smc_call(PMU_BLK_PWRUP, domain, TRUE, 0);
}

void pmu_initialize(void)
{
	int domain;

	for (domain = 0; domain < 2; domain++)
		pmu_blk_pwrup(domain);
}
