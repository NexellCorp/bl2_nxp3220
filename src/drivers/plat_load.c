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
#include <libnx.h>
#include <memory.h>
#include <plat_load.h>

#define SIP_PLATFORM_LOAD		0x82000002

/* external function define */
int smc_call(unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3);

int plat_load(void)
{
	struct nx_bootmanager *pbm
		= ((struct nx_bootmanager *)RE_HEADER_BASEADDR);
	struct platform_info pi;

	pi.is_sss_f = pbm->bi.is_sss_f;
	pi.s_dev_addr = pbm->bi.dbi.s_device_addr;
	pi.n_dev_addr = pbm->bi.dbi.n_device_addr;
	pi.esr_func = enter_self_refresh;

	return smc_call(SIP_PLATFORM_LOAD, (int)&pi, 0, 0);
}
