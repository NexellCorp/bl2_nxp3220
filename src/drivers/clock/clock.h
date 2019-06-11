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
#ifndef __CLOCK_H__
#define __CLOCK_H__

/* cpu-specific maximum clock frequency value.*/
#define CPU_0_AXI_CLK_MAX				(400 * 1000 * 1000)
#define CPU_0_APB_CLK_MAX				(100 * 1000 * 1000)
#define CPU_0_AT_CLK_MAX				(200 * 1000 * 1000)
#define CPU_0_CNT_CLK_MAX				(200 * 1000 * 1000)
#define CPU_0_TS_CLK_MAX				(200 * 1000 * 1000)
#define CPU_0_DBGAPB_CLK_MAX				(100 * 1000 * 1000)

/* bus-specific maximum clock frequency value. */
#define SRC_SYS_0_AXI_CLK_MAX				(1000 * 1000 * 1000)
#define SYS_0_AXI_CLK_MAX				(333 * 1000 * 1000)
#define SYS_0_APB_CLK_MAX				(166 * 1000 * 1000)

/* ddr-specific maximum clock frequency value. */
#define DDR_0_AXI_CLK_MAX				(333 * 1000 * 1000)
#define DDR_0_APB_CLK_MAX				(166 * 1000 * 1000)
#define DDR_0_DIV_CLK_MAX				(625 * 1000 * 1000)

/* Clock Function Define */
int clock_initialize(void);
void clock_information(void);

#endif // _CLOCK_H__
