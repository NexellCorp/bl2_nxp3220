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
#include <cmu.h>
#include <cmu_init_src.h>
#include <cmu_init_sys.h>
#include <cmu_init_usb.h>
#include <cmu_init_cpu.h>
#include <cmu_init_ddr.h>

#include <pll.h>

static int get_clk_type(int index)
{
	return ((index >> 28) & 0xF);
}

struct nx_clk_priv* get_clk_priv(unsigned int index)
{
	switch (get_clk_type(index)) {
		case CMU_NAME_SRC :
			return (&cmu_src[index & 0xffff]);
		case CMU_NAME_SYS :
			return (&cmu_sys[index & 0xffff]);
		case CMU_NAME_USB :
			return (&cmu_usb[index & 0xffff]);
		case CMU_NAME_HSIF:
//			printf("Unsupport CMU TYPE \r\n");
			break;
		case CMU_NAME_MM :
//			printf("Unsupport CMU TYPE \r\n");
			break;
		case CMU_NAME_CPU :
			return (&cmu_cpu[index & 0xffff]);
		case CMU_NAME_DDR :
			return (&cmu_ddr[index & 0xffff]);
		default:
//			printf("Unsupport CMU TYPE \r\n");
			break;
	}
	return (void *)-1;
}

static int get_clktop_base(void)
{
	return ((int)PHY_BASEADDR_CMU_SRC_MODULE);
}

static int src_get_clkrate(unsigned int index)
{
	struct nx_clk_priv *sys = get_clk_priv(index);
	unsigned int mux, div = 1;

	sys = get_clk_priv(index);
	while(1) {
		div *= ((mmio_read_32(&sys->reg->div_val[0]) & 0xFFFF) + 1);
		if (sys->p_id == -1)
			break;
		sys = get_clk_priv(sys->p_id);
	}

	mux = (mmio_read_32(&sys->reg->grp_clk_src) & 0xF);

	return (int)(get_pll_freq(mux) / div);
}

static unsigned int subdiv_get_clkrate(unsigned int index)
{
	struct nx_clk_priv *sys;
	struct nx_clk_cmu *reg;
	unsigned int mux, div = 1, subidx = 0;

	sys = get_clk_priv(index);
	reg = sys->reg;

	while(1) {
		div *= ((mmio_read_32(&reg->div_val[subidx++]) & 0xFFFF) + 1);
		if (sys->p_id == -1)
			break;
		sys = get_clk_priv(sys->p_id);
	}

	mux = (mmio_read_32(&sys->reg->grp_clk_src) & 0xF);

	/* @breif: cpu_backup_clk is not counsidered. */
	if (get_clk_type(index) == CMU_NAME_CPU) {
		if (mux == 0)
			mux += 2;
		else if (mux == 1) {
//			ERROR("Do not support 'cpu_backup_blk' \n");
			return FALSE;
		}
	} else	if (get_clk_type(index) == CMU_NAME_DDR) {
		mux += 3;
	}

	return (int)(get_pll_freq(mux) / div);
}

static long optimal_calc_div(unsigned int index, int req_freq)
{
	struct nx_clk_priv *sys = get_clk_priv(index);
	unsigned int div, calc_freq, rate;
	unsigned int subidx = ((sys->type != CMU_TYPE_ONLYGATE) ? sys->type : 0);

	div = ((mmio_read_32(&sys->reg->div_val[subidx]) & 0xFFFF) + 1);

	if (sys->p_id != -1)
		rate = subdiv_get_clkrate(sys->p_id);
	else
		rate = subdiv_get_clkrate(index);
	if (rate == FALSE)
		return rate;

	calc_freq = (rate * div);

	div = ((calc_freq + (req_freq - 1)) / req_freq);
	mmio_write_32(&sys->reg->div_val[subidx], ((div - 1) & 0xFFFF));

	return (calc_freq / div);
}

static long sel_optimal_calc_div(unsigned int index, int req_freq)
{
	struct nx_clk_priv *sys = get_clk_priv(index);
	struct nx_clk_priv *par = get_clk_priv(sys->p_id);

	int pll, mux;
	int s_div = 0, y_div = 0, div = 0;
	unsigned int opt_freq = 0xFFFFFFFF, calc_freq;
	int i;

	mux = mmio_read_32(&par->reg->grp_clk_src);

	for (i = 0; i < MAX_PLL_NUM - 1; i++) {
		pll = get_pll_freq(i);
		div = (pll / req_freq);

		calc_freq = (pll % div);
 		if (calc_freq < opt_freq) {
			opt_freq = calc_freq;
			mux = i;

			if (div > MAX_DIV) {
				s_div = MAX_DIV;
				y_div = (div - s_div);
			} else {
				s_div = div;
				y_div = 1;
			}
 		}
	}

	mmio_write_32(&par->reg->grp_clk_src, mux);
	mmio_write_32(&par->reg->div_val[0], ((s_div - 1) & 0xFFFF));
	mmio_write_32(&sys->reg->div_val[0], ((y_div - 1) & 0xFFFF));

	return (get_pll_freq(mux)/ s_div / y_div);
}

long get_optimal_div(unsigned int cur_freq,
			unsigned int req_freq, unsigned int *div)
{
	unsigned int cal_freq;
	unsigned int cal_div = 0;

	cal_div = (cur_freq / req_freq);

	if (cur_freq <= req_freq) {
		*div = 1;
		if (cur_freq < req_freq)
			return false;
	}

	if (cal_div > MAX_DIV) {
		*div = MAX_DIV;
		return false;
	} else {
		cal_freq = (cur_freq / cal_div);
		if (cal_freq > req_freq)
			cal_div += 1;
		*div = cal_div;
	}

	return true;
}

long get_src_mux(unsigned int index)
{
	struct nx_clk_priv *sys = get_clk_priv(index);

	return (mmio_read_32(&sys->reg->grp_clk_src) & 0xF);
}

unsigned long cmu_set_rate(unsigned int index, unsigned long freq)
{
	switch (get_clk_type(index)) {

		case CMU_NAME_SRC:
			/* Not Support */
			break;
		case CMU_NAME_SYS:
			return sel_optimal_calc_div(index, freq);
		case CMU_NAME_USB:
		case CMU_NAME_CPU:
		case CMU_NAME_DDR:
			return optimal_calc_div(index, freq);
	}

	return 0;
}

unsigned long cmu_get_rate(unsigned int index)
{
	switch (get_clk_type(index)) {

		case CMU_NAME_SRC:
		case CMU_NAME_SYS:
			/* @brief: only sys maindiv */
			return src_get_clkrate(index);
		case CMU_NAME_USB:
		case CMU_NAME_CPU:
		case CMU_NAME_DDR:
			return subdiv_get_clkrate(index);
	}

	return 0;
}

int cmu_clk_enable (int index, int enable)
{
	struct nx_clk_priv *sys = get_clk_priv(index);
	unsigned int reg_idx = (sys->clkenbit / 32);
	unsigned int reg_bit = (sys->clkenbit % 32);

	if (enable) {
		mmio_set_32(&sys->reg->grp_clkenb[reg_idx], (1 << reg_bit));
	} else {
		mmio_clear_32(&sys->reg->grp_clkenb[reg_idx], (1 << reg_bit));
		mmio_set_32(&sys->reg->grp_clkenb_clear[reg_idx], (1 << reg_bit));
	}

	return 0;
}

void cmu_clk_divstop(int index, unsigned int enable)
{
	struct nx_clk_topcmu *reg
		= (struct nx_clk_topcmu*)get_clktop_base();
	struct nx_clk_priv *sys = get_clk_priv(index);
	struct nx_clk_priv *src = get_clk_priv(sys->p_id);
	unsigned int reg_val;

	reg_val = mmio_read_32(&reg->clkgrp_clk_divstop[src->grp_idx/32]);
	reg_val &= ~(1 << (src->grp_idx % 32));

	if (enable)
		reg_val |= (1 << (src->grp_idx % 32));

	mmio_write_32(&reg->clkgrp_clk_divstop[src->grp_idx/32], reg_val);
}

void cmu_clkgrp_enable(int index, unsigned int enable)
{
	struct nx_clk_topcmu *top_reg
		= (struct nx_clk_topcmu*)get_clktop_base();
	struct nx_clk_priv *sys = get_clk_priv(index);
	struct nx_clk_priv *src = get_clk_priv(sys->p_id);

	unsigned int reg_idx = (sys->clkenbit / 32);
	unsigned int reg_bit = (sys->clkenbit % 32);
	unsigned int src_reg_idx = (src->clkenbit / 32);
	unsigned int src_bit = (src->clkenbit % 32);

	if (enable) {
		mmio_set_32(&sys->reg->grp_clkenb[reg_idx], (1 << reg_bit));
		mmio_set_32(&src->reg->grp_clkenb[src_reg_idx], (1 << src_bit));

		mmio_set_32(&top_reg->grp_clk_srcoff_clr[src->grp_idx/32],
			(1 << (src->grp_idx % 32)));
	 } else {
		mmio_clear_32(&sys->reg->grp_clkenb[reg_idx], (1 << reg_bit));
		mmio_clear_32(&src->reg->grp_clkenb[src_reg_idx], (1 << src_bit));

		mmio_clear_32(&top_reg->grp_clk_srcoff_set[src->grp_idx/32],
			(1 << (src->grp_idx % 32)));
	 }

}
