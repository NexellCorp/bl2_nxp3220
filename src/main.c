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
#define SET_GLOBAL_VARIABLES

#include <sysheader.h>
#include <clock.h>
#include <serial.h>
#include <pmic.h>
#include <pmu.h>
#include <memory.h>
#include <tz.h>
#include <alive.h>
#include <plat_load.h>
#include <main.h>

struct nx_vddpwr_reg *g_vddpwr_reg =
	((struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR);
struct nx_alive_reg *g_alive_reg =
	((struct nx_alive_reg *)PHY_BASEADDR_ALIVE);

#define NX_SUSPEND_SIGNATURE				(0x57505200)

unsigned int check_suspend_state(void)
{
	unsigned int reg_value;

	reg_value = mmio_read_32(&g_vddpwr_reg->new_scratch[0]);

	if (reg_value != NX_SUSPEND_SIGNATURE)
		return false;

	return true;
}

#define VDDPWRON_DDR_GPIO_NUM				11
#define DPHY_PAD_CTRL				(0x23091000 + 0x120)

void vddpwron_ddr_on(void)
{
	unsigned int reg_value = (1 << VDDPWRON_DDR_GPIO_NUM);

	mmio_set_32(&g_alive_reg->gpio_pullupenb_rst, reg_value);
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_rst, reg_value);
	mmio_set_32(&g_alive_reg->gpio_inputenb_rst, reg_value);

	mmio_set_32(&g_alive_reg->gpio_padout_rst, reg_value);			/* outpad - low  */
	mmio_set_32(&g_alive_reg->gpio_padout_set, reg_value);			/* outpad - high */
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_set, reg_value);		/* output enable */

	do {
		reg_value = mmio_read_32(DPHY_PAD_CTRL);
		reg_value = ((reg_value >> 31) & 0x1);
	} while (reg_value != 1);
}

void main(void)
{
	int serial_ch = g_nsih->serial_ch;
	int is_resume;

	is_resume = check_suspend_state();

#ifdef EARLY_SERIAL
	serial_init(serial_ch);
#endif
	pmic_initialize();

	pmu_initialize();

	tzpc_set_ddr();

	clock_initialize();

	serial_init(serial_ch);

	memory_initialize(is_resume);

	clock_information();

	plat_load();
}
