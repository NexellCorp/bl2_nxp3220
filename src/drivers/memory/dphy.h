/* Copyright (C) 2018	Nexell Co., Ltd.
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
#ifndef __DPHY_H__
#define __DPHY_H__

#include <type.h>

/*
 * Uniquify DPHY(DDR PHY) - Technical User Guide v4.2
 * Correct offset value will change over time. (Make 2017.08.25)
 */

#define DPHY_BASE_ADDR			0x23091000

#define SCL_START			0x100
#define SCL_DATA_0			0x104
#define SCL_DATA_1			0x108
#define SCL_LATENCY			0x10C
#define SCL_RD_ADDR			0x110
#define SCL_RD_DATA			0x114
#define SCL_CONFIG_1			0x118
#define SCL_CONFIG_2			0x11C
#define SCL_CONFIG_3			0x16C

#define PHY_PAD_CTRL			0x120
#define PHY_DLL_RECALIB			0x124
#define PHY_DLL_ADRCTRL			0x128
#define PHY_DLL_TRIM_CLK		0x1A4
#define PHY_LANE_SEL			0x12C

#define PHY_DLL_TRIM_1			0x130
#define PHY_DLL_TRIM_2			0x134
#define PHY_DLL_TRIM_3			0x138
#define PHY_DLL_INCR_TRIM_1		0x164
#define PHY_DLL_INCR_TRIM_3		0x168

#define SCL_DCAPCLK_DLY			0x13C
#define SCL_MAIN_CLK_DELTA		0x140
#define PHY_ERR_INJECT			0x144

#define WRLVL_CTRL			0x148
#define WRLVL_AUTOINC_TRIM		0x14C
#define WRLVL_DYN_ODT			0x150

#define WRLVL_ON_OFF			0x154

#define UNQ_ANALOG_DLL_1		0x15C
#define UNQ_ANALOG_DLL_2		0x160
#define UNQ_ANALOG_DLL_3		0x190

#define PHY_DYNAMIC_RECALIB_1		0x158
#define PHY_DYNAMIC_RECALIB_2		0x1A8

#define UNIQUIFY_TSMC_IO_1		0x170
#define UNIQUIFY_IO_1			0x170
#define UNIQUIFY_IO_2			0x174
#define UNIQUIFY_IO_3			0x1CC
#define UNIQUIFY_IO_4			0x1D0

#define UNIQUIFY_ADDR_CTRL_LOOPBACK_1	0x178
#define UNIQUIFY_ADDR_CTRL_LOOPBACK_2	0x17C

#define PHY_SCL_START_ADDR		0x188
#define PHY_DLL_RISE_FALL		0x18C

#define DSCL_CNT			0x19C
#define IP_DQ_DQS_BITWISE_TRIM		0x194
#define OP_DQ_DM_DQS_BITWISE_TRIM	0x1A0

#define DYNAMIC_BIT_LVL			0x1AC
#define PHY_REV_CNTRL_REG		0x1B0
#define SCL_WINDOW_TRIM			0x1B4
#define SCL_GATE_TIMING			0x1E0
#define DISABLE_GATING_FOR_SCL		0x1B8
#define SCL_CONFIG_4			0x1BC

#define DYNAMIC_WRITE_BIT_LVL		0x1C0
#define DDR4_CONFIG_1			0x1C4
#define VREF_TRAINING			0x1C8

#define VREF_TRAINING_WINDOWS		0x1D4

#define DC_PARAM_OUT			0x1D8
#define DC_PARAM_SEL			0x1DC
#define BIT_LVL_MASK			0x1E4

#define DYNAMIC_IE_TIMER		0x1E8

/**/
// Register: PHY_DLL_TRIM_2
#define CLEAR_ALL_TRIMS			BIT(1)
// Register: SCL_START
#define SET_DDR_SCL_GO_DONE		BIT(28)
#define WRLVL				BIT(30)
// Register: DDR4_CONFIG_1
#define DDR4_SUPPORT			BIT(0)
#define SCL_START_ROW_ADDR		BIT(2)
#define POD_TERM			BIT(3)

// register - PHY_DLL_RECALIB

/* Function Define */
void reg_write_phy(int addr, int reg_value);
void reg_set_phy(int addr, int reg_value);
 int reg_read_phy(int addr);
void reg_clear_phy(int addr, int reg_value);

void phy_set_init_values(void);

 int hw_write_leveling(void);
 int hw_bit_leveling(void);

#endif // #ifndef __DPHY_H__
