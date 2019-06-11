/* Copyright (C) 2018  Nexell Co., Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the Nexell nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL,SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
