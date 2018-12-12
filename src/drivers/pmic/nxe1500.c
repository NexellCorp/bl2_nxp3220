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
#include <i2c_gpio.h>
#include <nxe1500.h>

unsigned char nxe1500_get_ldo_step(int want_vol)
{
    unsigned int vol_step = 0;
    unsigned int temp = 0;

    if (want_vol < NXE1500_DEF_LDOx_VOL_MIN)
        want_vol = NXE1500_DEF_LDOx_VOL_MIN;
    else if (want_vol > NXE1500_DEF_LDOx_VOL_MAX)
        want_vol = NXE1500_DEF_LDOx_VOL_MAX;
    temp = (want_vol - NXE1500_DEF_LDOx_VOL_MIN);

    vol_step    = (temp / NXE1500_DEF_LDOx_VOL_STEP);

    return    (unsigned char)(vol_step & 0x7F);
}

unsigned char nxe1500_get_dcdc_step(int want_vol)
{
	unsigned int vol_step = 0;

	if (want_vol < NXE1500_DEF_DDCx_VOL_MIN) {
		want_vol = NXE1500_DEF_DDCx_VOL_MIN;
	} else if (want_vol > NXE1500_DEF_DDCx_VOL_MAX) {
		want_vol = NXE1500_DEF_DDCx_VOL_MAX;
	}

	vol_step = (want_vol - NXE1500_DEF_DDCx_VOL_MIN +
			NXE1500_DEF_DDCx_VOL_STEP - 1) /
		NXE1500_DEF_DDCx_VOL_STEP;

	return (unsigned char)(vol_step & 0xFF);
}

int nxe1500_write(char addr, char *pdata, int size)
{
	return i2c_gpio_write(I2C_ADDR_NXE1500, addr, pdata, size);
}

int nxe1500_read(char addr, char *pdata, int size)
{
	return i2c_gpio_read(I2C_ADDR_NXE1500, addr, pdata, size);
}

int nxe1500_dcdc_enable(char addr, int enable)
{
	char data = 0;

	nxe1500_read(addr, &data, 1);

	if (enable)
		data |= (1 << 0);
	else
		data &= ~(1 << 0);

	nxe1500_write(addr, &data, 1);

	return data;
}