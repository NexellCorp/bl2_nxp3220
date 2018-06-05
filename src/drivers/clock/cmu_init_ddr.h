/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexcel and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#ifndef __CMU_INIT_DDR_H__
#define __CMU_INIT_DDR_H__

struct nx_clk_priv cmu_ddr[] = {
	CMU_INIT(DDR_0_DDR_CLK,				0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x200), 0, -1, 0),
	CMU_INIT(DDR_0_AXI_CLK,				0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x400), 0, -1, 1),
	CMU_INIT(TZASC_DDR_0_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x400), 1, DDR_0_AXI_CLK, 1),
	CMU_INIT(DDR_0_APB_CLK,				0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x400), 2, DDR_0_AXI_CLK, 1),
	CMU_INIT(SYSREG_DDR_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x400), 3, DDR_0_APB_CLK, 1),
	CMU_INIT(PLL_DDR0_DIV_0_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x600), 0, -1, 2),
	CMU_INIT(PLL_DDR1_DIV_0_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_DDR_MODULE + 0x800), 0, -1, 3),
};

#endif /* #ifndef __CMU_INIT_DDR_H__ */
