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
#define SET_GLOBAL_VARIABLES

#include <sysheader.h>
#include <clock.h>
#include <serial.h>
#include <pmic.h>
#include <pmu.h>
#include <memory.h>
#include <tz.h>
#include <alive.h>
#include <plat_load.h>
#include <main.h>

struct nx_vddpwr_reg *g_vddpwr_reg =
	((struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR);
struct nx_alive_reg *g_alive_reg =
	((struct nx_alive_reg *)PHY_BASEADDR_ALIVE);

#define NX_SUSPEND_SIGNATURE				(0x57505200)

unsigned int check_suspend_state(void)
{
	unsigned int reg_value;

	reg_value = mmio_read_32(&g_vddpwr_reg->new_scratch[0]);

	if (reg_value != NX_SUSPEND_SIGNATURE)
		return FALSE;

	return TRUE;
}

#define VDDPWRON_DDR_GPIO_NUM				11
#define DPHY_PAD_CTRL					(0x23091000 + 0x120)

void vddpwron_ddr_on(void)
{
	unsigned int reg_value = (1 << VDDPWRON_DDR_GPIO_NUM);

	mmio_set_32(&g_alive_reg->gpio_pullupenb_rst, reg_value);
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_rst, reg_value);
	mmio_set_32(&g_alive_reg->gpio_inputenb_rst, reg_value);

	mmio_set_32(&g_alive_reg->gpio_inputenb_set, reg_value);

	do {
		reg_value = mmio_read_32(DPHY_PAD_CTRL);
		reg_value = ((reg_value >> 31) & 0x1);
	} while (reg_value != 1);
}

void build_information(void);
void main(void)
{
	int ret, serial_ch = g_nsih->serial_ch;
	unsigned int is_resume;

	is_resume = check_suspend_state();

#ifdef EARLY_SERIAL
	serial_init(serial_ch);
#endif
	pmic_initialize();

	pmu_initialize();

	tzpc_set_ddr();

	clock_initialize();

	serial_init(serial_ch);

	build_information();

	ret = memory_initialize(is_resume);

	clock_information();

	if (ret >= 0)
		plat_load();
	else {
		printf("ddr mem set error. system halt (ret.%d)!!!!\r\n", ret);
		while (1);
	}
}
