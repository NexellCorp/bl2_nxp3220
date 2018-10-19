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
#include <sm5011.h>

#define SM5011_I2C_GPIO_GRP 			1
#define SM5011_I2C_SCL 				28
#define SM5011_I2C_SDA 				27
#define SM5011_I2C_SCL_ALT 			0
#define SM5011_I2C_SDA_ALT 			0

/************************************************
  * XXX Board (PMIC: )  - Reference 201X.XX.XX
  * ARM		: 1.0V
  * CORE	: 1.0V
  * DDR		: 1.35V
  * DDR_IO	: 1.35V
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
	data = ((1 << 1) | (1 << 0));
	sm5011_write(BUCK_4_CNTL_REG_3_ADDR, &data, 0xFF);
	sm5011_write(BUCK_3_CNTL_REG_3_ADDR, &data, 0xFF);
	sm5011_write(BUCK_6_CNTL_REG_3_ADDR, &data, 0xFF);

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
//					mVol, SM5011_BUCK_DATA_MASK);
					BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);
	/* Core Voltage (Default: 1.0V) */
//	mVol = sm5011_get_buck_vol(asv_get_core_vol());
	sm5011_buck_output_cntl(BUCK_5_CNTL_REG_1_ADDR, OUTPUT_ON,
//					mVol, SM5011_BUCK_DATA_MASK);
					BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDRC Voltage (Default: 1.0V) */
	sm5011_buck_output_cntl(BUCK_4_CNTL_REG_1_ADDR, OUTPUT_ON,
					BUCK_V_1000mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDR I/O Voltage (Default: 1.35V) */
	sm5011_buck_output_cntl(BUCK_3_CNTL_REG_1_ADDR, OUTPUT_ON,
					BUCK_V_1350mV, SM5011_BUCK_DATA_MASK, FALSE);

	/* DDR Device Voltage (Default: 1.35V) */
	sm5011_buck_output_cntl(BUCK_6_CNTL_REG_1_ADDR, OUTPUT_ON,
					BUCK_V_1350mV, SM5011_BUCK_DATA_MASK, FALSE);

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

	I2C_DEINIT();
}
