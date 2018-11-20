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
#include <ddr3_sdram.h>
#include <ddr3_ac_timing.h>

/* Extern Functions */
extern void vddpwron_ddr_on(void);

#define RSTRELEASE_DDR_AXI			(0x23000000 + 0x400 + 0x30)
#define RSTENTER_DDR_AXI			(0x23000000 + 0x400 + 0x40)

void dphy_global_reset(void)
{
	unsigned int reg_value = (0x3 << 0);					/* RSTRELEASE_DDR_RST */

	/* DPHY Reset Enter */
	mmio_write_32(RSTENTER_DDR_AXI, reg_value);
	/* Must wait 20ns */
	udelay(20);
	/* DPHY Reset Release */
	mmio_write_32(RSTRELEASE_DDR_AXI, reg_value);
}

void self_refresh_entry(void)
{
	union DDR3_SDRAM_MR MR;
	unsigned int reg_value = 0;
	int c_ctrl_st = 0;

#if 1
	MR.REG          = 0;
	MR.MR2.RTT_WR   = 0; 							/* 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm)	*/
	MR.MR2.SRT      = 0;							/* self refresh normal range, if (ASR == 1) SRT = 0;	*/
	MR.MR2.ASR      = 1;							/* auto self refresh enable				*/
	MR.MR2.CWL	= (g_nsih->dii.ac_timing.WL + 0);			/* WL + AL						*/
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR2,
					MR.REG, g_nsih->dii.ac_timing.tMOD);

	MR.REG          = 0;
	MR.MR1.DLL      = 1;							/* 0: Enable, 1 : Disable	*/
	MR.MR1.AL	= 0;
	MR.MR1.ODS0	= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR.MR1.ODS1	= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR.MR1.RTT_Nom0	= 0;
	MR.MR1.RTT_Nom1	= 0;
	MR.MR1.RTT_Nom2	= 0;
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
	host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR1,
				MR.REG, g_nsih->dii.ac_timing.tMOD);

	/* Turn OFF DSCL - Whether PHY or controller triggered */
	reg_write_phy(DSCL_CNT, reg_value);
	reg_write_phy(AUTO_SCL_CTRL, reg_value);

	/* Set ref_burst_cnt to 1 to ensure quick entry into self refresh */
	mmio_set_32((DCTRL_BASE_ADDR + REF_CONFIG), (0x1 << 28));

	reg_value = ((1  << 22) |						/* prech_pwr_dn_only */
		     (2  << 12) |						/* self_ref_dly set to smallest allowed value to ensure | quick entry into self refresh */
		     (2  <<  4) |						/* pwr_dn_dly set to a small value 	*/
		     (1  <<  3) |						/* en_auto_self_ref	*/
		     (0  <<  2) |						/* en_auto_pwr_dn	*/
#if defined(DDR3)
		     (0  <<  1) |						/* en_correct_en	*/
		     (0	 <<  0));						/* ecc_en		*/
#elif defined(DDR4)
		     (1  <<  1) |						/* en_correct_en	*/
		     (1  <<  0));						/* ecc_en		*/
#endif
	reg_write_ctrl(PWR_SAVE_ECC_CONFIG, reg_value);

#else
	/* Self-Refresh Direct Command */
	enter_self_refresh();
#endif

	/* Check the Self-Refresh State (FSM) */
	do {
		c_ctrl_st = ((reg_read_ctrl(MEM_WIDTH) >> 12) & 0x7);
		ldelay(25);							/* 25us */
	} while (c_ctrl_st != DRAM_SELF_REFRESH);

	/*
	 * Wait enough time to ensure turn_off_addrctrl_drivers is asserted
	 * after entering self refresh
	 */
	unsigned int index = 10;
	index = ((reg_read_ctrl(DLY_CONFIG_2) & 0xFF) + index);
	ldelay(index * 10);

	/* 'receiver_en' bit in 'PHY_PAD_CTRL' set 0 */
	reg_clear_phy(PHY_PAD_CTRL, (1 << 28));
	reg_clear_phy(VREF_TRAINING, (1 << 4) | (1 << 2));
}

void enter_self_refresh(void)
{
	union DDR3_SDRAM_MR MR;
	int wait_cycle = g_nsih->dii.ac_timing.tCKESR;

	/* Step 01. Disable Controller State Machine. This also Stops Refresh Generation */
	reg_write_ctrl(MEM_START, 0x1);
	host_cmd_load(HOST_CMD_1_0, 0, CMD_IDLE_NOP, g_nsih->dii.ac_timing.tRFC);

	/* Step 02. Send PALL Command */
	host_cmd_load(HOST_CMD_1_1, 0, CMD_PRE_ALL, g_nsih->dii.ac_timing.tRP);

	/* Step 03. (DRAM) ODT OFF */
	MR.REG          = 0;
	MR.MR2.RTT_WR   = 0; 							// 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm)
	MR.MR2.SRT      = 0;							// self refresh normal range, if (ASR == 1) SRT = 0;
	MR.MR2.ASR      = 1;							// auto self refresh enable
	MR.MR2.CWL	= (g_nsih->dii.ac_timing.WL + 0);			// WL + AL
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR2,
					MR.REG, g_nsih->dii.ac_timing.tMOD);

	/* Step 04. (DRAM) Set the Drive Strength */
	MR.REG          = 0;
	MR.MR1.DLL      = 1;							// 0: Enable, 1 : Disable
	MR.MR1.AL	= 0;
	MR.MR1.ODS0	= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR.MR1.ODS1	= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
	MR.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
	MR.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
	host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR1,
				MR.REG, g_nsih->dii.ac_timing.tMOD);

	/* Step 05. Enter Self-Refresh Command */
	host_cmd_load(HOST_CMD_1_4, 0, CMD_SELF_REFE, g_nsih->dii.ac_timing.tCKESR);
	/*
	 * Step 06. Dummy command to ensure previous host cmd has finished
	 * executing before we continue
	 */
	host_cmd_load(HOST_CMD_1_5, 0, CMD_SELF_REF_CONT, 1);

	reg_write_ctrl(HOST_CMD_ISSUE, 0x15);
	while (reg_read_ctrl(HOST_CMD_ISSUE) & 0x10);
}

void exit_self_refresh(void)
{
	union DDR3_SDRAM_MR MR;

	int nCWL = (g_nsih->dii.ac_timing.WL + 0);
	int tDLLK = 512;							// 512 nCK
	int tXSDLL = tDLLK;

	int wait_cycle = g_nsih->dii.ac_timing.tXS;

//	reg_set_phy( PHY_PAD_CTRL), (1 << 28));

	/* Step 01. Bring DRAM out of self refresh and wait for DLL lock. tXSDLL = tDLLK */
	host_cmd_load(HOST_CMD_1_0, 0, CMD_SELF_REFX, tXSDLL);

	/* Step 02. Send PALL Command */
	host_cmd_load(HOST_CMD_1_1, 0, CMD_PRE_ALL, wait_cycle);

	/* Step 03. (DRAM) ODT On */
	MR.REG		= 0;
	MR.MR2.RTT_WR	= g_nsih->dsinfo.mr2_rtt_wr;				// Dynamic ODT (00: Rtt disabled, 01: RZQ/4, 10: RZQ/2, 11: reserved)
	MR.MR2.SRT      = 0;							// Self Refresh normal range, if (ASR == 1) SRT = 0;
	MR.MR2.ASR      = 0;							// Auto Self Refresh Enable
	MR.MR2.CWL	= (nCWL - 5);						// CAS Write Latency (000: 5CK(tCK>=2.5ns), 001: 6CK(2.5ns>tCK>=1.875ns, 010: 7CK(1.875ns>tCK>=1.25ns), 011: 8CK(1.5ns>tCK>=1.25ns), 100: 9CK(1.25ns>tCK>=1.07ns))
	host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR2, MR.REG, wait_cycle);

	/* Step 04. (DRAM) Set the Drive Strength */
	MR.REG          = 0;
	MR.MR1.DLL      = 0;							// 0: Enable, 1 : Disable
	MR.MR1.AL	= 0;
	MR.MR1.ODS0	= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
	MR.MR1.ODS1	= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
	MR.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
	MR.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
	MR.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
	host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR1, MR.REG, wait_cycle);

	reg_write_ctrl(HOST_CMD_ISSUE, 0x13);
	while (reg_read_ctrl(HOST_CMD_ISSUE) & 0x10);

	/* Step 05. Enable Controller State Machine. */
	reg_write_ctrl(MEM_START, 0x3);

#if 0
	int c_ctrl_st = 0;

	/* Step 06. Check the Normal State (FSM) */
	do {
		c_ctrl_st = ((reg_read_phy( MEM_WIDTH) >> 12) & 0x7);
	} while (c_ctrl_st != DRAM_NORMAL_OPERATION);
#endif
}

union DDR3_SDRAM_MR MR0, MR1, MR2, MR3;

void mem_init_seq_ddr3 (unsigned int is_resume)
{
	int mr0_cl, mr0_wr;

	int nCL  = (g_nsih->dii.ac_timing.RL + 0);				// RL + AL  tCK
	int nCWL = (g_nsih->dii.ac_timing.WL + 0);				// WL + AL  tCK
	int nWR  = (g_nsih->dii.ac_timing.tWR * 2);

	int wait_cycle;

	int reg_value;

	if (is_resume != TRUE) {
		reg_write_ctrl(MEM_START, 0x1);					// Kick off controller initialization state m/c

		/* Step 01. Power ON. Assert RESET# and ensure CKE is LOW at least max of 10ns or 5tCK */
		reg_clear_ctrl(MEM_WIDTH, (1 << 4));				// Bit 4 controls dfi_reset_n

		host_cmd_load(HOST_CMD_1_0, 0, CMD_CKE_LOW, 5);//max(8000/2500, 5));
		host_cmd_load(HOST_CMD_1_1, 0, CMD_CKE_LOW, 0);

		/* Issue 2 commands to the intialization state m/c and wait for it to be done */
		reg_write_ctrl(HOST_CMD_ISSUE, 0x11);
		while (reg_read_ctrl(HOST_CMD_ISSUE) & 0x10);
		/* Issue 2 commands to the intialization state m/c and wait for it to be done. */

	 	/*
	 	 * Step 02. DRAM RESET# pin must be LOW for at least 200us before DRAM is brought out of reset
	  	 * We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
	  	 * WARNING: 200 us is required before RST_N goes inactive.
	  	 * Insert software function to wait 200us here.
	  	 */
		udelay(200000);

		/* 03. CKE must be LOW 10ns prior to RESET# transitioning HIGH A`lready taken care of. So Deasserting RESET# */
		reg_set_ctrl(MEM_WIDTH, (1 << 4)|(1 << 1));			// dfi_reset_n

		/*
		 * Step 04. After RESET# is deasserted, wait 500us with CKE LOW
		 * We can't do this in simulation. Therefore ignore the following warning from the DRAM model:
		 * WARNING: 500 us is required after RST_N goes inactive before CKE goes active.
		 * Insert software function to wait 500us here..
		 */
		udelay(500000);

		/* Step 05. CKE may be brought HIGH and only NOP or DES commands may be issued for tXPR */
		int tXPR_MAX = (g_nsih->dii.ac_timing.tRFC + 10);
		host_cmd_load(HOST_CMD_1_0, 0, CMD_IDLE_NOP, tXPR_MAX);

		wait_cycle = (g_nsih->dii.ac_timing.tMRD);

		/* Step 06. Issue an MRS command to MR2. */
		MR2.REG			= 0;
		MR2.MR2.RTT_WR		= g_nsih->dsinfo.mr2_rtt_wr;		// Dynamic ODT (00: Rtt disabled, 01: RZQ/4, 10: RZQ/2, 11: reserved)
		MR2.MR2.CWL		= (nCWL - 5);				// CAS Write Latency (000: 5CK(tCK>=2.5ns), 001: 6CK(2.5ns>tCK>=1.875ns, 010: 7CK(1.875ns>tCK>=1.25ns), 011: 8CK(1.5ns>tCK>=1.25ns), 100: 9CK(1.25ns>tCK>=1.07ns))
		host_cmd_load_mr(HOST_CMD_1_1, SDRAM_MODE_REG_MR2, MR2.REG, wait_cycle);

		/* Step 07. Issue an MRS command to MR3. */
		MR3.REG			= 0;
		host_cmd_load_mr(HOST_CMD_1_2, SDRAM_MODE_REG_MR3, MR3.REG, wait_cycle);

		/* Step 08. Issue an MRS command to MR1. */
		MR1.REG			= 0;
		MR1.MR1.DLL		= 0;					// 0: Enable, 1: Disable
		MR1.MR1.AL		= 0;
		MR1.MR1.RTT_Nom0	= ((g_nsih->dsinfo.mr1_rtt_nom >> 0) & 0x1);
		MR1.MR1.RTT_Nom1	= ((g_nsih->dsinfo.mr1_rtt_nom >> 1) & 0x1);
		MR1.MR1.RTT_Nom2	= ((g_nsih->dsinfo.mr1_rtt_nom >> 2) & 0x1);
		MR1.MR1.ODS0		= ((g_nsih->dsinfo.mr1_ods >> 0) & 0x1);
		MR1.MR1.ODS1		= ((g_nsih->dsinfo.mr1_ods >> 1) & 0x1);
		MR1.MR1.QOff		= 0;
		MR1.MR1.WL		= 0;
		host_cmd_load_mr(HOST_CMD_1_3, SDRAM_MODE_REG_MR1, MR1.REG, wait_cycle);

		/* Step 09. Issue an MRS command to MR0. */
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
		MR0.MR0.PD		= 1;					// PD_EXIT
		MR0.MR0.WR		= mr0_wr;
		MR0.MR0.DLL		= 1;					// DLL_RESET
		MR0.MR0.CL0		= ((mr0_cl >> 0) & 0x1);
		MR0.MR0.CL1		= ((mr0_cl >> 1) & 0x7);
		host_cmd_load_mr(HOST_CMD_1_4, SDRAM_MODE_REG_MR0, MR0.REG, 0x6);//tMOD);

		/*
		 * Step 10. Issue a ZQCL command to calibrate Rtt and Ron values
		 * for the process voltage temperature
		 * (PVT). Prior to normal operation, tZQinit must be satisfied.
		 */
		wait_cycle = g_nsih->dii.ac_timing.tZQinit;
		host_cmd_load(HOST_CMD_1_5, 0x04, CMD_ZQ_CAL, wait_cycle);

		// Step 11. Issue 6 command to the intialization state m/c and wait for it to be done
		reg_write_ctrl(HOST_CMD_ISSUE, 0x15);
		while (reg_read_ctrl( HOST_CMD_ISSUE) & 0x10);

		reg_value = ((1 << 1) |						// start_mem_init
			     (1 << 0));						// mem_init_done
		reg_write_ctrl(MEM_START, reg_value);				// Set the Initialization complete bit.
	}

	/* Wait for Analog DLL to lock */
	reg_value = reg_read_phy( UNQ_ANALOG_DLL_2);		// 0x160
	while ((reg_value & 0x00000003) != 0x00000003)
		reg_value  = reg_read_phy( UNQ_ANALOG_DLL_2);

}

void t_corner_override(unsigned int override)
{
	unsigned int reg_value = 0;
	/* Program typical corner IO calibration settings in override mode */
	unsigned int cal_p_offset = 23, cal_n_offset = 7;
	unsigned int cal_rx_offset = 27;
	unsigned int bit_mask = 0x1F;

	if (override) {
		reg_value = reg_read_phy(UNIQUIFY_IO_1);
		reg_value &= ~((1 << 3) | (1 << 4) |
					(bit_mask <<  8) |
					(bit_mask << 16));
		reg_value |=  ((1 << 3) | (1 << 4) |
					((cal_p_offset & bit_mask) <<  8) |
					((cal_n_offset & bit_mask) << 16));
		reg_write_phy(UNIQUIFY_IO_1, reg_value);

		reg_value = reg_read_phy(UNIQUIFY_IO_2);
		reg_value &= ~((1 << 22) | (bit_mask << 16));
		reg_value |=  ((1 << 22) | (cal_rx_offset & bit_mask) << 16);
		reg_write_phy(UNIQUIFY_IO_2, reg_value);

		reg_value = reg_read_phy(UNIQUIFY_IO_3);
		reg_value &= ~((1 << 3)| (1 << 4) |
					 (bit_mask <<  8) |
					 (bit_mask << 16));
		reg_value |=  ((1 << 3)| (1 << 4) |
					 ((cal_p_offset & bit_mask) <<	8) |
					 ((cal_p_offset & bit_mask) << 16));
		reg_write_phy(UNIQUIFY_IO_3, reg_value);

		/* Pulse update bit to let override settings take effect */
		reg_set_phy(UNIQUIFY_IO_1, (1 << 2));
		reg_clear_phy(UNIQUIFY_IO_1, (1 << 2));
	} else {
		reg_value = reg_read_phy(UNIQUIFY_IO_1);
		reg_value &= ~((1 << 3) | (1 << 4) |
					(bit_mask <<  8) |
					(bit_mask << 16));
		reg_write_phy(UNIQUIFY_IO_1, reg_value);

		reg_value = reg_read_phy(UNIQUIFY_IO_2);
		reg_value &= ~((1 << 22) | (bit_mask << 16));
		reg_write_phy(UNIQUIFY_IO_2, reg_value);

		reg_value = reg_read_phy(UNIQUIFY_IO_3);
		reg_value &= ~((1 << 3)| (1 << 4) |
					 (bit_mask <<  8) |
					 (bit_mask << 16));
		reg_write_phy(UNIQUIFY_IO_3, reg_value);
		/* Start the automatic calibration */
		reg_write_phy(UNIQUIFY_IO_1, 0x1);

		/* Wait for done_seen to get set */
		reg_value = reg_read_phy(UNIQUIFY_IO_1);
		while (((reg_value >> 1) & 0x1) != 1)
			reg_value  = reg_read_phy(UNIQUIFY_IO_1);
		/* Wait for done to get cleared */
		reg_value = reg_read_phy(UNIQUIFY_IO_1);
		while (((reg_value >> 5) & 0x1) != 1)
			reg_value  = reg_read_phy(UNIQUIFY_IO_1);

		/* Pulse update bit to let override settings take effect */
		reg_set_phy(UNIQUIFY_IO_1, (1 << 2));
		reg_clear_phy(UNIQUIFY_IO_1, (1 << 2));
	}

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
 * Step 04. Set the Leveling & Calibration.
 *		-> Write-Leveling.
 *		-> Bit-Leveling.
 *************************************************************/
int  ddr3_initialize (unsigned int is_resume)
{
	int ret = 0;

	if (is_resume != TRUE) {
		/* At cold-boot, set 'PAD_RETEN_N' = 1 at this stage. */
		vddpwron_ddr_on();
	} else {
		/* @brief: Gloabal DPHY Reset Asserted -> Deasserted */
		dphy_global_reset();

		/* 'receiver_en' bit in 'PHY_PAD_CTRL' set 0 */
		reg_clear_phy(PHY_PAD_CTRL, (1 << 28));
		/* 'vref_output_enable' = 0, 'vref_value = 0' */
		reg_clear_phy(VREF_TRAINING, (0x7 << 4) | (1 << 2));
	}

	phy_set_init_values();

	if (is_resume == TRUE)
		t_corner_override(TRUE);

	ctrl_set_init_values();

	if (is_resume == TRUE) {
		/* 'start_mem_init' = 1, 'mem_init_done' = 1 */
		reg_write_ctrl(MEM_START, 0x3);
		/*
		 * @brief: For the controller to state that the state machine
		 * is in a self-refresh state.
		 */
//		self_refresh_entry();

		/* At resume, set 'PAD_RETEN_N' = 1 at this stage. */
		vddpwron_ddr_on();

		t_corner_override(FALSE);
	}

	mem_init_seq_ddr3(is_resume);

	return ret;
}
