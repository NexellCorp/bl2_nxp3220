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
#ifndef __SM5011_H__
#define __SM5011_H__

/* @brief: Slave Address */
#define SM5011_SLAVE_ADDR		(0x8E >> 1)

/* @brief: Register List */
#define PWRON_REG			0x00
#define PWROFF_REG			0x01

#define CNTRL_REG_1_ADDR		0x10
#define CNTRL_REG_2_ADDR		0x11
#define CNTRL_REG_3_ADDR		0x12
#define MRSTBCNTL			0x13

#define BUCK_2_CNTL_REG_1_ADDR		0x2B
#define BUCK_2_CNTL_REG_2_ADDR		0x2C
#define BUCK_2_CNTL_REG_3_ADDR		0x2D

#define BUCK_3_CNTL_REG_1_ADDR		0x2E
#define BUCK_3_CNTL_REG_2_ADDR		0x2F
#define BUCK_3_CNTL_REG_3_ADDR		0x30

#define BUCK_4_CNTL_REG_1_ADDR		0x31
#define BUCK_4_CNTL_REG_2_ADDR		0x32
#define BUCK_4_CNTL_REG_3_ADDR		0x33

#define BUCK_5_CNTL_REG_1_ADDR		0x34
#define BUCK_5_CNTL_REG_2_ADDR		0x35
#define BUCK_5_CNTL_REG_3_ADDR		0x36

#define BUCK_6_CNTL_REG_1_ADDR		0x37
#define BUCK_6_CNTL_REG_2_ADDR		0x38
#define BUCK_6_CNTL_REG_3_ADDR		0x39

#define LDO_15_CNTL_REG_1_ADDR		0x58
#define LDO_15_CNTL_REG_2_ADDR		0x59

/*
 * BUCK voltage setting : 562.5mV + (12.5mV * DEC VALUE) MAX 2150mV
 * BUCK_2 OTP 1800mV = 562.5mV + (12.5mV *  99[0x63]) default ON
 * BUCK_3 OTP 1200mV = 562.5mV + (12.5mV *  51[0x33]) default OFF
 * BUCK_4 OTP 1600mV = 562.5mV + (12.5mV *  83[0x53]) default OFF
 * BUCK_5 OTP 0600mV = 562.5mV + (12.5mV *  03[0x03]) default OFF
 * BUCK_6 OTP 2000mV = 562.5mV + (12.5mV * 115[0x73]) default ON
*/

/*
 * LDO voltage setting : 800mV + (50mV * DEC VALUE) MAX 3350mV
 * LDO_15 OTP 2800mV = 800mV + (50mV *  40[0x28]) default ON
 */


#define BUCK_V_1000mV			0x23
#define BUCK_V_1050mV			0x27
#define BUCK_V_1100mV			0x2B
#define BUCK_V_1150mV			0x2F
#define BUCK_V_1200mV			0x33
#define BUCK_V_1250mV			0x37
#define BUCK_V_1300mV			0x3B
#define BUCK_V_1350mV			0x3F
#define BUCK_V_1400mV			0x43
#define BUCK_V_1450mV			0x47
#define BUCK_V_1500mV			0x4B
#define BUCK_V_1600mV			0x53
#define BUCK_V_1700mV			0x5B
#define BUCK_V_1800mV			0x63
#define BUCK_V_1900mV			0x6B
#define BUCK_V_2000mV			0x73
#define BUCK_V_2100mV			0x7B

#define LDO_V_2800mV			0x28

#define OUTPUT_ON			3
#define OUTPUT_OFF			0
#if 0
#define SM5011_OUTPUT_CNTL_LENGTH	3
#define SM5011_BUCK_DATA_LENGTH		7
#define SM5011_LDO_DATA_LENGTH		6
#else
#define SM5011_OUTPUT_CNTL_MASK		0xFF
#define SM5011_BUCK_DATA_MASK		0xFF
#define SM5011_LDO_DATA_MASK		0x3F
#endif

#define BUCK_UV_DEFAULT			562500
#define BUCK_UV_ONESTEP			12500

#define LDO_UV_DEFAULT			800000
#define LDO_UV_ONESTEP			50000

/* Function Define */
int sm5011_read(unsigned char reg, unsigned char *data, unsigned char mask);
int sm5011_write(unsigned char reg, unsigned char *data, unsigned char mask);

int sm5011_get_buck_vol(int mvol);
int sm5011_get_ldo_vol(int mvol);
int sm5011_read(unsigned char reg, unsigned char *data, unsigned char mask);
int sm5011_write(unsigned char reg, unsigned char *data, unsigned char mask);
int sm5011_buck_output_cntl(unsigned char addr, unsigned char cntl,
				unsigned char data, unsigned char mask,
				unsigned int reset);
int sm5011_ldo_output_cntl(unsigned char addr, unsigned char cntl,
				unsigned char data, unsigned char mask,
				unsigned int reset);
#endif	/* #ifndef __SM5011_H__ */
