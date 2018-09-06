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
