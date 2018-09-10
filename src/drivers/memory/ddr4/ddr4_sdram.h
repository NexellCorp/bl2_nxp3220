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
#ifndef __DDR4_SDRAM_H__
#define __DDR4_SDRAM_H__

typedef enum
{
	SDRAM_MODE_REG_MR0      = 0,
	SDRAM_MODE_REG_MR1      = 1,
	SDRAM_MODE_REG_MR2      = 2,
	SDRAM_MODE_REG_MR3      = 3,
	SDRAM_MODE_REG_MR4      = 4,
	SDRAM_MODE_REG_MR5      = 5,
	SDRAM_MODE_REG_MR6      = 6,
	SDRAM_MODE_REG_MAX_MRn  = 0xFF
} SDRAM_MODE_REG;

struct DDR4_SDRAM_MR0 {
	volatile unsigned short BL		:2;     // 0: Burst Length ( 00: Fixed BL8, 01: 4 or 8 (A12), 10: Fixed BC4, 11: Reserved)
	volatile unsigned short CL0		:1;     // 2: CAS Latency 0 bit (0000: Reserved, 0010: 5, 0100: 6, 0110: 7, 1000: 8, 1010: 9, 1100:10, 1110:11, 0001:12, 0011:13
	volatile unsigned short BT		:1;     // 3: READ Burst Type (0: Sequential(nibble), 1: Interleaved)
	volatile unsigned short CL1		:3;     // 4: CAS Latency 1 bit  (See Table 2)
	volatile unsigned short TM		:1;     // 7: TM (0: Normal, 1:Test)
	volatile unsigned short DLL		:1;     // 8: DLL Reset ( 0: No, 1: Yes)
	volatile unsigned short WR		:3;     // 9: Write Recovery (000: Reserved, 001: 5, 010: 6, 011: 7, 100: 8, 101: 10, 110: 12, 111: 14) (See Table 1)
	volatile unsigned short RFU0		:6;     // 12 ~ 17: RFU (must be programmed to 0 during MRS)
};
struct DDR4_SDRAM_MR1 {
	volatile unsigned short DLL		:1;     // 0: DLL Enable (0: enable, 1: disable)
	volatile unsigned short ODS		:2;     // 1 ~ 2: Output Drive Strenghth 2 bit (00: RZQ/6(40ohm), 01: RZQ/7(34ohm), 10, 11: reserved)
	volatile unsigned short nAL		:2;     // 3 ~ 4: Additive Latency	(00: disable, 01: AL=CL-1, 10: AL=CL-2, 11: Reserved)
	volatile unsigned short RFU0		:2;     // 5 ~ 6: RFU (must be programmed to 0 during MRS)
	volatile unsigned short WL		:1;     // 7: Write Levelization (0: disable, 1: enable)
	volatile unsigned short RTT_Nom		:3;     // 8 ~ 10: RTT_Normal  (See Table 3)
	volatile unsigned short TDQS		:1;     // 11: TDQS (0: disable, 1: enable)
	volatile unsigned short QOff		:1;     // 12: Q Off (0: enabled, 1: disabled)
	volatile unsigned short RFU1		:5;     // 13 ~ 17: RFU (must be programmed to 0 during MRS)
};

struct DDR4_SDRAM_MR2 {
	volatile unsigned short RFU0		:3;     // 0 ~ 2:  (must be programmed to 0 during MRS)
	volatile unsigned short nCWL		:3;     // 3 ~ 5: CAS Write Latency (000: 5CK(tCK>=2.5ns), 001: 6CK(2.5ns>tCK>=1.875ns, 010: 7CK(1.875ns>tCK>=1.25ns), 011: 8CK(1.5ns>tCK>=1.25ns), 100: 9CK(1.25ns>tCK>=1.07ns))
	volatile unsigned short ASR		:2;     // 6 ~ 7: Auto Self Refresh(option)  (See Table 5)
	volatile unsigned short RFU1		:1;     // 8: RFU (must be programmed to 0 during MRS)
	volatile unsigned short RTT_WR		:2;     // 9: Dynamic ODT (See Table 6)
	volatile unsigned short RFU2		:5;     // 11: RFU (must be programmed to 0 during MRS)
	volatile unsigned short WRITE_CRC	:1;	// 12: Write CRC (0:Disable, 1:Enable)
};

struct DDR4_SDRAM_MR3 {
	volatile unsigned short MPR_RF		:2;     // 0 ~ 1: MPR Page Function (00: Page0, 01: Page1, 10:Page2, 11: Page3)
	volatile unsigned short MPR		:1;     // 2: MPR Operation (0: Normal DRAM operation, 1: Dataflow from MPR)
	volatile unsigned short GM		:1;	// 3: Geardown Mode (0: 1/2 Rate, 1:1/4 Rate)
	volatile unsigned short PDA		:1;	// 4: Per DRAM Addressability (0:Disable, 1: Enable)
	volatile unsigned short TSR		:1;	// 5: Temperature Sensor Readout (0: Disabled, 1: Enabled)
	volatile unsigned short FGR		:1;	// 6 ~ 8: Fine Granularity Refresh Mode (See Table 7)
	volatile unsigned short WCL_CRC_DM	:2;	// 9 ~ 10: Write CMD Lantency when CRC and DM are enabled (See Table 8)
	volatile unsigned short MPR_READ_FORMAT	:2;	// 11 ~ 12: MPR Read Format (00:Serial, 01: Parallel, 10:Staggered, 11: Reserved Temperature)
	volatile unsigned short RFU0		:5;	// 13 ~ 17: RFU (must be programmed to 0 during MRS)
};

struct DDR4_SDRAM_MR4 {
	volatile unsigned short RFU0		:1;	// 0: RFU
	volatile unsigned short MPD_MODE	:1;	// 1: Maximum Power Down Mode (0: Disable, 1:Enable)
	volatile unsigned short	TCRR		:1;	// 2: Temperature Controlleed Refresh Range (0: Normal, 1:Extended)
	volatile unsigned short TCRM		:1;	// 3: Temperature Controlled Refresh Mode (0: Disable, Enable)
	volatile unsigned short IVM		:1;	// 4: Internal Vref Monitor (0: Disable, 1:Enable)
	volatile unsigned short RFU1		:1;	// 5: RFU
	volatile unsigned short CS2CMD_CL	:3;	// 6 ~ 8: CS to CMD/ADDR Latency Mode (Cycles) (000: Disable, 001:3, 010:4, 011:5, 100:6, 101:8) (See Table 10)
	volatile unsigned short SRA		:1;	// 9: Self Refresh Abort (0: Disable, 1:Enable)
	volatile unsigned short RPTM		:1;	// 10: Read Preamble Training Mode (0: Disable, 1:Enable)
	volatile unsigned short RP		:1;	// 11: Read Preamble (0: 1nCK, 1: 2nCK)
	volatile unsigned short WP		:1;	// 12: Write Preamble (0: 1nCK, 1: 2nCK)
	volatile unsigned short RFU2		:5;	// 13 ~ 17 : RFU (must be programmed to 0 during MRS)
};

struct DDR4_SDRAM_MR5 {
	volatile unsigned short CA_PLM		:3;	// 0 ~ 2: C/A Parity Lantency Mode (See Table 12)
	volatile unsigned short CRC_EC		:1;	// 3: CRC Error Clear (0: Clear, 1:Error)
	volatile unsigned short CA_PES		:1;	// 4: C/A Parity Error Status (0:Clear, 1:Error)
	volatile unsigned short ODT_IBPD	:1;	// 5: ODT Input BUffer during Power Down mode
	volatile unsigned short RTT_PARK	:3;	// 6 ~ 8: RTT_Park  (See Table 11)
	volatile unsigned short CA_PE		:1;	// 9: CA Parity Persistent Error (0: Disable, 1:Enable)
	volatile unsigned short DATA_MASK	:1;	// 10: Data Mak(0:Disable, 1:Enable)
	volatile unsigned short WRITE_DBI	:1;	// 11: Write DBI (0:Disable, 1:Enable)
	volatile unsigned short READ_DBI	:1;	// 12: Read DBI(0: Disable, 1:Enable)
	volatile unsigned short RFU0		:5;	// 13 ~ 17: RFU (must be programmed to 0 during MRS)
};

struct DDR4_SDRAM_MR6 {
	volatile unsigned short VrefDQ_Value	:6;	// 0 ~ 5: VrefDQ Training Value (See Table 15)
	volatile unsigned short VrefDQ_TRange	:1;	// 6: VrefDQ Training Range ( See Table 14)
	volatile unsigned short VrefDQ_Enable	:1;	// 7: VrefDQ Training Enable (0:Normal Operation Mode, 1:Training Mode)
	volatile unsigned short RFU0		:2;	// 8 ~ 9:  RFU (must be programmed to 0 during MRS)
	volatile unsigned short ntCCD_L		:3;	// 10 ~ 12: (See Table 13)
	volatile unsigned short RFU1		:5;	// 13 ~ 17:  RFU (must be programmed to 0 during MRS)
};

union DDR4_SDRAM_MR {
	struct DDR4_SDRAM_MR0 MR0;
	struct DDR4_SDRAM_MR1 MR1;
	struct DDR4_SDRAM_MR2 MR2;
	struct DDR4_SDRAM_MR3 MR3;
	struct DDR4_SDRAM_MR4 MR4;
	struct DDR4_SDRAM_MR5 MR5;
	struct DDR4_SDRAM_MR6 MR6;

	volatile unsigned short REG;
};

#endif // #ifndef __DDR4_SDRAM_H__