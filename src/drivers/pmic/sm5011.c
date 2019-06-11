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

	return 1;
}

int sm5011_set_cntl(unsigned char addr, unsigned char value, unsigned char bitmask)
{
	unsigned char rcntl;

	sm5011_read(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK);

	rcntl &= ~bitmask;
	rcntl |= value;

	if (!sm5011_write(addr, &rcntl, 0xFF))
		return FALSE;

	return TRUE;
}

int sm5011_buck_output_cntl(unsigned char addr, unsigned char cntl,
				unsigned char data, unsigned char mask,
				unsigned int reset)
{
	unsigned char rcntl = 0;
	int bitmask = ~((0x3 << 6) | (0x3 << 0));

	sm5011_read(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK);

	if(cntl == OUTPUT_ON) {
		if ((reset != 0) && (rcntl & OUTPUT_ON)) {
			rcntl &= bitmask;
			rcntl |= OUTPUT_OFF;
			if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
				return FALSE;
			mdelay(5);
		}

		if (!sm5011_write((addr + 1), &data, mask))
			return FALSE;

		rcntl &= bitmask;
		rcntl |= OUTPUT_ON;
		if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;
	} else {
		rcntl &= bitmask;
		rcntl |= OUTPUT_OFF;
		if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;
	}

	return TRUE;
}

int sm5011_ldo_output_cntl(unsigned char addr, unsigned char cntl,
				unsigned char data, unsigned char mask,
				unsigned int reset)
{
	unsigned char rdata, rcntl = 0;
	int bitmask = ~((0x1 << 7) | (0x3 << 0));
	/* Low Power Mode */
	unsigned extra_mask = (0x3 << 6);

	sm5011_read(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK);

	if(cntl == OUTPUT_ON) {
		if ((reset != 0) && (rcntl & OUTPUT_ON)) {
			rcntl &= bitmask;
			rcntl |= OUTPUT_OFF;
			if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
				return FALSE;
			mdelay(5);
		}

		/* @brief: set the output voltage */
		sm5011_read((addr + 1), &rdata, (mask | extra_mask));
		rdata &= ~mask;
		rdata |= data;
		if (!sm5011_write((addr + 1), &rdata, (mask | extra_mask)))
			return FALSE;

		rcntl &= bitmask;
		rcntl |= OUTPUT_ON;
		if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;
	} else {
		rcntl &= bitmask;
		rcntl |= OUTPUT_OFF;
		if (!sm5011_write(addr, &rcntl, SM5011_OUTPUT_CNTL_MASK))
			return FALSE;
	}

	return TRUE;
}
