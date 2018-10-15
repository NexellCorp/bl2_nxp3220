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
#ifndef __NXE1500_H__
#define __NXE1500_H__

#define NXE1500_DEF_DDCx_VOL_MIN        600000				/* UINT = 1uV, 0.6V */
#define NXE1500_DEF_DDCx_VOL_MAX        3500000				/* UINT = 1uV, 3.5V */
#define NXE1500_DEF_DDCx_VOL_STEP       12500   			/* UINT = 1uV, 12.5mV */

#define NXE1500_DEF_DDC1_VOL            1000000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define NXE1500_DEF_DDC2_VOL            1000000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.1V */
#define NXE1500_DEF_DDC3_VOL            1000000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define NXE1500_DEF_DDC4_VOL            1500000 			/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */
#define NXE1500_DEF_DDC5_VOL            1500000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define NXE1500_REG_DC1VOL              0x36
#define NXE1500_REG_DC2VOL              0x37
#define NXE1500_REG_DC3VOL              0x38
#define NXE1500_REG_DC4VOL              0x39
#define NXE1500_REG_DC5VOL              0x3A

#define NXE1500_DEF_LDOx_VOL_MIN	900000				/* UINT = 1uV, 0.9V */
#define NXE1500_DEF_LDOx_VOL_MAX	3500000				/* UINT = 1uV, 3.5V */
#define NXE1500_DEF_LDOx_VOL_STEP	25000				/* UINT = 1uV, 25mV */

#define NXE1500_DEF_LDO2_VOL            1800000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define NXE1500_REG_LDO1VOL		0x4C
#define NXE1500_REG_LDO2VOL		0x4D
#define NXE1500_REG_LDO3VOL		0x4E
#define NXE1500_REG_LDO4VOL		0x4F
#define NXE1500_REG_LDO5VOL		0x50
#define NXE1500_REG_LDO6VOL		0x51
#define NXE1500_REG_LDO7VOL		0x52

#define I2C_ADDR_NXE1500                0x33

/* Function Define */
unsigned char nxe1500_get_ldo_step(int want_vol);
unsigned char nxe1500_get_dcdc_step(int want_vol);

 int nxe1500_write(char addr, char *pdata, int size);
 int nxe1500_read(char addr, char *pdata, int size);

#endif	// ifdef __NXE1500_H__