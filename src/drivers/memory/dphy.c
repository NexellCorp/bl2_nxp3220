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
#include <dphy.h>

#if defined(DDR3)
#include <ddr3_ac_timing.h>
#elif defined(DDR4)
#include <ddr4_ac_timing.h>
#endif

#include <pll.h>

/* Fixed Features */
#define SCL_LANES			4

#define HALF_RATE_MODE			1
#define SWAP_PHASE			0

#define MEM_STRB_WIDTH			2
#define MEM_CLOCKS			1

/* Bit-Leveling Features */
#define BIT_LVL_WR_DYNAMIC		0

/* SCL_WINDOW_TRIM: Address 7 h6D */
#define INITIAL_DCAP_CLK_DLY		2

/* VREF_TRAINING: Address 7 h72 */
#define VREF_OUTPUT_ENABLE		1
#define VREF_VALUE			7
#define VREF_WIDTH			6

#define SLV_DLY_WIDTH			6
#define BITLVL_DLY_WIDTH		6

/* PHY_PAD_CTRL: Address 7 h48 */
#define PREAMBLE_DLY			0x2

/* DDR - Device ODT, ODS Values */
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
#define DDR3_RTT_NOM			CONFIG_DRAM_MR1_RTT_Nom			// MR1: 3'b011 = RZQ/6, 40 Ohm
#define DDR3_RTT_WR			CONFIG_DRAM_MR2_RTT_WR			// MR2: Dynamic ODT. 2'b01 = RZQ/4, 60 ohm
#define DDR3_AL				0					// MR1: Additive Latency disabled
#define DDR3_DIC			CONFIG_DRAM_MR1_ODS			// MR1: Driver Impedance. 2'b00 = RZQ/7, 34 Ohm
#define CWL_DDR3			2
#else
#define DDR4_RTT_NOM			CONFIG_DRAM_MR1_RTT_Nom			// MR1: 3'b011 = RZQ/6, 40 Ohm
#define DDR4_RTT_WR			CONFIG_DRAM_MR2_RTT_WR			// MR2: RZQ/2, 120 Ohm
#define DDR4_AL				0					// MR1: Additive Latency disabled
#define DDR4_DIC			CONFIG_DRAM_MR1_ODS			// MR1: Driver Impedance. 2'b00 = RZQ/7, 34 Ohm
#define CWL_DDR4			2
#endif

#endif

/* Only DDR4 */
#define DDR4_2_CYCLE_PREAMBLE		1

#define MAIN_CLK_DLY			7
#define SCL_WAIT_LATENCY		5

#define UNIQUIFY_NUM_STAGES_A2D		1

static volatile unsigned int m_flag = 0;


static void reg_write_phy(int addr, int reg_value)
{
	mmio_write_32((void*)(DPHY_BASE_ADDR + addr), reg_value);

#if defined(DDR_DBG_ON)
	printf("[phy-write] addr: 0x%08X, reg: 0x%08X \r\n", (DPHY_BASE_ADDR + addr), reg_value);
#endif
}

static void reg_set_phy(int addr, int reg_value)
{
	mmio_set_32((void*)(DPHY_BASE_ADDR + addr), reg_value);

#if defined(DDR_DBG_ON)
	printf("[phy-set] addr: 0x%08X, reg: 0x%08X \r\n", (DPHY_BASE_ADDR + addr), reg_value);
#endif
}

static int reg_read_phy(int addr)
{
	int reg_value;
	reg_value = mmio_read_32((void*)(DPHY_BASE_ADDR + addr));
#if defined(DDR_DBG_ON)
	printf("[phy-read] addr: 0x%08X, reg: 0x%08X \r\n", (DPHY_BASE_ADDR + addr), reg_value);
#endif
	return reg_value;
}

static int get_period_ps(void)
{
	int cycle_period_ps;

	/* step xx. Calcurate the DDRx One Cycle -> Period(ps)  */
	cycle_period_ps = (1000000/(get_pll_freq(3)/1000000));

	return cycle_period_ps;
}

int hw_write_leveling(void)
{
	int wrlvl_rslt, status;
	int lane_mask = ((0x1 << MEM_STRB_WIDTH) - 1);

	/* step 01. set the automatic wrrite-leveling */
	reg_write_phy(WRLVL_AUTOINC_TRIM, lane_mask);				/* Set start_wr_lvl for all lanes */
	reg_write_phy(PHY_DLL_TRIM_2, (0x1 << 16));				/* Clear all dlls_trim_2 registers */
	reg_write_phy(SCL_START, (0x1 << 30) | (0x1 << 28));			/* Set Write Leveling Command */

	/* step 02. check for write-leveling completion */
	do {
		status = ((reg_read_phy(SCL_START) >> 28) & 0x1);
	} while (status);

	/* step 03. display the write-leveling information */
	wrlvl_rslt = ((reg_read_phy(WRLVL_CTRL)	>> 0) & lane_mask);
	printf("####### Write Leveling - Information ########\r\n");
	printf("Write Leveling %s (%d) \r\n", (wrlvl_rslt == 0x3)
		? "Pass!!" : "Failed!!", wrlvl_rslt);
	printf("#############################################\r\n");

	return true;
}

void hw_bit_leveling_information(void)
{
	int op_bitwise_trim, ip_bitwise_trim;
	int bit_lvl_failure_status, analog_dll_lock, dll_ps;
	int lane, dq_num;
	int inc, step;
	int once = 0;

	printf("######## Bit Leveling - Information #########\r\n");
	printf("Write Trim:\r\n");
	for (lane = 0; lane < MEM_STRB_WIDTH; lane++) {
		bit_lvl_failure_status = (reg_read_phy(DYNAMIC_WRITE_BIT_LVL)
			>> (20 + lane)) & 0x1;
		if (!bit_lvl_failure_status) {
			if (!once) {
				printf("	", lane);
				for (dq_num = 0; dq_num < 10; dq_num++) {
					printf("%s%d  ",
						(dq_num != 8) ? ((dq_num != 9) ?
						" DQ":"DQS"):(" DM"), dq_num);
				}
				printf("\r\n");
				once = 1;
			}
			printf("Lane %d  ", lane);
			for (dq_num = 0; dq_num < 10; dq_num++) {
				reg_write_phy(PHY_LANE_SEL,
					((lane * (BITLVL_DLY_WIDTH + 1)) |
					 (dq_num << 8)));
				op_bitwise_trim =
					reg_read_phy(OP_DQ_DM_DQS_BITWISE_TRIM);
				inc  = ((op_bitwise_trim >> 6) & 0x1);
				step = ((op_bitwise_trim >> 0) & 0x3F);
				printf(" %c%2d	",
					(inc ? '+': (step != 0 ? '-': ' ')), step);
			}
			printf("\r\n");
		} else
			printf("bit leveling failure\r\n");
	}
	printf("\r\n");

	once = 0;
	printf("Read Trim:\r\n");
	for (lane = 0; lane < MEM_STRB_WIDTH; lane++) {
		bit_lvl_failure_status = (reg_read_phy(DYNAMIC_BIT_LVL)
			>> (14 + lane)) & 0x1;
		if (!bit_lvl_failure_status) {
			if (!once) {
				printf("	", lane);
				for (dq_num = 0; dq_num < 9; dq_num++) {
					printf("%s%d  ",
						(dq_num != 8) ? " DQ":"DQS", dq_num);
				}
				printf("\r\n");
				once = 1;
			}
			printf("Lane %d  ", lane);
			for (dq_num = 0; dq_num < 9; dq_num++) {
				reg_write_phy(PHY_LANE_SEL,
					(lane * (BITLVL_DLY_WIDTH + 1)) |
					(dq_num << 8));
				ip_bitwise_trim =
					reg_read_phy(IP_DQ_DQS_BITWISE_TRIM);
				inc  = ((ip_bitwise_trim >> 6) & 0x1);
				step = ((ip_bitwise_trim >> 0) & 0x3F);
				printf(" %c%2d	",
					(inc ? '+': (step != 0 ? '-': ' ')), step);
			}
			printf("\r\n");
		} else
			printf("bit leveling failure\r\n");
	}
	printf("\r\n");

	analog_dll_lock = ((reg_read_phy(PHY_DLL_ADRCTRL) >> 24) & 0xff);
	dll_ps = (get_period_ps()/analog_dll_lock);
	once = (get_period_ps() - dll_ps * analog_dll_lock) * 1000 / analog_dll_lock;

	printf("#############################################\r\n");
	printf("1-Cycle Period(ps): %d, 1-Step Period(ps): %d.%d \r\n",
		get_period_ps(), dll_ps, once);
	printf("Lock Value: %d\r\n", analog_dll_lock);

}

int hw_bit_leveling(void)
{
	int status, reg_value;

	reg_write_phy(DISABLE_GATING_FOR_SCL, 0x00000000);

	reg_write_phy(SCL_DATA_0, 0xFF00FF00);					// Write in the bit leveling data
	reg_write_phy(SCL_DATA_1, 0xFF00FF00);
	reg_write_phy(PHY_SCL_START_ADDR, (0x8 << 16));				// Set the address to start at in the DRAM, Note scl_start_col_addr moves in ddr4
	reg_write_phy(SCL_START, (0x88 << 21));					// Initialize the data by setting wr_only

	do {
		status = ((reg_read_phy(SCL_START) >> 28) & 0x1);
	} while (status);

	reg_write_phy(PHY_SCL_START_ADDR, (0x0 << 16));				// Set the address for scl start
	reg_write_phy(SCL_START, (0x305 << 20));				// Set bit leveling normal from DRAM with write side
	reg_value  = reg_read_phy(SCL_START);

	do {
		status = ((reg_read_phy(SCL_START) >> 28) & 0x1);
	} while (status);

	reg_value  = reg_read_phy(DYNAMIC_WRITE_BIT_LVL);			// Todo WR_BIT_LVL fail check with reg_value

	reg_write_phy(DISABLE_GATING_FOR_SCL, 0x00000001);

	reg_write_phy(SCL_MAIN_CLK_DELTA, 0x00000010);				// Clear main clock delta register before doing SCL write-read test
	reg_write_phy(SCL_DATA_0, 0x789b3de0);					// Assume that bit-leveling has used these
	reg_write_phy(SCL_DATA_1, 0xf10e4a56);					// registers so re-write the scl data
	reg_write_phy(SCL_START, 1 << 28 | 1 << 24);				// Set wr_only bit to initialize memory data

	do {
		status = ((reg_read_phy(SCL_START) >> 28) & 0x1);
	} while (status);

	/* Call SCL without write-leveling save restore data(set [31] high for save/restore) */
	reg_write_phy(SCL_START, (1 << 29) |					//continuous_rds
				 (1 << 28) |					//set_ddr_scl_go_done
				 (1 << 26));					//incr_scl
	do {
		status = ((reg_read_phy(SCL_START) >> 28) & 0x1);
	} while (status);

	/* step xx. display the bit-leveling information */
	hw_bit_leveling_information();

	/* step xx. set the dynamic bit-leveling (write/read) */
	reg_write_phy(DISABLE_GATING_FOR_SCL, 0x00000001);
	mmio_clear_32(DPHY_BASE_ADDR + DYNAMIC_WRITE_BIT_LVL, (1 << 0));
	mmio_set_32(DPHY_BASE_ADDR + DYNAMIC_BIT_LVL, (1 << 0));

	return true;
}

void phy_set_init_values (void)
{
/*
  * NOTE: For write side bit leveling the state machine wait counters are critically dependent on
  * the speed grade of the memory used and the speed grade support set in the addr phy. Therefore we
  * adjust the latencies to be programmed based on what has been selected for the phy, below.
  */
  	float one_ns = 1000000000;
	int dram_clk_period = (one_ns/get_pll_freq(3))*1000;//1250;	[DEBUG]

	int CMD_TO_DDR = (SWAP_PHASE ? 1 : 2);

	int PHY_BL = 2;								// BL used in the PHY

	int READ_TO_WRITE = (6 >> 1) + 2 + 2;					// 6 refers to tCCD_MAX in DDR4
	int WRITE_TO_READ = (0xD + 2);

	int RL = g_nsih->dii.ac_timing.RL, WL = g_nsih->dii.ac_timing.WL;
	float rl_d = RL, wl_d = WL;
	float true_rl_d = round_up(rl_d / 2), true_wl_d = round_up(wl_d / 2);

	int TRUE_RL = (int)true_rl_d, TRUE_WL = (int)true_wl_d;


	// Read to Write  2nd Write burst in ddr4 Write to Read	2 Reads bit-lvl st mc evaluate
	int TRIM_LAT = READ_TO_WRITE + 1 + TRUE_RL - TRUE_WL
	#if defined(DDR4)
			+ PHY_BL
	#endif
			+ WRITE_TO_READ + 1 + PHY_BL * 2 - 2;
	// Read to Write, 2nd Write burst in ddr4 Write to Read, 2 Reads, Half of 3rd read, Int-cmd to DDR time
	int READ_LAT = READ_TO_WRITE + 1 + TRUE_RL - TRUE_WL			// cmd to data avail, d1->d3 rd_side_done, start signal A->D tim, switch state Rd->Wr
	#if defined(DDR4)
			+ PHY_BL
	#endif
			+ WRITE_TO_READ + 1 + PHY_BL * 2 + PHY_BL/2 + CMD_TO_DDR// cmd to data avail, d1->d3 rd_side_done, start signal A->D tim, switch state Rd->
			+ TRUE_RL - 2 - UNIQUIFY_NUM_STAGES_A2D + 1;
	int DLY_DFI_WRDATA = (((WL % 2 == 0) ? 1 : 0) ^ SWAP_PHASE);

	int extra_addrctrl_dly;

	int i, reg_value = 0;

//	mdelay(0xFFFFFFFF);

	/* step xx. ddr4 support */
#if defined(DDR4) || defined(LPDDR3)
	reg_value = (POD_TERM | DDR4_SUPPORT);
	reg_write_phy(DDR4_CONFIG_1, reg_value);
#endif

#if defined(DDR4)
	/*
	  * only used with uniquify IOs. Register only exists if PHY supports
	  * DDR4 memories.
	  */
	mmio_write_32((DPHY_BASE_ADDR + UNIQUIFY_IO_3), (1 << 24));		// ucal_2step_cal
#else
//	reg_value = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_3);
//	reg_value &= ~(1 << 24);
	reg_value = 0;
	reg_write_phy(UNIQUIFY_IO_3, reg_value);				// ucal_2step_cal
#endif

	/* Step XX. Set the PHY_PAD_CTRL */
#if (USE_NSIH_HEADER == 0)
	reg_value = ((0x1 <<  0)  |						// odt_sel [0]
		     (0x1 <<  1)  |						// odt_en  [1]
		     (0x0 <<  2) |						// io_mode [2]
		     (0xF <<  4)  |						// dq_dqs_drive [7:4]
		     (0x0 <<  8)  |						// extra_oen_clk [8]
//		     (0x0 <<  9)  |						// no_external_dll, legacy [9]
		     (0x1 << 10) |						// dynamic_receiver_en [10]
		     (0x0 << 11) |						// reduce_cmd_latency [11]
		     (HALF_RATE_MODE << 14) |					// half_rate (read-only) [14]
//		     (DDR1P2 << 15) |						// ddr1p2 (lpddr2/3/4) [15]
		     (0xF << 16) |						// adrctrl_drive [19:16]
		     (0xF << 20) |						// clk_drive [23:20]
		     (0x1 << 28) |						// receiver_en [28]
		     (PREAMBLE_DLY << 29));					// preamble_dly [30:29]
#else
	reg_value = ((0x1 <<  0)  |						// odt_sel [0]
		     (0x1 <<  1)  |						// odt_en  [1]
		     (g_nsih->phy_dsinfo.io_mode <<  2) |			// io_mode [2]
		     (g_nsih->phy_dsinfo.dq_dqs_drive <<  4)  |			// dq_dqs_drive [7:4]
		     (0x0 <<  8)  |						// extra_oen_clk [8]
//		     (0x0 <<  9)  |						// no_external_dll, legacy [9]
		     (0x1 << 10) |						// dynamic_receiver_en [10]
		     (0x0 << 11) |						// reduce_cmd_latency [11]
		     (HALF_RATE_MODE << 14) |					// half_rate (read-only) [14]
//		     (DDR1P2 << 15) |						// ddr1p2 (lpddr2/3/4) [15]
		     (g_nsih->phy_dsinfo.adrctl_drive << 16) |			// adrctrl_drive [19:16]
		     (g_nsih->phy_dsinfo.clk_drive    << 20) |			// clk_drive [23:20]
		     (0x1 << 28) |						// receiver_en [28]
		     (PREAMBLE_DLY << 29));					// preamble_dly [30:29]
#endif
//	reg_value = 0x50ff44f3;							// [DEBUG]
	reg_write_phy(PHY_PAD_CTRL, reg_value);

	/* Step XX. Set the VREF_TRAINING */
	for (i = 0; i < MEM_STRB_WIDTH; i++) {
		reg_write_phy(PHY_LANE_SEL, (VREF_WIDTH * i));
		reg_value = ((0x0 <<  0) |					// vref_training
			    (0x0 <<  1) |					// update_ddr_verf
			    (VREF_OUTPUT_ENABLE <<  2) |			// vref_output_enable
			    (VREF_VALUE << 4) |					// vref_value [4: ]
			    (0x0 << 11) |					// ddr_vref_range [11]
			    (0x0 << 12) |					// ddr_vref_value [17:12]
			    (0x2 << 20));					// tccd_l [22: 20]
		reg_write_phy(VREF_TRAINING, reg_value);
	}

	// ALL IO configuration registers should be setup PRIOR to starting IO calibration
	reg_write_phy(UNIQUIFY_IO_1, 1);					// Start the automatic calibration

	// Default trim setting for Analog DLL
	reg_write_phy(PHY_DLL_INCR_TRIM_3, 0);					// Decrementing the trim because of initial delay in Analog DLL
	reg_write_phy(PHY_DLL_INCR_TRIM_1, 0);					// Decrementing the trim because of initial delay in Analog DLL

	for (i = 0; i < MEM_STRB_WIDTH; i++) {
		reg_write_phy(PHY_LANE_SEL, (SLV_DLY_WIDTH * i));
		reg_write_phy(PHY_DLL_TRIM_1,   5);
		reg_write_phy(PHY_DLL_TRIM_3, 0xA);
	}

	// SCL_WINDOW_TRIM
	reg_value = (INITIAL_DCAP_CLK_DLY <<  0) |				//winA_extra_margin. Set to INITIAL_DCAP_CLK_DLY to compensate for initial delay on data_capture_clk delay line. [0+:`MAS_DLY_WIDTH - 1]
		    (0x0 <<  8) |						//winB_extra_margin [8+:`MAS_DLY_WIDTH - 1]
		    ((0x7 - INITIAL_DCAP_CLK_DLY) <<  16) |			//incr_winA_val. Subtract INITIAL_DCAP_CLK_DLY to compensate for initial delay on data_capture_clk delay line. [16+:`MAS_DLY_WIDTH - 1]
		    ((0x7 + INITIAL_DCAP_CLK_DLY) <<  24);			//incr_winB_val. Add INITIAL_DCAP_CLK_DLY to compensate for initial delay on data_capture_clk delay line. [24+:`MAS_DLY_WIDTH - 1]
	reg_write_phy(SCL_WINDOW_TRIM, reg_value);

	// Unreset the Analog DLL
	reg_write_phy(UNQ_ANALOG_DLL_1, 0);

	// DYNAMIC_IE_TIMER
	reg_value = (0x1A <<  0) |						//dynamic_ie_timer_val - value calculated based on biggest possible RL and burst length (LPDDR4-4266) // reg[0:9]
		    (0x0  << 16);						//enable_ctrl_ie
	reg_write_phy(DYNAMIC_IE_TIMER, reg_value);


	//SCL_CONFIG_1
	reg_value = ((0x1  <<  0) |						//burst8
		     (0x0  <<  1) |						// ddr2
		     (0x0  <<  2) |						// ddr3
		     (0x0  <<  3) |						//dly_dfi_phyupd_ack
		     (round_up(rl_d / 2) << 4) |				//rd_cas_latency [4:7]
		     (0x0  <<  8) |						//additive_latency [8:11]
		     (0x0  << 12) |						//local_odt_ctrl [12:15]
		     (0x0  << 16) |						//ddr_odt_ctrl_rd [16+:`MEM_CHIP_SELECTS]
		     (0x0  << 24));						//ddr_odt_ctrl_wr [24+:`MEM_CHIP_SELECTS]

	if (g_nsih->dii.chip_num >= 2)
		reg_value |= (0x3 << 24);					//ddr_odt_ctrl_wr [24+:`MEM_CHIP_SELECTS]
	else
		reg_value |= (0x1 << 24);					//ddr_odt_ctrl_wr [24+:`MEM_CHIP_SELECTS]
	reg_value &= ~(0x7 <<  0);

#if defined(DDR2)
	reg_value |= ((0x1  <<  0) |						//burst8
		      (0x1  <<  1) |						// ddr2
		      (0x0  <<  2));						// ddr3
#elif defined(DDR3)
	reg_value |= ((0x1  <<  0) |						//burst8
		      (0x0  <<  1) |						// ddr2
		      (0x1  <<  2));						// ddr3
#elif defined(DDR4)
	reg_value |= ((0x1  <<  0) |						//burst8
		      (0x0  <<  1) |						// ddr2
		      (0x0  <<  2));						// ddr3
#elif defined(LPDDR2)
	reg_value |= ((0x1  <<  0) |						//burst8
		      (0x1  <<  1) |						// ddr2
		      (0x0  <<  2));						// ddr3
#elif defined(LPDDR3)
	reg_value |= ((0x1  <<  0) |						//burst8
		      (0x1  <<  1) |						// ddr2
		      (0x0  <<  2));						// ddr3
#endif
 	reg_write_phy(SCL_CONFIG_1, reg_value);

	// SCL_CONFIG_2
	reg_value = ((0x1 <<  0) |						// scl_test_cs
		     (round_up(wl_d / 2) << 8) |				// wr_cas_latency
		     (0x0 << 12) |						//double_ref_dly
		     (DLY_DFI_WRDATA << 24) |					//dly_dfi_wrdata
		     (0x0 << 25) |						//wr_cas_latency_overflow
		     (0x0 << 26) |						//rd_cas_latency_overflow
#if defined(LPDDR2) || defined(LPDDR3)
		     (0x1 << 29) |						//lpddr2
#endif
		     (SWAP_PHASE << 31));					//swap_phase_2t_timing
	reg_write_phy(SCL_CONFIG_2, reg_value);

	// SCL_CONFIG_3
	// Mask lanes that SCL should NOT run on
	reg_value = mmio_read_32(DPHY_BASE_ADDR + SCL_CONFIG_3);
	reg_value &= ~((SCL_LANES * 4) << 0);					//scl_lanes_mask
	reg_write_phy(SCL_CONFIG_3, reg_value);

	// DYNAMIC_WRITE_BIT_LVL
	// Program crucial timing values for write side bit leveling, please see spec on how to choose values
	reg_value = (READ_LAT << 12) |						// bit_lvl_wr_side_read_lat
		    (TRIM_LAT <<  4) |						// bit_lvl_wr_side_trim_lat
		    (BIT_LVL_WR_DYNAMIC <<  0);					// dynamic_write_bit_leveling
//	reg_value = 0x0002B220;							// [DEBUG]
	reg_write_phy(DYNAMIC_WRITE_BIT_LVL, reg_value);

	// Half Rate Mode (Must)
	reg_value = (((WL % 2) == 0) && SWAP_PHASE);				//dly_dfi_wrdata_en_start
	reg_write_phy(SCL_CONFIG_4, reg_value);

	// SCL_GATE_TIMING
#if defined(DDR3)
	reg_value = ((0x0 <<  0) |						//invert_cycle_cnt_modified_clk
		     (0x0 <<  1) |						//del_dqs_gate
		     (0xF <<  4));						//dqs_gate_trim
#elif defined(DDR4)
	reg_value = ((DDR4_2_CYCLE_PREAMBLE <<  0) |				//invert_cycle_cnt_modified_clk
		     (DDR4_2_CYCLE_PREAMBLE <<  1) |				//del_dqs_gate
		     (0xA << 4));						//dqs_gate_trim
#endif
	reg_write_phy(SCL_GATE_TIMING, reg_value);

	// WRLVL_DYN_ODT: Address 7'h54
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = ((0x0         <<  0) |
		     ((WL - 5)    <<  3) |
		     (0x0         <<  9) |					//turn off Rtt_WR before write leveling
		     (0x0         << 16) |
		     ((WL - 5)    << 19) |
		     (DDR3_RTT_WR << 25));					//enable Rtt_WR after write leveling
//	reg_value = 0x02200020;							// [DEBUG]
	reg_write_phy(WRLVL_DYN_ODT, reg_value);
#elif defined(DDR4)
	reg_value = ((CWL_DDR4     <<  3) |
		     (0x3          <<  6) |					//auto self refresh
		     (0x0          <<  9) |					//turn off Rtt_WR before write leveling
		     (CWL_DDR4     << 19) |
		     (0x3          << 22) |
		     (DDR4_RTT_WR  << 25));					//enable Rtt_WR after write leveling
	reg_write_phy(WRLVL_DYN_ODT, reg_value);
#elif defined(LPDDR3)
	reg_write_phy(WRLVL_DYN_ODT, 0x020000);
#endif
#else

#if defined(DDR3)
	int DDR3_RTT_WR = g_nsih->dsinfo.mr2_rtt_wr;
	reg_value = ((0x0         <<  0) |
		     ((WL - 5)    <<  3) |
		     (0x0         <<  9) |					//turn off Rtt_WR before write leveling
		     (0x0         << 16) |
		     ((WL - 5)    << 19) |
		     (DDR3_RTT_WR << 25));					//enable Rtt_WR after write leveling

	reg_write_phy(WRLVL_DYN_ODT, reg_value);
#elif defined(DDR4)
	int DDR4_RTT_WR = g_nsih->dsinfo.mr2_rtt_wr;
	int CWL_DDR4 = WL + 0;

	reg_value = ((CWL_DDR4     <<  3) |
		     (0x3          <<  6) |					//auto self refresh
		     (0x0          <<  9) |					//turn off Rtt_WR before write leveling
		     (CWL_DDR4     << 19) |
		     (0x3          << 22) |
		     (DDR4_RTT_WR  << 25));					//enable Rtt_WR after write leveling
	reg_write_phy(WRLVL_DYN_ODT, reg_value);
#elif defined(LPDDR3)
	reg_write_phy(WRLVL_DYN_ODT, 020000);
#endif


#endif

	// WRLVL_ON_OFF
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = ((((DDR3_DIC >> 0) & 0x1)	  <<  1) |
		    (((DDR3_RTT_NOM >> 0) & 0x1) <<  2) |
		    (((DDR3_DIC >> 1) & 0x1)     <<  5) |
		    (((DDR3_RTT_NOM >> 1) & 0x1) <<  6) |
		    (0x0 <<  7) |					 	//disable write leveling
		    (((DDR3_RTT_NOM >> 2) & 0x3) <<  9) |
		    (((DDR3_DIC >> 0) & 0x1)     << 17) |
		    (((DDR3_RTT_NOM >> 0) & 0x1) << 18) |
		    (((DDR3_DIC >> 1) & 0x1)     << 21) |
		    (((DDR3_RTT_NOM >> 1) & 0x1) << 22) |
		    (0x1 <<  23) |					 	//enable write leveling
		    (((DDR3_RTT_NOM >> 2) & 0x3) << 25));
#elif defined(DDR4)
	reg_value  = (0x1 <<  0) |						// always enable DLL
		     ((DDR4_DIC & 0x3)		<<  1) |
		     ((DDR4_AL & 0x3)		<<  3) |
		     (0x1			<<  7) |			//enable write leveling
		     ((DDR4_RTT_NOM & 0x3)	<<  8) |
		     ((DDR4_DIC & 0x3)		<< 17) |
		     ((DDR4_AL & 0x3)		<< 19) |
		     (0x1			<< 23) |
		     ((DDR4_RTT_NOM & 0x3)	<< 24);

#elif defined(LPDDR3)
	reg_value = (((0x10 <<  0) | (RL - 2)) <<  0) |				// MR2[4] needs to be set
		    (((0x0  <<  7) | (RL - 2)) << 16);
	if (RL >= 16)
		reg_value |= 0x10 << 16;
#endif

#else // #if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	int DDR3_DIC = g_nsih->dsinfo.mr1_ods;
	int DDR3_RTT_NOM = g_nsih->dsinfo.mr1_rtt_nom;

	reg_value = ((((DDR3_DIC >> 0) & 0x1)	  <<  1) |
		    (((DDR3_RTT_NOM >> 0) & 0x1) <<  2) |
		    (((DDR3_DIC >> 1) & 0x1)     <<  5) |
		    (((DDR3_RTT_NOM >> 1) & 0x1) <<  6) |
		    (0x0 <<  7) |					 	//disable write leveling
		    (((DDR3_RTT_NOM >> 2) & 0x3) <<  9) |
		    (((DDR3_DIC >> 0) & 0x1)     << 17) |
		    (((DDR3_RTT_NOM >> 0) & 0x1) << 18) |
		    (((DDR3_DIC >> 1) & 0x1)     << 21) |
		    (((DDR3_RTT_NOM >> 1) & 0x1) << 22) |
		    (0x1 <<  23) |					 	//enable write leveling
		    (((DDR3_RTT_NOM >> 2) & 0x3) << 25));
#elif defined(DDR4)
	int DDR4_DIC = g_nsih->dsinfo.mr1_ods;
	int DDR4_RTT_NOM = g_nsih->dsinfo.mr1_rtt_nom;
	int DDR4_AL = 0;

	reg_value  = (0x1 <<  0) |						// always enable DLL
		     ((DDR4_DIC & 0x3)		<<  1) |
		     ((DDR4_AL & 0x3)		<<  3) |
		     (0x1			<<  7) |			//enable write leveling
		     ((DDR4_RTT_NOM & 0x3)	<<  8) |
		     ((DDR4_DIC & 0x3)		<< 17) |
		     ((DDR4_AL & 0x3)		<< 19) |
		     (0x1			<< 23) |
		     ((DDR4_RTT_NOM & 0x3)	<< 24);

#elif defined(LPDDR3)
	reg_value = (((0x10 <<  0) | (RL - 2)) <<  0) |			// MR2[4] needs to be set
		    (((0x0  <<  7) | (RL - 2)) << 16);
	if (RL >= 16)
		reg_value |= 0x10 << 16;
#endif

#endif
//	reg_value = 0x00c60046;							// [DEBUG]
	reg_write_phy(WRLVL_ON_OFF, reg_value);

	float UNIQUIFY_DLY_CHAIN_STEP = 0.010;

	// Delay addr/ctrl lines by an extra 1/2 DDR clock period for write leveling
	#if defined(DDR3) || defined(DDR4) || defined(LPDDR3)
		extra_addrctrl_dly = (int)(round_up(round_up(dram_clk_period / 4)
					/ (UNIQUIFY_DLY_CHAIN_STEP * 2000)));	// UNIQUIFY_DLY_CHAIN_STEP is in ns
	#elif defined(LPDDR2)
		extra_addrctrl_dly = 6;						// small delay since LPDDR2 does not have WR leveling
	#endif
	printf("dram_clk_period: %d, extra_addrctrl_dly: %d \r\n", dram_clk_period, extra_addrctrl_dly);
	// Add delay to addr/ctrl lines for write leveling
	// PHY_DLL_ADRCTRL
	reg_value = (extra_addrctrl_dly << 0) |					//dlls_trim_adrctrl
		    (0x0 <<  8) |						//override_adrctrl
		    (0x1 <<  9);						//incr_dly_adrctrl
	reg_write_phy(PHY_DLL_ADRCTRL, reg_value);

	int clk_num;
	for (clk_num = 0; clk_num < MEM_CLOCKS; clk_num++) {
		reg_write_phy(PHY_LANE_SEL , clk_num * (SLV_DLY_WIDTH + 2));
//		reg_write_phy(PHY_DLL_TRIM_CLK, ((1 << SLV_DLY_WIDTH) + 1) |	//set increment delay bit
//			(extra_addrctrl_dly << 0) );
		reg_write_phy(PHY_DLL_TRIM_CLK, 0x90);
	}

	// Reduce extra_addrctrl_dly for LPDDR3 CA bus to center clock edge about CA
#if defined(LPDDR2)
	extra_addrctrl_dly = 3;
#endif

	// PHY_DLL_RECALIB
	reg_value = ((extra_addrctrl_dly <<  0) |				//dlls_trim_adrctrl_ma
		    (0x10 <<  8) |						//recalib_cnt
		    (0x0  << 26) |						//disable_recalib
		    (0x1  << 27) |						//incr_dly_adrctrl_ma
		    (0xA  << 28));						//ctr_start_val
	reg_write_phy(PHY_DLL_RECALIB, reg_value);

	// Ensure IO calibration values are updated after dfi_init_complete is asserted");
	reg_value = reg_read_phy(PHY_DLL_ADRCTRL);				// 0x128
	while ((reg_value & 0xFF000000) == 0xFF000000) {
		reg_value  = reg_read_phy(PHY_DLL_ADRCTRL);
	}

	reg_value = reg_read_phy(UNIQUIFY_IO_1); 				// 0x170
	while ((reg_value & 0x00000002) != 0x00000002) {
		reg_value  = reg_read_phy(UNIQUIFY_IO_1);
	}

#if defined(LPDDR2)
	// Increment clk trim by a few steps to center it on CA
	for (i = 0; i < MEM_CLOCKS; i++) {
		reg_write_phy(PHY_LANE_SEL , i * (SLV_DLY_WIDTH + 2));
		// Set increment delay bit
		reg_write_phy(PHY_DLL_TRIM_CLK, (1 << (SLV_DLY_WIDTH + 1) ) | 6);
	}
	// Since clk trim was incremented, increment CS/CKE delay also
	reg_write_phy(PHY_DLL_ADRCTRL, 0x200 | 6 );
#endif

	// Manually update the UCAL I/O.
	reg_value = (1 << 2);
	reg_set_phy(UNIQUIFY_IO_1, reg_value);

	reg_value = mmio_read_32(DPHY_BASE_ADDR + UNIQUIFY_IO_1);
	reg_value &= ~(1 << 2);
	reg_write_phy(UNIQUIFY_IO_1, reg_value);

	// SCL_LATENCY
	reg_value = ((0x6 <<  0) |						//capture_clk_dly
		     (MAIN_CLK_DLY <<  4) |					//main_clk_dly
		     (0x0 <<  8) |						//ext_loopback
		     (0x0 <<  9) |						//dqs_gate_disabled
		     (SCL_WAIT_LATENCY <<  12));				//scl_calib_wait_latency
	reg_write_phy(SCL_LATENCY, reg_value);
}
