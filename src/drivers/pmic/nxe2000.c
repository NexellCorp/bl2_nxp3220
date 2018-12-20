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
#include <nxe2000.h>

unsigned char nxe2000_get_ldo_step(int want_vol)
{
    unsigned int vol_step = 0;
    unsigned int temp = 0;

    if (want_vol < NXE2000_DEF_LDOx_VOL_MIN)
        want_vol = NXE2000_DEF_LDOx_VOL_MIN;
    else if (want_vol > NXE2000_DEF_LDOx_VOL_MAX)
        want_vol = NXE2000_DEF_LDOx_VOL_MAX;
    temp = (want_vol - NXE2000_DEF_LDOx_VOL_MIN);

    vol_step    = (temp / NXE2000_DEF_LDOx_VOL_STEP);

    return    (unsigned char)(vol_step & 0x7F);
}

unsigned char nxe2000_get_dcdc_step(int want_vol)
{
	unsigned int vol_step = 0;

	if (want_vol < NXE2000_DEF_DDCx_VOL_MIN) {
		want_vol = NXE2000_DEF_DDCx_VOL_MIN;
	} else if (want_vol > NXE2000_DEF_DDCx_VOL_MAX) {
		want_vol = NXE2000_DEF_DDCx_VOL_MAX;
	}

	vol_step = (want_vol - NXE2000_DEF_DDCx_VOL_MIN +
		    NXE2000_DEF_DDCx_VOL_STEP - 1) /
		   NXE2000_DEF_DDCx_VOL_STEP;

	return (unsigned char)(vol_step & 0xFF);
}

int nxe2000_write(char addr, char *pdata, int size)
{
	return i2c_gpio_write(I2C_ADDR_NXE2000, addr, pdata, size);
}

int nxe2000_read(char addr, char *pdata, int size)
{
	return i2c_gpio_read(I2C_ADDR_NXE2000, addr, pdata, size);
}

/* Do not use it yet. The reason is that code is not generally available. */
#if 0

#if 0
inline void nxe2000(void)
{
	char data[4];

	/* nxe200 i2c initialize for core & arm & power. */
	i2c_gpio_init(NXE2000_I2C_GPIO_GRP, NXE2000_I2C_SCL, NXE2000_I2C_SDA,
			NXE2000_I2C_SCL_ALT, NXE2000_I2C_SDA_ALT);

	/* arm voltage change */// 1.25V
	data[0] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC1_VOL);
	nxe2000_write(NXE2000_REG_DC1VOL, data, 1);

	/* core voltage change */ // 1.2V
	data[1] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC2_VOL);
	nxe2000_write(NXE2000_REG_DC2VOL, data, 1);

	/* ddr3 voltage change */ // 1.5V
	data[2] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC4_VOL);
	nxe2000_write(NXE2000_REG_DC4VOL, data, 1);
	/* ddr3 IO voltage change */ // 1.5V
	data[3] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC5_VOL);
	nxe2000_write(NXE2000_REG_DC5VOL, data, 1);

	return;
}
#else
inline void nxe2000(void)
{
	char data[4];

	/* nxe200 i2c initialize for core & arm & power. */
	i2c_gpio_init(NXE2000_I2C_GPIO_GRP, NXE2000_I2C_SCL, NXE2000_I2C_SDA,
			NXE2000_I2C_SCL_ALT, NXE2000_I2C_SDA_ALT);

#if (ARM_VOLTAGE_CONTROL_SKIP == 0)
	/* arm voltage change */// 1.25V
	data[0] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC1_VOL);
	nxe2000_write(NXE2000_REG_DC1VOL, data, 1);
#endif

#if defined(SVT_PMIC_INIT) && (MP8845_SYSCCNTL2_CORE == 0)
	/* core voltage change */ // 1.2V
	data[1] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC2_VOL);
	nxe2000_write(NXE2000_REG_DC2VOL, data, 1);
#endif

	/* ddr3 voltage change */ // 1.5V
	data[2] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC4_VOL);
	nxe2000_write(NXE2000_REG_DC4VOL, data, 1);
	/* ddr3 IO voltage change */ // 1.5V
	data[3] = nxe2000_get_dcdc_step(NXE2000_DEF_DDC5_VOL);
	nxe2000_write(NXE2000_REG_DC5VOL, data, 1);

	return;
}
#endif
#endif

#endif // if defined(NXE2000_PMIC_ENABLE)
