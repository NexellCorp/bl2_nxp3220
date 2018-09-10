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
#include <tz.h>

#define TZPC_ACCESS_DDR			0x82000004

/* external function define */
extern int smc_call(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);

void tzpc_set_ddr(void)
{
	smc_call(TZPC_ACCESS_DDR, 0, 0, 0);
}
