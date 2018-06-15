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
#include <dctrl.h>
#include <ddr3_sdram.h>
#include <ddr3_ac_timing.h>

void enter_self_refresh(void)
{
	union DDR3_SDRAM_MR MR;

	int c_ctrl_st = 0;
	int wait_cycle = g_nsih->dii.ac_timing.tCKESR;

	/* Step 01. Send PALL Command */
	host_cmd_load(HOST_CMD_1_0, 0, CMD_PRE_ALL, wait_cycle);

	/* Step 02. (DRAM) ODT OFF */
	MR.REG          = 0;
	MR.MR2.RTT_WR   = 0; 							// 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm)
	MR.MR2.SRT      = 0;							// self refresh normal range, if (ASR == 1) SRT = 0;
	MR.MR2.ASR      = 1;							// auto self refresh enable
#if (USE_NSIH_HEADER == 1)
	MR.MR2.CWL	= (g_nsih->dii.ac_timing.WL + 0);	// WL + AL
#else
	MR.MR2.CWL      = (nCWL - 5);
#endif
	host_cmd_load_mr(HOST_CMD_1_1, SDRAM_MODE_REG_MR0, MR.REG, wait_cycle);

	/* Step 03. (DRAM) Set the Drive Strength */
	MR.REG          = 0;
	MR.MR1.DLL      = 1;							// 0: Enable, 1 : Disable
#if (USE_NSIH_HEADER == 1)
	MR.MR1.AL	= 0;
	MR.MR1.ODS0	= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR.MR1.ODS1	= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
	MR.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
	MR.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
#else
	MR.MR1.AL       = MR1_nAL;
	MR.MR1.ODS1     = (CONFIG_DRAM_MR1_ODS >> 1) & 1;
	MR.MR1.ODS0     = (CONFIG_DRAM_MR1_ODS >> 0) & 1;
	MR.MR1.RTT_Nom2 = (CONFIG_DRAM_MR1_RTT_Nom >> 2) & 1;
	MR.MR1.RTT_Nom1 = (CONFIG_DRAM_MR1_RTT_Nom >> 1) & 1;
	MR.MR1.RTT_Nom0 = (CONFIG_DRAM_MR1_RTT_Nom >> 0) & 1;
#endif
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR1, MR.REG, wait_cycle);

	/* Step 04. Enter Self-Refresh Command */
	host_cmd_load(HOST_CMD_1_3, 0, CMD_SELF_REFE,
			g_nsih->dii.ac_timing.tXPR * 2);
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x13);
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	/* Step 05. Check the Sel-Refresh State (FSM) */
	do {
		c_ctrl_st = ((mmio_read_32(DPHY_BASE_ADDR + MEM_WIDTH) >> 12) & 0x7);
	} while (c_ctrl_st != DRAM_SELF_REFRESH);
}

void exit_self_refresh(void)
{
	union DDR3_SDRAM_MR MR;

	int c_ctrl_st = 0;
	int wait_cycle = g_nsih->dii.ac_timing.tXS;

	/* Step 01. Send PALL Command */
	host_cmd_load(HOST_CMD_1_0, 0, CMD_PRE_ALL, wait_cycle);

	/* Step 02. (DRAM) Set the Drive Strength */
	MR.REG          = 0;
	MR.MR1.DLL      = 1;							// 0: Enable, 1 : Disable
#if (USE_NSIH_HEADER == 1)
	MR.MR1.AL	= 0;
	MR.MR1.ODS0	= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR.MR1.ODS1	= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
	MR.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
	MR.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
#else
	MR.MR1.AL       = MR1_nAL;
	MR.MR1.ODS1     = (CONFIG_DRAM_MR1_ODS >> 1) & 1;
	MR.MR1.ODS0     = (CONFIG_DRAM_MR1_ODS >> 0) & 1;
	MR.MR1.RTT_Nom2 = (CONFIG_DRAM_MR1_RTT_Nom >> 2) & 1;
	MR.MR1.RTT_Nom1 = (CONFIG_DRAM_MR1_RTT_Nom >> 1) & 1;
	MR.MR1.RTT_Nom0 = (CONFIG_DRAM_MR1_RTT_Nom >> 0) & 1;
#endif
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
	host_cmd_load_mr(HOST_CMD_1_1, SDRAM_MODE_REG_MR1, MR.REG, wait_cycle);

	/* Step 03. (DRAM) ODT OFF */
	MR.REG          = 0;
	MR.MR2.RTT_WR   = 0; 							// 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm)
	MR.MR2.SRT      = 0;							// self refresh normal range, if (ASR == 1) SRT = 0;
	MR.MR2.ASR      = 1;							// auto self refresh enable
#if (USE_NSIH_HEADER == 1)
	MR.MR2.CWL	= (g_nsih->dii.ac_timing.WL + 0);	// WL + AL
#else
	MR.MR2.CWL      = (nCWL - 5);
#endif
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR2, MR.REG, wait_cycle);

	/* Step 04. Exit the Self-Refresh Command */
	host_cmd_load(HOST_CMD_1_0, 0, CMD_SELF_REFX, wait_cycle);

	/* Step 05. Check the Normal State (FSM) */
	do {
		c_ctrl_st = ((mmio_read_32(DPHY_BASE_ADDR + MEM_WIDTH) >> 12) & 0x7);
	} while (c_ctrl_st != DRAM_NORMAL_OPERATION);
}

void mem_init_seq_ddr3 (int is_resume)
{
	union DDR3_SDRAM_MR MR0, MR1, MR2, MR3;

	int mr0_cl, mr0_wr;

	int nCL  = (g_nsih->dii.ac_timing.RL + 0);					// RL + AL  tCK
	int nCWL = (g_nsih->dii.ac_timing.WL + 0);					// WL + AL  tCK
	int nWR  = (g_nsih->dii.ac_timing.tWR * 2);

	int wait_cycle;

	int reg_value;

	mmio_write_32((DCTRL_BASE_ADDR + MEM_START), 0x1);			//Kick off controller initialization state m/c

	// Step 01. Power ON. Assert RESET# and ensure CKE is LOW at least max of 10ns or 5tCK
	mmio_clear_32((DCTRL_BASE_ADDR + MEM_WIDTH), 1 << 4);			// Bit 4 controls dfi_reset_n
					// 10000ps / dram_clk
	host_cmd_load(HOST_CMD_1_0, 0, CMD_CKE_LOW, 5);//max(8000/2500, 5));
	host_cmd_load(HOST_CMD_1_1, 0, CMD_CKE_LOW, 0);

	/* Issue 2 commands to the intialization state m/c and wait for it to be done */
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x11);
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);
	/* Issue 2 commands to the intialization state m/c and wait for it to be done. */

 	/* Step 02. DRAM RESET# pin must be LOW for at least 200us before DRAM is brought out of reset
  	  * We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
  	  * WARNING: 200 us is required before RST_N goes inactive.
  	  * Insert software function to wait 200us here.
  	  */
	udelay(200000);

	/* 03. CKE must be LOW 10ns prior to RESET# transitioning HIGH A`lready taken care of. So Deasserting RESET# */
	mmio_set_32((DCTRL_BASE_ADDR + MEM_WIDTH), (1 << 4)|(1 << 1));		// dif_reset_n

	/* 04. After RESET# is deasserted, wait 500us with CKE LOW
	* We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	* WARNING: 500 us is required after RST_N goes inactive before CKE goes active.
	* Insert software function to wait 500us here..
	*/
	udelay(500000);

	/* Step 05. CKE may be brought HIGH and only NOP or DES commands may be issued for tXPR */
	int tXPR_MAX = (g_nsih->dii.ac_timing.tRFC + 10);
	host_cmd_load(HOST_CMD_1_0, 0, CMD_IDLE_NOP, tXPR_MAX);

	// Mode Register
	wait_cycle = (g_nsih->dii.ac_timing.tMRD);

	/* Step 06. Issue an MRS command to MR2. */
	MR2.REG			= 0;
	MR2.MR2.RTT_WR		= g_nsih->dsinfo.mr2_rtt_wr;			// Dynamic ODT (00: Rtt disabled, 01: RZQ/4, 10: RZQ/2, 11: reserved)
	MR2.MR2.CWL		= (nCWL - 5);					// CAS Write Latency (000: 5CK(tCK>=2.5ns), 001: 6CK(2.5ns>tCK>=1.875ns, 010: 7CK(1.875ns>tCK>=1.25ns), 011: 8CK(1.5ns>tCK>=1.25ns), 100: 9CK(1.25ns>tCK>=1.07ns))
	host_cmd_load_mr(HOST_CMD_1_1, SDRAM_MODE_REG_MR2, MR2.REG, wait_cycle);

	// Step 07. Issue an MRS command to MR3.
	MR3.REG			= 0;
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR3, MR3.REG, wait_cycle);

	// Step 08. Issue an MRS command to MR1.
	MR1.REG			= 0;
	MR1.MR1.DLL		= 0;						// 0: Enable, 1: Disable
	MR1.MR1.AL		= 0;
	MR1.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
	MR1.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
	MR1.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
	MR1.MR1.ODS0		= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR1.MR1.ODS1		= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR1.MR1.QOff		= 0;
	MR1.MR1.WL		= 0;
	host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR1, MR1.REG, wait_cycle);

	// Step 09. Issue an MRS command to MR0.
	if ((nCL >= 5) && (11 >= nCL))
		mr0_cl = ((nCL -  4) << 1);
	else if ((nCL >= 12) && (16 >= nCL))
		mr0_cl = (((nCL - 12) << 1) | 0x1);
	else
		mr0_cl = 0;

	if ((nWR >= 5) && (8 >= nWR))
		mr0_wr = (nWR - 4);
	else if ((nWR >= 10) && (nWR <= 14))
		mr0_wr = (nWR / 2);
	else
		mr0_wr = 0;

	MR0.REG 		= 0;
	MR0.MR0.PD		= 1;						// PD_EXIT
	MR0.MR0.WR		= mr0_wr;
	MR0.MR0.DLL		= 1;						// DLL_RESET
	MR0.MR0.CL0		= ((mr0_cl >> 0) & 0x1);
	MR0.MR0.CL1		= ((mr0_cl >> 1) & 0x7);
	host_cmd_load_mr(HOST_CMD_1_4, SDRAM_MODE_REG_MR0, MR0.REG, 0x6);//tMOD);

	/*
	 * Step 10. Issue a ZQCL command to calibrate Rtt and Ron values for the process voltage temperature
	 * (PVT). Prior to normal operation, tZQinit must be satisfied.
	 */
	wait_cycle = g_nsih->dii.ac_timing.tZQinit;
	host_cmd_load(HOST_CMD_1_5, 0x04, CMD_ZQ_CAL, wait_cycle);

	// Step 11. Issue 6 command to the intialization state m/c and wait for it to be done
	mmio_write_32((DCTRL_BASE_ADDR + HOST_CMD_ISSUE), 0x15);
	while (mmio_read_32(DCTRL_BASE_ADDR + HOST_CMD_ISSUE) & 0x10);

	reg_value = ((1 << 1) |							// start_mem_init
		     (1 << 0));							// mem_init_done
	mmio_write_32((DCTRL_BASE_ADDR + MEM_START), reg_value);		// Set the Initialization complete bit.

	// Wait for Analog DLL to lock
	reg_value = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);		// 0x160
	while ((reg_value & 0x00000003) != 0x00000003)
		reg_value  = mmio_read_32(DPHY_BASE_ADDR + UNQ_ANALOG_DLL_2);

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
int  ddr3_initialize (unsigned int is_resume)
{
	/* step xx. set the dphy */
	phy_set_init_values();
	/* step xx. set the dctrl */
	ctrl_set_init_values();

	/* step xx. set the dram-device */
	mem_init_seq_ddr3(is_resume);

	/* step xx. write-leveling */
	hw_write_leveling();
	/* step xx. bit-leveling */
	hw_bit_leveling();

	return 0;
}
