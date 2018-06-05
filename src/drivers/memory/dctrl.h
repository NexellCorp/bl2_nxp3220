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
#ifndef __DCTRL_H__
#define __DCTRL_H__

/*
 * Uniquify DDR Controller(DDR Controller) - Controller Technical User Guide v3.0
 * Correct offset value will change over time. (Make 2017.08.25)
 */

#define DCTRL_BASE_ADDR			0x23090000

#define ARB_AGENT_SEL			0x00
#define MIN_LIMIT			0x04
#define RQST_PERIOD			0x08
#define MIN_CMD_ACPT			0x0C

#define MEM_START			0x10
#define MEM_WIDTH			0x50
#define MEM_CONFIG_1			0x14
#define MEM_CONFIG_2			0x18
#define MEM_CONFIG_3			0x1C
#define MEM_CONFIG_4			0x20
#define MEM_CONFIG_5			0x24
#define MEM_CONFIG_7			0x12C

#define REF_CONFIG			0x28

#define PWR_SAVE_ECC_CONFIG		0x2C
#define DLY_CONFIG_1			0x30
#define DLY_CONFIG_2			0x34
#define DLY_CONFIG_3			0x38
#define DLY_CONFIG_4			0x3C

#define ODT_CONFIG                      0x40
#define DATA_XFR_CONFIG                 0x44
#define HOST_CMD_ISSUE                  0x48
#define ODT_EN_CONFIG                   0x4C
#define MEM_WIDTH                       0x50
#define ZQC_CONFIG                      0x54
#define AUTO_SCL_CTRL                   0x58
#define AGENT_REF_PRIORITY              0x5C

#define RESET_DEASSERT_OK               0x7C
#define HOST_CMD_1_0                    0x80
#define HOST_CMD_1_1                    0x84
#define HOST_CMD_1_2                    0x88
#define HOST_CMD_1_3                    0x8C
#define HOST_CMD_1_4                    0x90
#define HOST_CMD_1_5                    0x94
#define HOST_CMD_1_6                    0x98
#define HOST_CMD_1_7                    0x9C
#define HOST_CMD_1_8                    0xA0
#define HOST_CMD_1_9                    0xA4
#define HOST_CMD_1_10                   0xA8
#define HOST_CMD_1_11                   0xAC
#define HOST_CMD_1_12                   0xB0
#define HOST_CMD_1_13                   0xB4
#define HOST_CMD_1_14                   0xB8
#define HOST_CMD_1_15                   0xBC

#define HOST_CMD_2_0                    0xC0
#define HOST_CMD_2_1                    0xC4
#define HOST_CMD_2_2                    0xC8
#define HOST_CMD_2_3                    0xCC
#define HOST_CMD_2_4                    0xD0
#define HOST_CMD_2_5                    0xD4
#define HOST_CMD_2_6                    0xD8
#define HOST_CMD_2_7                    0xDC
#define HOST_CMD_2_8                    0xE0
#define HOST_CMD_2_9                    0xE4
#define HOST_CMD_2_10                   0xE8
#define HOST_CMD_2_11                   0xEC
#define HOST_CMD_2_12                   0xF0
#define HOST_CMD_2_13                   0xF4
#define HOST_CMD_2_14                   0xF8
#define HOST_CMD_2_15                   0xFC

#define DDR4_DLY_CONFIG_1		0x130
#define DDR4_DLY_CONFIG_2		0x134
#define DDR4_DLY_CONFIG_3		0x138

#define MRR_DLY_CONFIG			0x124
#define MRR_RD_DATA			0x128
#define AXI_PORTN_HALT_REG		0x140
#define AXI_PORTN_IDLE_REG		0x144
#define AXI_PORTN_FIFO_REG		0x148
#define ZQC_CONFIG_1			0x188
#define ODT_CONFIG			0x40
#define ODT_EN_CONFIG			0x4C

#define AUTO_SCL_CTRL			0x58
#define AGENT_REF_PRIORITY		0x5C
#define DATA_XFR_CONFIG			0x44

#define HOST_CMD_ISSUE			0x48

#define HOST_CMD_1			0xBC
#define HOST_CMD_2			0xFC

#define RESET_DEASSERT_OK		0x7C

// Register - MEM_START: Address 7'h4
#define START_MEM_INIT			BIT(0)
#define MEM_INIT_DONE			BIT(1)



// Register - UNIQUIFY_IO_3: Address 7'h73

enum ctrl_state {
	DRAM_NORMAL_OPERATION	= 2,
	DRAM_POWER_DOWN		= 4,
	DRAM_SELF_REFRESH	= 5
};

#if defined(DDR3)
#define WAIT_SHIFT			12
struct host_cmd2 {
	volatile unsigned int op_code2		: 8;
	volatile unsigned int mr		: 3;
	volatile unsigned int reserved0		: 1;
	volatile unsigned int wait_cycle	: 9;
	volatile unsigned int reserved1		: 11;
};
#elif defined(DDR4)
#define WAIT_SHIFT			16
struct host_cmd2 {
	volatile unsigned int op_code2		: 7;
	volatile unsigned int mr		: 3;
	volatile unsigned int act_n		: 2;
	volatile unsigned int wait_cycle	: 10;
	volatile unsigned int reserved0		: 10;
};
#endif

struct host_cmd1 {
	volatile unsigned int   cmd		: 24;
	volatile unsigned int	op_code1	: 8;
};

union host_cmd {
	struct host_cmd1 cmd1;
	struct host_cmd2 cmd2;
	volatile unsigned int reg;
};

#if defined(DDR4)
/*************************************************************
  * Host Command Setting
  *
  * HOST_CMD_1
  * (MA[7:0] << 24) | (WE_N_S << 23) | (CAS_N_S << 22) | (RAS_N_S << 21) |
  * (CS_N_S << 13) |(CKE_S << 12) | (WE_N_F << 11) | (CAS_N_F << 10) |
  * (RAS_N_F << 9) | (CS_N_F << 1) | (CKE_F << 0)
  *
  * HOST_CMD_2 <-- DDR4
  * (Waiting Cycle << 16) | (ACT_N2 << 12) | (ACT_N1 << 11) | (BG[1:0] << 9)
  * (BA[1:0] << 7) | (MA[17], MA[13:8] << 0)
  * Waiting Cycle : Number of clock cycle to wait after issuing the command before
  * moving to the next HOST_CMD_1 and HOST_CMD_2 registers.
  ************************************************************/
//CLE_LOW?
#define CMD_CKE_LOW		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	 (0x00 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0xFF <<  1) | (0x00 <<  0))
#define CMD_CKE_LOW2		 (0x3 << 11)					// Sets ACT_N pin to HIGH for two cycles in HOST_CMD2 for CKE_LOW

// No Operation
#define CMD_IDLE_NOP		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) |(0x01 << 21)|	\
				 (0xFF << 13) |	 (0x01 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0xFF <<  1) | (0x01 <<  0))
#define CMD_IDLE_NOP2		 (0x3 << 11)					// Sets ACT_N pin to HIGH for two cycles in HOST_CMD2 for DES command

// Mode Register
#define CMD_LOAD_MODE		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	 (0x01 << 12) | (0x00 << 11) | (0x00 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x01 <<  0))
#define CMD_LOAD_MODE_CMD2	 (0x3 << 11)					// Sets ACT_N pin to HIGH for two cycles in HOST_CMD2 for MRS command

// ZQ Calibration
#define CMD_ZQ_CAL1		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	 (0x01 << 12) | (0x00 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0x00 <<  1) | (0x01 <<  0))

#define CMD_ZQ_CAL2		((0x00 << 13) | (0x01 << 12) | (0x01 << 11) | (0x0 << 3) |	\
				 (0x01 <<  2) | (0x00 <<  0))

// Self Refresh Entry (SRE)
#define CMD_SELF_REFE		((0x00 << 24) | (0x01 << 23) | (0x00 << 22) | (0x00 << 21) |	\
				 (0x00 << 13) |	(0x01 << 12) | (0x01 << 11) | (0x00 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x00 <<  0))
// Self Refresh Exit (SRX)
#define CMD_SELF_REFX		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) | (0x01 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0x00 <<  1) | (0x00 <<  0))

// Precharge All Banks (PREA)
#define CMD_PRE_ALL		((0x00 << 24) | (0x00 << 23) | (0x01 << 22) | (0x00 << 21) |	\
				 (0xFF << 13) | (0x01 << 12) | (0x00 << 11) | (0x01 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x01 <<  0))

#elif defined(DDR3)

/*************************************************************
  * Host Command Setting
  *
  * HOST_CMD_1
  * (MA[7:0] << 24) | (WE_N_S << 23) | (CAS_N_S << 22) | (RAS_N_S << 21) |
  * (CS_N_S << 13) |(CKE_S << 12) | (WE_N_F << 11) | (CAS_N_F << 10) |
  * (RAS_N_F << 9) | (CS_N_F << 1) | (CKE_F << 0)
  *
  * HOST_CMD_2 <--- DDR 2/3
  * (Waiting Cycle << 12) | (Rserved << 11) | (BA[2:0] << 8) | (MA[15:8] << 2)
  * Waiting Cycle : Number of clock cycle to wait after issuing the command before
  * moving to the next HOST_CMD_1 and HOST_CMD_2 registers.
  ************************************************************/
#define CMD_CKE_LOW		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	(0x00 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0xFF <<  1) | (0x00 <<  0))

// No Operation
#define CMD_IDLE_NOP		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) |(0x01 << 21)|	\
				 (0xFF << 13) |	(0x01 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0xFF <<  1) | (0x01 <<  0))

// Mode Register
#define CMD_LOAD_MODE		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	(0x01 << 12) | (0x00 << 11) | (0x00 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x01 <<  0))
// ZQ Calibration
#define CMD_ZQ_CAL		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	(0x01 << 12) | (0x00 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0x00 <<  1) | (0x01 <<  0))
// Refresh (REF)
#define CMD_REF			((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) |	(0x01 << 12) | (0x01 << 11) | (0x00 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x01 <<  0))

// Self Refresh Entry (SRE)
#define CMD_SELF_REFE		((0x00 << 24) | (0x01 << 23) | (0x00 << 22) | (0x00 << 21) |	\
				 (0x00 << 13) |	(0x01 << 12) | (0x01 << 11) | (0x00 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x00 <<  0))
// Self Refresh Exit (SRX)
#define CMD_SELF_REFX		((0x00 << 24) | (0x01 << 23) | (0x01 << 22) | (0x01 << 21) |	\
				 (0xFF << 13) | (0x01 << 12) | (0x01 << 11) | (0x01 << 10) |	\
				 (0x01 <<  9) | (0x00 <<  1) | (0x00 <<  0))

// Precharge All Banks (PREA)
#define CMD_PRE_ALL		((0x00 << 24) | (0x00 << 23) | (0x01 << 22) | (0x00 << 21) |	\
				 (0xFF << 13) | (0x01 << 12) | (0x00 << 11) | (0x01 << 10) |	\
				 (0x00 <<  9) | (0x00 <<  1) | (0x01 <<  0))

#endif

/* Function Define */

void host_cmd_load_mr(unsigned int offset, unsigned char mr_num,
					unsigned short mr_value, unsigned int wait_cycle);
void host_cmd_load(unsigned int offset, unsigned int cmd_high, unsigned int cmd_low,
				int wait_cycle);
void ctrl_set_init_values(void);

#endif // #ifndef __DCTRL_H__

