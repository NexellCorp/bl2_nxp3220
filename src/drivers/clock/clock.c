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
#include <clock.h>
#include <pll.h>
#include <cmu.h>

void clock_information(void)
{
	INFO("PLL0: %d  PLL1: %d  PLL_CPU: %d  PLL_DDR0: %d PLL_DDR1: %d \r\n\n",
			get_pll_freq(0), get_pll_freq(1),
			get_pll_freq(2), get_pll_freq(3),
			get_pll_freq(4));
	INFO("DDR_0_AXI_CLK: %d, DDR_0_APB_CLK: %d, CPU_0_AXI_CLK: %d, SYS_0_AXI_CLK: %d \r\n",
		cmu_get_rate(DDR_0_AXI_CLK), cmu_get_rate(DDR_0_APB_CLK),
		cmu_get_rate(CPU_0_AXI_CLK), cmu_get_rate(SYS_0_AXI_CLK));
}

int clock_initialize(void)
{
	volatile unsigned char *base;
	unsigned int cur_freq[5], freq;
	unsigned int index, div[6];

	for (index = 0; index < 5; index++)
		cur_freq[index] = get_pre_pll_freq(index,
			g_nsih->clk[index].pll_pm,
			g_nsih->clk[index].pll_sk);

	/*
	 * @brief: Do not change the DDR source clock without advice
	 * from the memory controller developer. (Fix: PLLDDR0)
	 */
	index = (get_src_mux(DDR_0_DDR_CLK) + PLLDDR0);
	get_optimal_div(cur_freq[index], DDR_0_AXI_CLK_MAX, &div[0]);
	freq = (cur_freq[index] / div[0]);
	get_optimal_div(freq, DDR_0_APB_CLK_MAX, &div[1]);
	get_optimal_div(freq, DDR_0_DIV_CLK_MAX, &div[2]);

	base = (volatile unsigned char*)(PHY_BASEADDR_CMU_DDR_MODULE);
	mmio_write_32((base + 0x200), 0);					/* CLKMUX_DDR[0].DDR */
	mmio_write_32((base + 0x260), 1-1);					/* DIVVAL_DDR[0].DDR */
	mmio_write_32((base + 0x400), 0);					/* CLKMUX_DDR[0].AXI */
	mmio_write_32((base + 0x460), div[0]-1);				/* DIVVAL_DDR[0].AXI */
	mmio_write_32((base + 0x464), div[1]-1);				/* DIVVAL_DDR[0].APB */
	mmio_write_32((base + 0x660), div[2]-1);				/* DIVVAL_PLL_DDR0_DIV[0].CLK	*/

	/*
	 * @brief: "CPU.ARM" clock and the sub clock are all used by
	 * dividing the "CPU.ARM" clock.
	 */
	index = PLLCPU;
	get_optimal_div(cur_freq[index], CPU_0_AXI_CLK_MAX, &div[0]);
	get_optimal_div(cur_freq[index], CPU_0_AT_CLK_MAX , &div[1]);
	get_optimal_div(cur_freq[index], CPU_0_CNT_CLK_MAX, &div[2]);
	get_optimal_div(cur_freq[index], CPU_0_TS_CLK_MAX , &div[3]);
	get_optimal_div(cur_freq[index], CPU_0_DBGAPB_CLK_MAX, &div[4]);
	get_optimal_div(cur_freq[index], CPU_0_APB_CLK_MAX, &div[5]);

	base = (volatile unsigned char*)(PHY_BASEADDR_CMU_CPU_MODULE);
	/* CLKMUX_CPU[0].ARM (0: PLLCPU, 1:CPU_BACKUP_0_CLK) */
	mmio_write_32((base + 0x200), 0);
	mmio_write_32((base + 0x260), 1-1);					/* DIVVAL_CPU[0].ARM	*/
	mmio_write_32((base + 0x264), div[0]-1);				/* DIVVAL_CPU[0].AXI	*/
	mmio_write_32((base + 0x268), div[1]-1);				/* DIVVAL_CPU[0].ATCLK	*/
	mmio_write_32((base + 0x26C), div[2]-1);				/* DIVVAL_CPU[0].CNTCLK	*/
	mmio_write_32((base + 0x270), div[3]-1);				/* DIVVAL_CPU[0].TSCLK	*/
	mmio_write_32((base + 0x274), div[4]-1);				/* DIVVAL_CPU[0].DBGAPB	*/
	mmio_write_32((base + 0x278), div[5]-1);				/* DIVVAL_CPU[0].APB	*/

	/*
	 * @brief: If you want to change this clock, SRC_CLK, SYS_CLK has a
	 * complex structure, please refer to the datasheet.
	 */
	/*
	 * @brief: In fact, since PLL0/1 is not changed, the current
	 * clock is fetched
	 */
	index = get_src_mux(SRC_SYS_0_AXI_CLK);
	cur_freq[index] = cmu_get_rate(index);
	get_optimal_div(cur_freq[index], SRC_SYS_0_AXI_CLK_MAX, &div[0]);
	freq = (cur_freq[index] / div[0]);
	get_optimal_div(freq , SYS_0_AXI_CLK_MAX, &div[1]);
	freq = (freq / div[1]);
	get_optimal_div(freq, SYS_0_APB_CLK_MAX, &div[2]);

	base = (volatile unsigned char*)(PHY_BASEADDR_CMU_SRC_MODULE);
	mmio_write_32((base + 0x200), div[0]-1);				/* DIVVAL_SRC_SYS[0].AXI*/
	base = (volatile unsigned char*)(PHY_BASEADDR_CMU_SYS_MODULE);
	mmio_write_32((base + 0x260), div[1]-1);				/* DIVVAL_SYS[0].AXI	*/
	mmio_write_32((base + 0x264), div[2]-1);				/* DIVVAL_SYS[0].APB	*/

	if (true != pll_initialize())
		return false;

	return true;
}
