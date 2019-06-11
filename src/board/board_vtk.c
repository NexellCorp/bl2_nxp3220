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

#if defined(PMIC_SM5011)
#include <sm5011.h>

#define SM5011_I2C_GPIO_GRP	1
#define SM5011_I2C_SCL		28
#define SM5011_I2C_SDA		27
#define SM5011_I2C_SCL_ALT	0
#define SM5011_I2C_SDA_ALT	0

/************************************************
  * XXX Board (PMIC: )  - Reference 201X.XX.XX
  * ARM		: 1.0V
  * CORE	: 1.0V
  * DDR		: 1.5V
  * DDR_IO	: 1.5V
  ************************************************/
void pmic_board_init(void)
{
	unsigned char data = 0;
	unsigned char mVol = 0;

	I2C_INIT(SM5011_I2C_GPIO_GRP, SM5011_I2C_SCL, SM5011_I2C_SDA,
			SM5011_I2C_SCL_ALT, SM5011_I2C_SDA_ALT);
	/*
	 * @brief: BUCKxCNTL3 - Control Register 3 for BUCK
	 * [3] : DVSRAMPBx (00: 3.125mV/us, 01: 6.25mV/us, 10: 12.5mV/us, 11:25mV/us)
	 * [2] : ENRAMPBxDOWN (0: Disable, 1:Enable with 3.125mV/us)
	 * [1] : BUCKxADISEN (Active-Discharge 0: Disable, 1: Enabled)
	 * [0] : BUCKxMODE (0: Auto-Mode, 1: Forced PWM)
	 */
#ifdef CONFIG_ENABLE_FPWM
	data = ((1 << 1) | (1 << 0));
	sm5011_write(BUCK_4_CNTL_REG_3_ADDR, &data, 0xFF);
	sm5011_write(BUCK_3_CNTL_REG_3_ADDR, &data, 0xFF);
	sm5011_write(BUCK_6_CNTL_REG_3_ADDR, &data, 0xFF);
#endif
	/*
	 * @brief: Manual Reset Control Register
	 * [3] : ENPMICOFF2ON	(0: Do not Reboot, 1)
	 * [2] : ENnRESETOFF2ON (0: Do not toggle th nRESET pin)
	 * [1] : KEYOPTION (0: One-Key, 1: Dual-Key)
	 * [0] : ENMRSTB (0: Disable Manual Reset, 1: Enable the Reset Function)
	 */
	data = ((1 << 3) | (1 << 0));
	sm5011_write(MRSTBCNTL, &data, 0xFF);

	/* ARM Voltage (Default: 1.0V)	*/
//	mVol = sm5011_get_buck_vol(asv_get_arm_vol());
	sm5011_buck_output_cntl(BUCK_2_CNTL_REG_1_ADDR, OUTPUT_ON,
//				mVol, SM5011_BUCK_DATA_MASK);
				BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);
	/* Core Voltage (Default: 1.0V) */
//	mVol = sm5011_get_buck_vol(asv_get_core_vol());
	sm5011_buck_output_cntl(BUCK_5_CNTL_REG_1_ADDR, OUTPUT_ON,
//				mVol, SM5011_BUCK_DATA_MASK);
				BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDRC Voltage (Default: 1.0V) */
	sm5011_buck_output_cntl(BUCK_4_CNTL_REG_1_ADDR, OUTPUT_ON,
				BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDR I/O Voltage (Default: 1.5V) */
	sm5011_buck_output_cntl(BUCK_3_CNTL_REG_1_ADDR, OUTPUT_ON,
				BUCK_V_1500mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDR Device Voltage (Default: 1.5V) */
	sm5011_buck_output_cntl(BUCK_6_CNTL_REG_1_ADDR, OUTPUT_ON,
				BUCK_V_1500mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDRC PLL Voltage (Default: 1.8V) */
	mVol = sm5011_get_ldo_vol(1800);
	sm5011_ldo_output_cntl(LDO_15_CNTL_REG_1_ADDR, OUTPUT_ON,
				mVol, SM5011_LDO_DATA_MASK, TRUE);
}

void pmic_poweroff(void)
{
	unsigned char data;

	I2C_INIT(SM5011_I2C_GPIO_GRP, SM5011_I2C_SCL, SM5011_I2C_SDA,
			SM5011_I2C_SCL_ALT, SM5011_I2C_SDA_ALT);

	/* @breif: CNTRL2 - [4]: GLOBALSHDN Bit = 1 (Enable : Do Shutdown) */
	data = (1 << 4);
	sm5011_write(CNTRL_REG_2_ADDR, &data, 0xFF);

	/* @breif: PWROFF_REG - [0] GLOBALSHDN Bit = 1 */
	data = (1 << 0);
	sm5011_write(PWROFF_REG, &data, 0xFF);

	I2C_DEINIT();
}

#elif defined(PMIC_NXE1500)
#include <nxe1500.h>

#define NXE1500_I2C_GPIO_GRP	1
#define NXE1500_I2C_SCL		28
#define NXE1500_I2C_SDA		27
#define NXE1500_I2C_SCL_ALT	0
#define NXE1500_I2C_SDA_ALT	0

/************************************************
  * VTK Board (PMIC: )  - Reference 201X.XX.XX
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

#endif
