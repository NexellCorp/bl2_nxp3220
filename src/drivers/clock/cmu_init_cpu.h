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
#ifndef __CMU_INIT_CPU_H__
#define __CMU_INIT_CPU_H__

struct nx_clk_priv cmu_cpu[] = {
	CMU_INIT(CPU_0_ARM_CLK,				0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 0, -1, 0),
	CMU_INIT(BLK_CPU_BIST_CLK_0_800MHZ_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 1, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_AXI_CLK,				0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 2, CPU_0_ARM_CLK, 0),
	CMU_INIT(AXIM_CPU_0_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 3, CPU_0_AXI_CLK, 0),
	CMU_INIT(CPU_0_ATCLK_CLK,			0, CMU_TYPE_SUBDIV1,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 4, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_CNTCLK_CLK,			0, CMU_TYPE_SUBDIV2,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 5, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_TSCLK_CLK,			0, CMU_TYPE_SUBDIV3,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 6, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_DBGAPB_CLK,			0, CMU_TYPE_SUBDIV4,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 7, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_APB_CLK,				0, CMU_TYPE_SUBDIV5,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 8, CPU_0_ARM_CLK, 0),
	CMU_INIT(SYSREG_CPU_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 9, CPU_0_APB_CLK, 0),
	CMU_INIT(AXIM_CPU_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 10, CPU_0_APB_CLK, 0),
	CMU_INIT(PLL_CPU_DIV_0_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x400), 0, -1, 1),
	CMU_INIT(HPM_CPU_0_HPM_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x600), 0, -1, 2),

};

#endif /* #ifndef __CMU_INIT_CPU_H__ */
