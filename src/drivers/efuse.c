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
#include <efuse.h>

static struct nx_efuse_reg  *g_efuse_reg
	= ((struct nx_efuse_reg *)PHY_BASEADDR_ECID_SECURE_MODULE);

int efuse_get_bootcfg(void)
{
	return mmio_read_32(&g_efuse_reg->boot_cfg);
}

int efuse_get_ecid(int index)
{
	return mmio_read_32(&g_efuse_reg->ecid[index]);
}
