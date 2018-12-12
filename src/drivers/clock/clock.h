/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
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
