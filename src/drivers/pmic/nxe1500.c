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