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
#include <sysheader.h>

#include <dphy.h>
#include <dctrl.h>
#include <ddr4_sdram.h>
#include <ddr4_ac_timing.h>

void enter_self_refresh(void)
{
	// todo
}

void exit_self_refresh(void)
{
	// todo
}

int mem_init_seq_ddr4 (unsigned int is_resume)
{
	union DDR4_SDRAM_MR MR0, MR1, MR2, MR3;
	union DDR4_SDRAM_MR MR4, MR5, MR6;

	struct ddr4_timing_task *ac_timing =
		((struct ddr4_timing_task *)&g_nsih->dii.ac_timing);
	struct ddr4dev_drvdsinfo *dsinfo =
		((struct ddr4dev_drvdsinfo *)&g_nsih->dsinfo);

	int nCL  = (ac_timing->RL + 0);
	int nCWL = (ac_timing->WL + 0);
	int mr0_cl;

	int wait_cycle;
	int ctrl_status;
//	int is_pd;

	int reg_value;
	int ret = 0;

	is_resume = is_resume;

#ifdef UNIQUIFY_AC_SHARING
	mmio_write_32((DCTRL_BASE_ADDR + MEM_START), 0x1);
#endif

	/* Step xx. Check the Controller(FSM) Status  */
	ctrl_status = ((mmio_read_32(DCTRL_BASE_ADDR + MEM_WIDTH) >> 12) & 0xF);
	if (ctrl_status == DRAM_NORMAL_OPERATION)
		is_resume = false;
	else if (ctrl_status == DRAM_SELF_REFRESH)
		is_resume = true;
//	else if (ctrl_status == DRAM_POWER_DOWN)
//		is_pd = true;

	/* Step 01. Power ON. Assert RESET# and ensure CKE is LOW at least max of 10ns or 5tCK */
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x11);

	host_cmd_load(HOST_CMD_1_0, CMD_CKE_LOW, CMD_CKE_LOW2, (5 * 2048));
	host_cmd_load(HOST_CMD_1_1, CMD_CKE_LOW, CMD_CKE_LOW2, 0);

	/* Issue 2 commands to the intialization state m/c and wait for it to be done. */
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x11);
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

 	/* Step 02. DRAM RESET# pin must be LOW for at least 200us before DRAM is brought out of reset
  	  * We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
  	  * WARNING: 200 us is required before RST_N goes inactive.
  	  * Insert software function to wait 200us here.
  	  */
	udelay(200);

	/* 03. CKE must be LOW 10ns prior to RESET# transitioning HIGH A`lready taken care of. So Deasserting RESET# */
	mmio_write_32((DCTRL_BASE_ADDR + MEM_WIDTH), (1 << 4));			// dif_reset_n

	/* 04. After RESET# is deasserted, wait 500us with CKE LOW
	  * We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	  * WARNING: 500 us is required after RST_N goes inactive before CKE goes active.
	  * Insert software function to wait 500us here..
	  */
	udelay(500);

	/* Step 05. CKE may be brought HIGH and only NOP or DES commands may be issued for TXPR */
	wait_cycle = (ac_timing->tXPR * 2);
	host_cmd_load(HOST_CMD_1_0, CMD_IDLE_NOP2, CMD_IDLE_NOP, wait_cycle);

	// Mode Register
	wait_cycle = (ac_timing->tMRD * 2);

	/* Step 06. Issue an MRS command to MR3. */
	MR3.REG			= 0;
	MR3.MR3.MPR_READ_FORMAT = 0;						// MPR Read Format: Serial
	MR3.MR3.WCL_CRC_DM	= 0;						// Write command latency for CRC+DM: 4nCK
	MR3.MR3.FGR		= 0;						// Find Granularity Refresh Mode: Normal
	MR3.MR3.TSR		= 0;						// Temperature Readout: Disabled
	MR3.MR3.PDA		= 0;						// Per DRAM addressability: Disabled
	MR3.MR3.GM		= 0;						// Geradown mode: 1/2 Rate
	MR3.MR3.MPR		= 0;						// MPR Operation: Normal
	MR3.MR3.MPR_RF		= 0;						// MPR Page Selection: Page00
	host_cmd_load_mr(HOST_CMD_1_1, SDRAM_MODE_REG_MR3, MR3.REG, wait_cycle);

	/* Step 07. Issue and MRS Command to MR6. */
	MR6.REG			= 0;
	MR6.MR6.ntCCD_L		= ac_timing->tCCD_L - 4;					// tCCD_L (?)
	MR6.MR6.VrefDQ_Enable	= 0;						// VrefDQ Training: Disabled
	MR6.MR6.VrefDQ_TRange	= 0;						// VrefDQ Training: Range 1
	MR6.MR6.VrefDQ_Value	= 0;						// VrefDQ Training Value: 60.00%
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR6, MR6.REG, wait_cycle);

	/* Step 08. Issue an MRS Command to MR5 */
	MR5.REG			= 0;
	MR5.MR5.READ_DBI	= 0;						// Read DBI: Disabled
	MR5.MR5.WRITE_DBI	= 0;						// Write DBI: Disabled
	MR5.MR5.DATA_MASK	= 0;						// Data Mak: Enabled
	MR5.MR5.CA_PE		= 0;						// CA Parity: Persistent Error: Disabled
	MR5.MR5.RTT_PARK	= dsinfo->mr5_rtt_park;//CONFIG_DRAM_MR5_RTT_PARK;			// RTT_PARK
	MR5.MR5.ODT_IBPD	= 0;						// ODT Input Buffer during PD: (0:Actived, 1:Deactivated)
	MR5.MR5.CA_PES		= 0;						// CA Parity Error Status: (0: Clear, 1:Error)
	MR5.MR5.CRC_EC		= 0;						// CRC Error Clear: (0: Clear, 1:Error)
	MR5.MR5.CA_PLM		= 0;						// CA Parity Latency: (0: Disabled, See Table 12)
	host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR5, MR5.REG, wait_cycle);

	/* Step 09. Issue an MRS command to MR4 */
	MR4.REG			= 0;
	MR4.MR4.WP		= 0;						// Write Preamble (0: 1nCk, 1:2nCK)
	MR4.MR4.RP		= 1;//DDR4_2_CYCLE_PREAMBLE;			// Read Preamble
	MR4.MR4.RPTM		= 0;						// Read Preamble Training Mode: Disabled
	MR4.MR4.SRA		= 0;						// Self-Refresh Abort: Disabled
	MR4.MR4.CS2CMD_CL	= 0;						// CS to CMD/ADDR Latency: Dsiabled
	MR4.MR4.IVM		= 0;						// Internal vREF Monitor: Disabled
	MR4.MR4.TCRM		= 0;//DDR4_TEMP_CTRL_REF;				// Temperature Controlled Refresh (0:Disabled, 1:Enabled)
	MR4.MR4.TCRR		= 0;//DDR4_TEMP_CTRL_REF_RANGE;			// Temperature Controlled Refresh Range (0:Disabled, 1:Enabled)
	MR4.MR4.MPD_MODE	= 0;//DDR4_MAX_PD;					// Max Power Down Mode
	host_cmd_load_mr(HOST_CMD_1_4, SDRAM_MODE_REG_MR4, MR4.REG, wait_cycle);

	/* Step 10. Issue an MRS command MR2 */
	MR2.REG			= 0;
	MR2.MR2.RTT_WR		= dsinfo->mr2_rtt_wr;//CONFIG_DRAM_MR2_RTT_WR;	// RTT_WR
	MR2.MR2.ASR		= 0;//AUTO_SELF_REFRESH;			// Auto Self-Refresh
	MR2.MR2.nCWL		= (nCWL - 5);					// CAS Write Latency (See Table 6) // ???
	host_cmd_load_mr(HOST_CMD_1_5, SDRAM_MODE_REG_MR2, MR2.REG, wait_cycle);

	/* Step 11. Issue an MRS command MR1 */
	MR1.REG			= 0;
	MR1.MR1.RTT_Nom		= dsinfo->mr1_rtt_nom;//CONFIG_DRAM_MR1_RTT_Nom;// RTT_Nomal
	MR1.MR1.nAL		= 0;						// Additive Latency
	MR1.MR1.DLL		= 1;						// DLL: Enabled
	host_cmd_load_mr(HOST_CMD_1_6, SDRAM_MODE_REG_MR1, MR1.REG, wait_cycle);

	// Step XX. Calcurate the CL(Cas Latency)
	if ((nCL >= 5) && (11 >= nCL))
		mr0_cl = ((nCL -  4) << 1) + 1;
	else if ((nCL >= 11) && (16 >= nCL))
		mr0_cl = ((nCL - 12) << 1) + 1;
	else
		mr0_cl = 0;

	/* Step 12. Issue an MRS command MR0 */
	MR0.REG			= 0;
	MR0.MR0.WR		= 0;///??					// Write Reconvery and Raead to Precharge
	MR0.MR0.DLL		= 1;						// DLL Reset : Yes
	MR0.MR0.TM		= 0;						// Test Mode: No
	MR0.MR0.BT		= 0;						// Read Burst Type: Sequential
	MR0.MR0.CL0		= ((mr0_cl >> 0) & 0x1);			// Cas Latency 0
	MR0.MR0.CL1		= ((mr0_cl >> 1) & 0x7);			// Cas Latency 1
	MR0.MR0.BL		= DDR4_BURST_LENGTH;				// Burst Length: 8
	wait_cycle = (ac_timing->tMOD * 2);
	host_cmd_load_mr(HOST_CMD_1_7, SDRAM_MODE_REG_MR0, MR0.REG, wait_cycle);

	/*
	  * Step 13. Issue a ZQCL command to calibrate Rtt and Ron values for the process voltage temperature
	  * (PVT). Prior to normal operation, tZQinit must be satisfied.
	  */
	wait_cycle = (ac_timing->tZQinit * 2);
	host_cmd_load(HOST_CMD_1_5, CMD_ZQ_CAL2, CMD_ZQ_CAL1, wait_cycle);

	/* Step 14. Issue 8 Command to the initialization m/c and wait for it to be done. */
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x18);
	mmio_set_32  ((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x10);

	reg_value = ((1 << 1) |							// start_mem_init
		     (1 << 0));							// mem_init_done
	mmio_write_32((DCTRL_BASE_ADDR + MEM_START), reg_value);		// Set the Initialization complete bit.

#ifdef UNIQUIFY_AC_SHARING
	mmio_write_32((DCTRL_BASE_ADDR + MEM_START), 0x1);			//Set initialization complete bit for slv controller
#endif

	return ret;
}

/*************************************************************
 * Must be nxp3220
 * Chage teh PLL(Phase Locked Loop) sequence in nxp3220
 * must go through the following steps:
 *
 * Step 01. Set the DPHY(DDR-PHY).
 * Step 02. Set the DCTRL(DDR-CTRL).
 * Step 03. Set the DRAM Device.
 * 		-> Set the Initialize & Mode Register.
 *		->
 * Step 04. Set the Leveling & Calibration.
 *		-> Write-Leveling.
 *		-> Bit-Leveling.
 *************************************************************/
 int ddr4_initialize (int is_resume)
{
	/* step xx. set the dphy */
	phy_set_init_values();
	/* step xx. set the dctrl */
	ctrl_set_init_values();
	/* step xx. set the dram-device */
	mem_init_seq_ddr4(is_resume);

	/* step xx. write-leveling */
	hw_write_leveling();
	/* step xx. bit-leveling */
	hw_bit_leveling();

	return 0;
}
