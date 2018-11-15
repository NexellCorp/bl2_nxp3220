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
#include <asv.h>
#include <nxe1500.h>

#define NXE1500_I2C_GPIO_GRP 			1
#define NXE1500_I2C_SCL 			28
#define NXE1500_I2C_SDA 			27
#define NXE1500_I2C_SCL_ALT 			0
#define NXE1500_I2C_SDA_ALT 			0

/************************************************
  * EVB Board (PMIC: )  - Reference 201X.XX.XX
  * ARM		: 1.0V
  * CORE	: 1.0V
  * DDR		: 1.5V
  * DDR_IO	: 1.5V
  ************************************************/
void pmic_board_init(void)
{
	char data = 0;

	I2C_INIT(NXE1500_I2C_GPIO_GRP, NXE1500_I2C_SCL, NXE1500_I2C_SDA,
			NXE1500_I2C_SCL_ALT, NXE1500_I2C_SDA_ALT);

	/* ARM Voltage (Default: 1.0V)	*/
	data = nxe1500_get_dcdc_step(NXE1500_DEF_DDC1_VOL);
	nxe1500_write(NXE1500_REG_DC1VOL, &data, 1);

	/* Core Voltage (Default: 1.0V) */
	data = nxe1500_get_dcdc_step(NXE1500_DEF_DDC2_VOL);
	nxe1500_write(NXE1500_REG_DC2VOL, &data, 1);

	/* DDR Device Voltage (Default: 1.5V)	*/
	data = nxe1500_get_dcdc_step(NXE1500_DEF_DDC3_VOL);
	nxe1500_write(NXE1500_REG_DC3VOL, &data, 1);

	/* DDRIO Voltage (Default: 1.5V)	*/
	data = nxe1500_get_dcdc_step(NXE1500_DEF_DDC4_VOL);
	nxe1500_write(NXE1500_REG_DC4VOL, &data, 1);

	/* DDRPLL, Device 1.8V Voltage (Default: 1.8V)	*/
	data = nxe1500_get_ldo_step(NXE1500_DEF_LDO2_VOL);
	nxe1500_write(NXE1500_REG_LDO2VOL, &data, 1);

	I2C_DEINIT();
}

void pmic_poweroff(void)
{
	char data;

	I2C_INIT(NXE1500_I2C_GPIO_GRP, NXE1500_I2C_SCL, NXE1500_I2C_SDA,
			NXE1500_I2C_SCL_ALT, NXE1500_I2C_SDA_ALT);
	/*
	 * brief: SLPCNT: Sleep Control Register
	 * [0] : SWPWROFF (1: 0FF, 0: Default)
	 */
	data = (0x1 << 0);
	nxe1500_write(NXE1500_REG_SLPCNT, &data, 1);

	I2C_DEINIT();
}
