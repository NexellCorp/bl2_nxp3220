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
#include <gpio.h>

static struct nx_gpio_regs (*const g_gpio_regs)[1] =
	(struct nx_gpio_regs(*)[])(PHY_BASEADDR_GPIOA_NONSECURE_MODULE);

void gpio_set_outenb(int module, int pad, int enb)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (enb) {
		mmio_set_32(&gpio_reg->outenb, (1 << pad));
		mmio_clear_32(&gpio_reg->inputenb, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->outenb, (1 << pad));
		mmio_set_32(&gpio_reg->inputenb, (1 << pad));
	}
}

void gpio_set_output(int module, int pad, int val)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (val)
		mmio_set_32(&gpio_reg->out, (1 << pad));
	else
		mmio_clear_32(&gpio_reg->out, (1 << pad));
}

int gpio_get_pad(int module, int pad)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	return ((mmio_read_32(&gpio_reg->pad) >> pad) & 0x1);
}

void gpio_set_pullenb(int module, int pad, int enb)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module]);

	if (enb) {
		mmio_set_32(&gpio_reg->pullenb, (1 << pad));
		mmio_set_32(&gpio_reg->pullenb_disable_default, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->pullenb, (1 << pad));
		mmio_clear_32(&gpio_reg->pullenb_disable_default, (1 << pad));
	}
}

void gpio_set_pull(int module, int pad, int sel)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (sel) {
		mmio_set_32(&gpio_reg->pullsel, (1 << pad));
		mmio_set_32(&gpio_reg->pullsel_disable_default, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->pullsel, (1 << pad));
		mmio_clear_32(&gpio_reg->pullsel_disable_default, (1 << pad));
	}
}

void gpio_set_alt_function(int module, int pad, int alt)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	int reg_num = (pad >> 4);
	int alt_clear = ((pad & 0xF) * 2);
	int reg_value, reg_value_ex;

	reg_value = mmio_read_32(&gpio_reg->altfn[reg_num]);
	reg_value &= ~(0x3 << alt_clear);
	reg_value |= ((alt & 0x3) << alt_clear);

	reg_value_ex = mmio_read_32(&gpio_reg->altfn_ex);
	reg_value_ex &= ~(0x1 << (pad & 0xF));
	reg_value_ex |= (((alt >> 2) & 0x1) << pad);

	mmio_write_32(&gpio_reg->altfn_ex, reg_value_ex);
	mmio_write_32(&gpio_reg->altfn[reg_num], reg_value);
}

void nx_gpio_set_alt(const union nx_gpio_pad *pad)
{
	gpio_set_alt_function((int)pad->padi.group,
			(int)pad->padi.pad, (int)pad->padi.alternate);
}
