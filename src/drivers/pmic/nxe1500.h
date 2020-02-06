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
#ifndef __NXE1500_H__
#define __NXE1500_H__

#define NXE1500_DEF_DDCx_VOL_MIN        600000				/* UINT = 1uV, 0.6V */
#define NXE1500_DEF_DDCx_VOL_MAX        3500000				/* UINT = 1uV, 3.5V */
#define NXE1500_DEF_DDCx_VOL_STEP       12500   			/* UINT = 1uV, 12.5mV */

// MM(DDC1)/CORE(DDC2) typycal voltage 1.1V/1.1V for MM, CORE LVCC
#define NXE1500_DEF_DDC1_VOL            1100000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define NXE1500_DEF_DDC2_VOL            1100000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.1V */
#define NXE1500_DEF_DDC3_VOL            1500000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define NXE1500_DEF_DDC4_VOL            1500000 			/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */
#define NXE1500_DEF_DDC5_VOL            1000000				/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define NXE1500_REG_SLPCNT		0xE

#define NXE1500_REG_DC1CTL1		0x2C
#define NXE1500_REG_DC1CTL2		0x2D
#define NXE1500_REG_DC2CTL1		0x2E
#define NXE1500_REG_DC2CTL2		0x2F
#define NXE1500_REG_DC3CTL1		0x30
#define NXE1500_REG_DC3CTL2		0x31
#define NXE1500_REG_DC4CTL1		0x32
#define NXE1500_REG_DC4CTL2		0x33

#define NXE1500_REG_DC1VOL              0x36
#define NXE1500_REG_DC2VOL              0x37
#define NXE1500_REG_DC3VOL              0x38
#define NXE1500_REG_DC4VOL              0x39

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
