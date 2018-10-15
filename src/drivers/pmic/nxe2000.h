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

#ifndef __NXE2000_H__
#define __NXE2000_H__

#define NXE2000_DEF_DDCx_VOL_MIN        600000				/* UINT = 1uV, 0.6V */
#define NXE2000_DEF_DDCx_VOL_MAX        3500000				/* UINT = 1uV, 3.5V */
#define NXE2000_DEF_DDCx_VOL_STEP       12500				/* UINT = 1uV, 12.5mV */

#define NXE2000_DEF_DDC1_VOL            1250000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define NXE2000_DEF_DDC2_VOL            1100000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.1V */
#define NXE2000_DEF_DDC3_VOL            3300000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define NXE2000_DEF_DDC4_VOL            1500000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */
#define NXE2000_DEF_DDC5_VOL            1500000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define NXE2000_REG_DC1VOL              0x36
#define NXE2000_REG_DC2VOL              0x37
#define NXE2000_REG_DC3VOL              0x38
#define NXE2000_REG_DC4VOL              0x39
#define NXE2000_REG_DC5VOL              0x3A

#define NXE2000_DEF_LDOx_VOL_MIN	900000				/* UINT = 1uV, 0.9V */
#define NXE2000_DEF_LDOx_VOL_MAX	3500000				/* UINT = 1uV, 3.5V */
#define NXE2000_DEF_LDOx_VOL_STEP	25000				/* UINT = 1uV, 25mV */

#define NXE2000_REG_LDO7VOL		0x52

#define I2C_ADDR_NXE2000                0x32

/* Function Define */
unsigned char nxe2000_get_ldo_step(int want_vol);
unsigned char nxe2000_get_dcdc_step(int want_vol);
 int nxe2000_write(char addr, char *pdata, int size);
 int nxe2000_read(char addr, char *pdata, int size);

#endif	// ifdef __NXE2000_H__
