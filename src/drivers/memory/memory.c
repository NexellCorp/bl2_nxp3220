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
#include <sysheader.h>
#if defined(DDR3)
#include <ddr3_sdram.h>
#elif defined(DDR4)
#include <ddr4_sdram.h>
#endif
#include <memory.h>

/* Extern Function */
extern int ddr3_initialize (unsigned int is_resume);
extern int ddr4_initialize (unsigned int is_resume);

struct dram_device_info g_ddr_info;

static void get_dram_information(struct dram_device_info *me)
{
	int byte = 8;

	/* Nexell Step XX. Memory Address (for Write Training (DRAM)) */
	me->bank_num	= g_nsih->dii.bank_num;
	me->row_num	= g_nsih->dii.row_num;
	me->column_num	= g_nsih->dii.col_num;

	me->column_size	= (1 << me->column_num)/byte;				// Unit : Byte
	me->row_size	= (1 << me->row_num);
	me->bank_size	= (me->row_size * me->column_size);
	me->chip_size	= (((me->bank_size * (1 << me->bank_num))
				* g_nsih->dii.bus_width)/ 1024 / 1024);		// Unit: MB
	me->sdram_size	= (me->chip_size * (g_nsih->dii.chip_num
				* 32 / g_nsih->dii.bus_width));
#if 0
	SYSMSG("############## [SDRAM] Memory Specification ###############\r\n");
	SYSMSG("[Bit] Bank Address   : %d \r\n", me->bank_num);
	SYSMSG("[Bit] Column Address : %d \r\n", me->column_num);
	SYSMSG("[Bit] Row Address    : %d \r\n", me->row_num);
	SYSMSG("[Bit] Data Line      : %d \r\n", g_nsih->dii.bus_width);
	SYSMSG("[BYTE] Column    Size: %d \r\n", me->column_size);
	SYSMSG("[BYTE] Row(Page) Size: %d \r\n", me->row_size);
	SYSMSG("[BYTE] Bank      Size: %d \r\n", me->bank_size);
#if 1
	SYSMSG("[MB]   Chip      Size: %d \r\n", me->chip_size);
	SYSMSG("[MB]   SDRAM     Size: %d \r\n", me->sdram_size);
#endif
	SYSMSG("############################################################\r\n");
#endif
}


#include <dphy.h>
#include <dctrl.h>

#if 0
static mem_back(void)
{
	int reg_value;

	// phy initialization
	mmio_write_32(DPHY_BASE_ADDR + UNIQUIFY_IO_3            , 0x00000000);      // 0x1cc
	mmio_write_32(DPHY_BASE_ADDR + PHY_PAD_CTRL             , 0x50ff44f3);      // 0x120
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + VREF_TRAINING            , 0x00200074);      // 0x1c8
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000006);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + VREF_TRAINING            , 0x00200074);      // 0x1c8
	mmio_write_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 0x00000001);      // 0x170
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_INCR_TRIM_3      , 0x00000000);      // 0x168
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_INCR_TRIM_1      , 0x00000000);      // 0x164
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_1           , 0x00000005);      // 0x130
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_3           , 0x0000000a);      // 0x138
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000006);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_1           , 0x00000005);      // 0x130
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_3           , 0x0000000a);      // 0x138
	mmio_write_32(DPHY_BASE_ADDR + SCL_WINDOW_TRIM          , 0x09050002);      // 0x1b4
	mmio_write_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_1         , 0x00000000);      // 0x15c
	mmio_write_32(DPHY_BASE_ADDR + DYNAMIC_IE_TIMER         , 0x0000001a);      // 0x1e8
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_1             , 0x01000075);      // 0x118
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_2             , 0x00000501);      // 0x11c
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_3             , 0x00000000);      // 0x16c
	mmio_write_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL    , 0x0002B220);      // 0x1c0
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_4             , 0x00000000);      // 0x1bc
	mmio_write_32(DPHY_BASE_ADDR + SCL_GATE_TIMING          , 0x000000f0);      // 0x1e0    // f0?
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_DYN_ODT            , 0x02200020);      // 0x150
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_ON_OFF             , 0x00c60046);      // 0x154
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL          , 0x00000210);      // 0x128
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_CLK         , 0x00000090);      // 0x1a4
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_RECALIB          , 0xa8001010);      // 0x124

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL);                   // 0x128
	while ((reg_value & 0xFF000000) == 0xFF000000) {
	    reg_value  = mmio_read_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1);                     // 0x170
	while ((reg_value & 0x00000002) != 0x00000002) {
	    reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1);
	}

	mmio_set_32  (DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 1<<2      );      // 0x170
	mmio_clear_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 1<<2      );      // 0x170

	mmio_write_32(DPHY_BASE_ADDR + SCL_LATENCY              , 0x00005076);      // 0x10C

	// controller initialization
	mmio_write_32(DCTRL_BASE_ADDR + MEM_WIDTH               , 0x00000048);      // 0x50
	mmio_write_32(DCTRL_BASE_ADDR + ARB_AGENT_SEL           , 0x00000000);      // 0x0
	mmio_write_32(DCTRL_BASE_ADDR + MIN_CMD_ACPT            , 0x00000004);      // 0xc
	mmio_write_32(DCTRL_BASE_ADDR + ARB_AGENT_SEL           , 0x00000008);      // 0x0
	mmio_write_32(DCTRL_BASE_ADDR + MIN_CMD_ACPT            , 0x00000004);      // 0xc
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_1            , 0x03190a0d);      // 0x14
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_2            , 0x00007fff);      // 0x18
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_3            , 0x000a0000);      // 0x1c
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_4            , 0x000003ff);      // 0x20
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_5            , 0x00000007);      // 0x24
	mmio_write_32(DCTRL_BASE_ADDR + REF_CONFIG              , 0x18670c33);      // 0x28     // 0x182a0c27
	mmio_write_32(DCTRL_BASE_ADDR + PWR_SAVE_ECC_CONFIG     , 0x00002020);      // 0x2c
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_1            , 0xc32131aa);      // 0x30
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_2            , 0x00228102);      // 0x34
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_3            , 0xa553d306);      // 0x38
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_4            , 0x800e130d);      // 0x3c
	mmio_write_32(DCTRL_BASE_ADDR + ZQC_CONFIG              , 0x6d000002);      // 0x54
	mmio_write_32(DCTRL_BASE_ADDR + AUTO_SCL_CTRL           , 0x00000002);      // 0x58
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00000000);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + ODT_EN_CONFIG           , 0x00010002);      // 0x4c
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00000002);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + ODT_EN_CONFIG           , 0x00020001);      // 0x4c
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00211100);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + DATA_XFR_CONFIG         , 0x73030003);      // 0x44
	mmio_write_32(DCTRL_BASE_ADDR + AXI_PORTN_HALT_REG      , 0x00000000);      // 0x140
	mmio_write_32(DCTRL_BASE_ADDR + AXI_PORTN_FIFO_REG      , 0x00000000);      // 0x148

	// ddr3 initialization
	mmio_write_32(DCTRL_BASE_ADDR + MEM_START               , 0x00000001);      // 0x10
	mmio_clear_32(DCTRL_BASE_ADDR + MEM_WIDTH               , 1<<2      );      // 0x50

	// 1. Power On. Asert RESET# and ensure CKE is LOW at least max of 10ns or 5tCK
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_0            , 0x00ffeffe);      // 0x80
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_0            , 0x00004000);      // 0xc0
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_1            , 0x00ffeffe);      // 0x84
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_1            , 0x00000000);      // 0xc4

	// Issue 2 commands to the initialization statue m/c and wait for it to be done
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE          , 0x00000011);      // 0x48
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	//2. DRAM RESET# pin must be LOW for at least 200us before DRAM is brought out of reset
	//   We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	//   WARNING: 200 us is required before RST_N goes inactive.
	//   Insert software function to wait 200us here..
	//udelay(200);

	//3. CKE must be LOW 10ns prior to RESET# transitioning HIGH
	//   Already taken care of. So Deasserting RESET#
	mmio_set_32  (DCTRL_BASE_ADDR + MEM_WIDTH               , 0x00000012);

	//4. After RESET# is deasserted, wait 500us with CKE LOW
	//   We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	//   WARNING: 500 us is required after RST_N goes inactive before CKE goes active.
	//   Insert software function to wait 500us here..
	//udelay(500);

	//5. CKE may be brought HIGH and only NOP or DES commands may be issued for TXPR
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_0            , 0x00ffffff);     // 0x80
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_0            , 0x0006c000);     // 0xc0

	//6.Issue an MRS command to MR2.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_1            , 0x20fff001);     // 0x84
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_1            , 0x00001202);     // 0xc4

	//7.Issue an MRS command to MR3.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_2            , 0x00fff001);     // 0x88
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_2            , 0x00001300);     // 0xc8

	//8.Issue an MRS command to MR1.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_3            , 0x46fff001);     // 0x8c
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_3            , 0x00001100);     // 0xcc

	//9.Issue an MRS command to MR0.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_4            , 0x14fff001);     // 0x90
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_4            , 0x0000601f);     // 0xd0

	//10.Issue a ZQCL command to calibrate Rtt and Ron values for the process voltage temperature
	//   (PVT). Prior to normal operation, tZQinit must be satisfied.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_5            , 0x00fff601);     // 0x94
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_5            , 0x00100004);     // 0xd4

	// Issue 6 command to the intialization state m/c and wait for it to be done
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE          , 0x00000015);     // 0x48
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	//Set initialization complete bit
	mmio_write_32(DCTRL_BASE_ADDR + MEM_START               , 0x00000003);     // 0x80

	// Wait for Analog DLL to lock
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);                 // 0x160
	while ((reg_value & 0x00000003) != 0x00000003) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);
	}

	// training
	// wrtite leveling
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_AUTOINC_TRIM           , 0x00000003);          // Set start_wr_lvl for all lanes
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_2               , 0x1 << 16 );          // Clear all dlls_trim_2 registers
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x1 << 30 |
	                                                              0x1 << 28 );          // Set wr level command
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);                                 // Check for completion
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + WRLVL_CTRL);                                // Read out the result
	// Todo WR_LVL fail check with reg_value


	// bit leveling - ?
	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000000);

	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_0                   , 0xFF00FF00);          // Write in the bit leveling data
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_1                   , 0xFF00FF00);
	mmio_write_32(DPHY_BASE_ADDR + PHY_SCL_START_ADDR           , 0x8   <<16);          // Set the address to start at in the DRAM, Note scl_start_col_addr moves in ddr4
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x88  <<21);          // Initialize the data by setting wr_only
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);                                 // Check for completion
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	mmio_write_32(DPHY_BASE_ADDR + PHY_SCL_START_ADDR           , 0x0  <<16   );        // Set the address for scl start
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x305<<20   );        // Set bit leveling normal from DRAM with write side
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + DYNAMIC_BIT_LVL);
	// Todo BIT_LVL fail check with reg_value

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL);
	// Todo WR_BIT_LVL fail check with reg_value

	// SCL
	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000001);

	mmio_write_32(DPHY_BASE_ADDR + SCL_MAIN_CLK_DELTA           , 0x00000010);          // Clear main clock delta register before doing SCL write-read test
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_0                   , 0x789b3de0);          // Assume that bit-leveling has used these
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_1                   , 0xf10e4a56);          // registers so re-write the scl data
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 1 << 28 | 1 << 24);   // Set wr_only bit to initialize memory data
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	// Call SCL without write-leveling save restore data(set [31] high for save/restore)
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 1 << 29 |   //continuous_rds
	                                                              1 << 28 |   //set_ddr_scl_go_done
	                                                              1 << 26 );  //incr_scl
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}
	// Todo check SCL without write-leveling fail check with reg_value

	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000003);
	mmio_clear_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL        , 1<<0      );
	mmio_set_32  (DPHY_BASE_ADDR + DYNAMIC_BIT_LVL              , 1<<0      );
}
#else
static mem_back(void)
{
	int reg_value;

	// phy initialization
	mmio_write_32(DPHY_BASE_ADDR + UNIQUIFY_IO_3            , 0x00000000);      // 0x1cc
	mmio_write_32(DPHY_BASE_ADDR + PHY_PAD_CTRL             , 0x50ff44f3);      // 0x120
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + VREF_TRAINING            , 0x00200074);      // 0x1c8
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000006);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + VREF_TRAINING            , 0x00200074);      // 0x1c8
	mmio_write_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 0x00000001);      // 0x170
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_INCR_TRIM_3      , 0x00000000);      // 0x168
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_INCR_TRIM_1      , 0x00000000);      // 0x164
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_1           , 0x00000005);      // 0x130
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_3           , 0x0000000a);      // 0x138
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000006);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_1           , 0x00000005);      // 0x130
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_3           , 0x0000000a);      // 0x138
	mmio_write_32(DPHY_BASE_ADDR + SCL_WINDOW_TRIM          , 0x09050002);      // 0x1b4
	mmio_write_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_1         , 0x00000000);      // 0x15c
	mmio_write_32(DPHY_BASE_ADDR + DYNAMIC_IE_TIMER         , 0x0000001a);      // 0x1e8
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_1             , 0x01000075);      // 0x118
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_2             , 0x00000501);      // 0x11c
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_3             , 0x00000000);      // 0x16c
	mmio_write_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL    , 0x000261c0);      // 0x1c0
	mmio_write_32(DPHY_BASE_ADDR + SCL_CONFIG_4             , 0x00000000);      // 0x1bc
	mmio_write_32(DPHY_BASE_ADDR + SCL_GATE_TIMING          , 0x000000f0);      // 0x1e0    // f0?
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_DYN_ODT            , 0x02200020);      // 0x150
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_ON_OFF             , 0x00c60046);      // 0x154
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL          , 0x00000210);      // 0x128
	mmio_write_32(DPHY_BASE_ADDR + PHY_LANE_SEL             , 0x00000000);      // 0x12c
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_CLK         , 0x00000090);      // 0x1a4
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_RECALIB          , 0xa8001010);      // 0x124

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL);                   // 0x128
	while ((reg_value & 0xFF000000) == 0xFF000000) {
	    reg_value  = mmio_read_32(DPHY_BASE_ADDR + PHY_DLL_ADRCTRL);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1);                     // 0x170
	while ((reg_value & 0x00000002) != 0x00000002) {
	    reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1);
	}

	mmio_set_32  (DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 1<<2      );      // 0x170
	mmio_clear_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1            , 1<<2      );      // 0x170

	mmio_write_32(DPHY_BASE_ADDR + SCL_LATENCY              , 0x00005076);      // 0x10C

	// controller initialization
	mmio_write_32(DCTRL_BASE_ADDR + MEM_WIDTH               , 0x00000048);      // 0x50
	mmio_write_32(DCTRL_BASE_ADDR + ARB_AGENT_SEL           , 0x00000000);      // 0x0
	mmio_write_32(DCTRL_BASE_ADDR + MIN_CMD_ACPT            , 0x00000004);      // 0xc
	mmio_write_32(DCTRL_BASE_ADDR + ARB_AGENT_SEL           , 0x00000008);      // 0x0
	mmio_write_32(DCTRL_BASE_ADDR + MIN_CMD_ACPT            , 0x00000004);      // 0xc
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_1            , 0x03190a0d);      // 0x14
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_2            , 0x00007fff);      // 0x18
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_3            , 0x000a0000);      // 0x1c
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_4            , 0x000003ff);      // 0x20
	mmio_write_32(DCTRL_BASE_ADDR + MEM_CONFIG_5            , 0x00000807);      // 0x24
	mmio_write_32(DCTRL_BASE_ADDR + REF_CONFIG              , 0x18660c2e);      // 0x28     // 0x182a0c27
	mmio_write_32(DCTRL_BASE_ADDR + PWR_SAVE_ECC_CONFIG     , 0x00002020);      // 0x2c
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_1            , 0xc32131aa);      // 0x30
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_2            , 0x00218002);      // 0x34
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_3            , 0xa553d206);      // 0x38
	mmio_write_32(DCTRL_BASE_ADDR + DLY_CONFIG_4            , 0x7f0d130d);      // 0x3c
	mmio_write_32(DCTRL_BASE_ADDR + ZQC_CONFIG              , 0x6c000002);      // 0x54
	mmio_write_32(DCTRL_BASE_ADDR + AUTO_SCL_CTRL           , 0x00000002);      // 0x58
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00000000);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + ODT_EN_CONFIG           , 0x00010002);      // 0x4c
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00000002);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + ODT_EN_CONFIG           , 0x00020001);      // 0x4c
	mmio_write_32(DCTRL_BASE_ADDR + ODT_CONFIG              , 0x00211100);      // 0x40
	mmio_write_32(DCTRL_BASE_ADDR + DATA_XFR_CONFIG         , 0x73030003);      // 0x44
	mmio_write_32(DCTRL_BASE_ADDR + AXI_PORTN_HALT_REG      , 0x00000000);      // 0x140
	mmio_write_32(DCTRL_BASE_ADDR + AXI_PORTN_FIFO_REG      , 0x00000000);      // 0x148

	// ddr3 initialization
	mmio_write_32(DCTRL_BASE_ADDR + MEM_START               , 0x00000001);      // 0x10
	mmio_clear_32(DCTRL_BASE_ADDR + MEM_WIDTH               , 1<<2      );      // 0x50

	// 1. Power On. Asert RESET# and ensure CKE is LOW at least max of 10ns or 5tCK
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_0            , 0x00ffeffe);      // 0x80
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_0            , 0x00003000);      // 0xc0
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_1            , 0x00ffeffe);      // 0x84
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_1            , 0x00000000);      // 0xc4

	// Issue 2 commands to the initialization statue m/c and wait for it to be done
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE          , 0x00000011);      // 0x48
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	//2. DRAM RESET# pin must be LOW for at least 200us before DRAM is brought out of reset
	//   We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	//   WARNING: 200 us is required before RST_N goes inactive.
	//   Insert software function to wait 200us here..
	//udelay(200);

	//3. CKE must be LOW 10ns prior to RESET# transitioning HIGH
	//   Already taken care of. So Deasserting RESET#
	mmio_set_32  (DCTRL_BASE_ADDR + MEM_WIDTH               , 0x00000012);

	//4. After RESET# is deasserted, wait 500us with CKE LOW
	//   We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	//   WARNING: 500 us is required after RST_N goes inactive before CKE goes active.
	//   Insert software function to wait 500us here..
	//udelay(500);

	//5. CKE may be brought HIGH and only NOP or DES commands may be issued for TXPR
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_0            , 0x00ffffff);     // 0x80
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_0            , 0x0006b000);     // 0xc0

	//6.Issue an MRS command to MR2.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_1            , 0x20fff001);     // 0x84
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_1            , 0x00001202);     // 0xc4

	//7.Issue an MRS command to MR3.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_2            , 0x00fff001);     // 0x88
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_2            , 0x00001300);     // 0xc8

	//8.Issue an MRS command to MR1.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_3            , 0x46fff001);     // 0x8c
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_3            , 0x00001100);     // 0xcc

	//9.Issue an MRS command to MR0.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_4            , 0x14fff001);     // 0x90
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_4            , 0x0000501d);     // 0xd0

	//10.Issue a ZQCL command to calibrate Rtt and Ron values for the process voltage temperature
	//   (PVT). Prior to normal operation, tZQinit must be satisfied.
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_1_5            , 0x00fff601);     // 0x94
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_2_5            , 0x00100004);     // 0xd4

	// Issue 6 command to the intialization state m/c and wait for it to be done
	mmio_write_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE          , 0x00000015);     // 0x48
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	//Set initialization complete bit
	mmio_write_32(DCTRL_BASE_ADDR + MEM_START               , 0x00000003);     // 0x80

	// Wait for Analog DLL to lock
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);                 // 0x160
	while ((reg_value & 0x00000003) != 0x00000003) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);
	}

	// training
	// wrtite leveling
	mmio_write_32(DPHY_BASE_ADDR + WRLVL_AUTOINC_TRIM           , 0x00000003);          // Set start_wr_lvl for all lanes
	mmio_write_32(DPHY_BASE_ADDR + PHY_DLL_TRIM_2               , 0x1 << 16 );          // Clear all dlls_trim_2 registers
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x1 << 30 |
	                                                              0x1 << 28 );          // Set wr level command
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);                                 // Check for completion
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + WRLVL_CTRL);                                // Read out the result
	// Todo WR_LVL fail check with reg_value


	// bit leveling - ?
	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000000);

	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_0                   , 0xFF00FF00);          // Write in the bit leveling data
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_1                   , 0xFF00FF00);
	mmio_write_32(DPHY_BASE_ADDR + PHY_SCL_START_ADDR           , 0x8   <<16);          // Set the address to start at in the DRAM, Note scl_start_col_addr moves in ddr4
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x88  <<21);          // Initialize the data by setting wr_only
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);                                 // Check for completion
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	mmio_write_32(DPHY_BASE_ADDR + PHY_SCL_START_ADDR           , 0x0  <<16   );        // Set the address for scl start
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 0x305<<20   );        // Set bit leveling normal from DRAM with write side
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + DYNAMIC_BIT_LVL);
	// Todo BIT_LVL fail check with reg_value

	reg_value  = mmio_read_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL);
	// Todo WR_BIT_LVL fail check with reg_value

	// SCL
	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000001);

	mmio_write_32(DPHY_BASE_ADDR + SCL_MAIN_CLK_DELTA           , 0x00000010);          // Clear main clock delta register before doing SCL write-read test
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_0                   , 0x789b3de0);          // Assume that bit-leveling has used these
	mmio_write_32(DPHY_BASE_ADDR + SCL_DATA_1                   , 0xf10e4a56);          // registers so re-write the scl data
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 1 << 28 | 1 << 24);   // Set wr_only bit to initialize memory data
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}

	// Call SCL without write-leveling save restore data(set [31] high for save/restore)
	mmio_write_32(DPHY_BASE_ADDR + SCL_START                    , 1 << 29 |   //continuous_rds
	                                                              1 << 28 |   //set_ddr_scl_go_done
	                                                              1 << 26 );  //incr_scl
	reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	while ((reg_value & 0x1 << 28) == 0x1 << 28) {
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + SCL_START);
	}
	// Todo check SCL without write-leveling fail check with reg_value

	mmio_write_32(DPHY_BASE_ADDR + DISABLE_GATING_FOR_SCL       , 0x00000003);
	mmio_clear_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL        , 1<<0      );
	mmio_set_32  (DPHY_BASE_ADDR + DYNAMIC_BIT_LVL              , 1<<0      );
}

#endif

int memory_initialize(int is_resume)
{
	int ret = 0;

	/* step xx. memory initialize  */
#if defined(DDR3)
#if 1
	ret = ddr3_initialize(is_resume);
#else
	mem_back();
#endif

#elif defined(DDR4)
	ret = ddr4_initialize(is_resume);
#endif

	NOTICE("Memory Initialize %s! (%d:%d) \r\n\n",
			(ret >= 0) ? "Done" : "Failed", ret, is_resume);

	if (ret >= 0)
		return -1;

	get_dram_information(&g_ddr_info);

	return ret;
}
