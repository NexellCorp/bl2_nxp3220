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
#include <i2c_gpio.h>
#include <sm5011.h>

int sm5011_get_buck_vol(int mvol)
{
	int uvol = (mvol * 1000);
	int def_uv = BUCK_UV_DEFAULT;
	int cal_uv, step_uv = BUCK_UV_ONESTEP;

	cal_uv = uvol - def_uv;
	if (cal_uv < 0)
		return -1;
	else if (cal_uv == 0)
		return 0;

	return (cal_uv /= step_uv);
}

int sm5011_get_ldo_vol(int mvol)
{
	int uvol = (mvol * 1000);
	int def_uv = LDO_UV_DEFAULT;
	int cal_uv, step_uv = LDO_UV_ONESTEP;

	cal_uv = uvol - def_uv;
	if (cal_uv < 0)
		return -1;
	else if (cal_uv == 0)
		return 0;

	return (cal_uv /= step_uv);
}

int sm5011_read(unsigned char reg, unsigned char *data, unsigned char mask)
{
	if (!i2c_gpio_read(SM5011_SLAVE_ADDR, reg, (char*)data, 1))
		return 0;

	return (data[0] & mask);
}

int sm5011_write(unsigned char reg, unsigned char *data, unsigned char mask)
{
	char value;

	value = (data[0] & mask);

	if (!i2c_gpio_write(SM5011_SLAVE_ADDR, reg, &value, 1))
		return 0;

	return value;
}

int sm5011_sboot_output_cntl(unsigned char addr, unsigned char cntl,
				unsigned char data, unsigned char mask)
{
	unsigned char c_data, rdata;

	if(cntl == OUTPUT_ON) {
		sm5011_read(addr, &rdata, SM5011_OUTPUT_CNTL_MASK);
		if(rdata & OUTPUT_ON) {
			rdata = OUTPUT_OFF;
			if(!sm5011_write(addr, &rdata, SM5011_OUTPUT_CNTL_MASK))
				return FALSE;
			mdelay(5);
		}

		c_data = OUTPUT_ON;
		if(!sm5011_write((addr + 1), &data, mask))
			return FALSE;

		if(!sm5011_write(addr, &c_data, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;

	} else {
		c_data = OUTPUT_OFF;
		if(!sm5011_write(addr, &c_data, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;
	}

	return TRUE;
}
