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
	unsigned int req_pll[4], div[4][2], mux;
	int ret = true;

	req_pll[0] = get_pre_pll_freq(3, g_nsih->clk[3].pll_pm,
						g_nsih->clk[3].pll_sk);
	req_pll[1] = get_pre_pll_freq(3, g_nsih->clk[3].pll_pm,
						g_nsih->clk[3].pll_sk);
	req_pll[2] = get_pre_pll_freq(2, g_nsih->clk[2].pll_pm,
						g_nsih->clk[2].pll_sk);
	mux = get_src_mux(SYS_0_AXI_CLK);
	req_pll[3] = get_pre_pll_freq(mux, g_nsih->clk[mux].pll_pm,
						g_nsih->clk[mux].pll_sk);
	get_optimal_div(req_pll[0], DDR_0_AXI_CLK_MAX,
		&div[0][0], &div[0][1]);					/* DDR.AXI_CLK.DYNAMIC_DIVIDER_VALUE	*/
	get_optimal_div(req_pll[1], DDR_0_APB_CLK_MAX,
		&div[1][0], &div[1][1]);					/* DDR.APB_CLK.DYNAMIC_DIVIDER_VALUE	*/
	get_optimal_div(req_pll[2], CPU_0_AXI_CLK_MAX,
		&div[2][0], &div[2][1]);					/* CPU.AXI.DYNAMIC_DIVIER_VALUE	*/
	get_optimal_div(req_pll[3], SYS_0_AXI_CLK_MAX,
		&div[3][0], &div[3][1]);					/* SYS.AXI.DYNAMIC_DIVIER_VALUE	*/

	base = (volatile unsigned int*)(PHY_BASEADDR_CMU_DDR_MODULE);
//	mmio_write_32((base + 0x200), 0);
	mmio_write_32((base + 0x260), 2-1);					// DDR
//	mmio_write_32((base + 0x400), 0);
	mmio_write_32((base + 0x460), 3-1);//div[0][0] - 1);			// DDR AXI
	mmio_write_32((base + 0x464), 2-1);//div[1][0] - 1);			// DDR APB
//	mmio_write_32((base + 0x660), 2-1);//div[1][0] - 1);			// PLL_DDR0
	base = (volatile unsigned int*)(PHY_BASEADDR_CMU_CPU_MODULE);
//	mmio_write_32((base + 0x260), 0x4-1); //div[2][0]);			// CPU AXI
	base = (volatile unsigned int*)(PHY_BASEADDR_CMU_SYS_MODULE);
//	mmio_write_32((base + 0x260), 0x2-1); //div[3][0]);			// SYS AXI

	if (true != pll_initialize())
		return false;

	return ret;
}
