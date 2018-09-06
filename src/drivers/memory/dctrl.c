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
#include <dctrl.h>

#if defined(DDR3)
#include <ddr3_ac_timing.h>
#elif defined(DDR4)
#include <ddr4_ac_timing.h>
#endif

// REF_CONFIG: Address 7¡¯hA
#define MAX_LATENCY			4

// DLY_CONFIG_4: Address 7'F
#define SWAP_PHASE			0

// ZQC_CONFIG_1 : Address 7'h62
#define ZQC_SHORT			0
#define ZQC_INTERVAL			2
#define AUTO_ZQC_ENABLE			0

// AUTO_SCL_CTRL: Address 7'h16
#define AUTO_SCL_INTERVAL		2
#define AUTO_SCL_ENABLE			0

// DATA_XFR_CONFIG : Address 7'h11
#define BIG_ENDIAN			0

// MEM_WIDTH : Address 7'h14
#define NATIVE_BURST8			0					// (1: only full-rate mode)
#define HALF_RATE_MODE			1					// (1: only full-rate mode or both)
#define TWO_T_TIMING   			1					//
#define EN_AUTO_PRECH			0					// enable the auto precharge

// REF_CONFIG : Address 7'hA
#define MAX_PEND_REF			8					// Maximum number of refreshes that can be posted or delayed. Allowed range is 0-
#define REF_BURST_CNT			1					// Number of refreshes that need to be burst together. Minimum value is 1. Maximum value = MAX_PEND_REF

// PWR_SAVE_ECC_CONFIG : Address 7'hB
#if defined(DDR3)
// REF_CONFIG: Address 7¡¯hA
#define ECC_DLY				0

#define ECC_EN				0
#define ERR_CORR_EN			0
#define EN_AUTO_PWR_DN			0
#define EN_AUTO_SELF_REF		0					// Set to 1 if wanting controller to automatically put memory into self-refresh
#define PWR_DN_DLY			2
#define SELF_REF_DLY			2					// Number of refresh BURST periods of idle time the controller needs to wait before automatic entry into self refresh mode
#define PRECH_PWR_DN_ONLY		0
#elif defined(DDR4)
// REF_CONFIG: Address 7¡¯hA
#define ECC_DLY				1

#define ECC_EN				1
#define ERR_CORR_EN			1
#define EN_AUTO_PWR_DN			1
#define EN_AUTO_SELF_REF		0					// Set to 1 if wanting controller to automatically put memory into self-refresh
#define PWR_DN_DLY			2
#define SELF_REF_DLY			2					// Number of refresh BURST periods of idle time the controller needs to wait before automatic entry into self refresh mode
#define PRECH_PWR_DN_ONLY		1
#endif

#define CLK_JITTER_RND_UP		1
#define MEM_CHIP_SELECTS		2

#if (USE_NSIH_HEADER == 0)

/* Address Mapping (ref. section 6.2) */
/* {CS, ROW, BA, COL}
 * {ROW, CS, BA, COL}
 * {CS, ROW, COL_HIGH, BA, COL_LOW}
 * {ROW, COL_HIGH, CS, BA, COL_LOW}
 */
#if defined(DDR3)
/* Address Mapping RSHIFT */
#define COL_BITS			CONFIG_DDR3_COLUMN_NUM
#define COL_BITS_LOW			CONFIG_DDR3_COLUMN_NUM// ((COL_BITS > 8) ? 8 : COL_BITS)
#define COL_BITS_HIGH			(COL_BITS - COL_BITS_LOW)
#define ROW_BITS			CONFIG_DDR3_ROW_NUM
#define BA_BITS				CONFIG_DDR3_BANK_NUM
#define BA_BITS_LOW			3 // 2					// why?
#define MEM_CS_BITS			CONFIG_DDR3_CS_NUM

// Case 1: {row_address, bank_addr_part1, column_address_part1, bank_addr_part2, column_address_part2}
#define CS_ADDR_RSHIFT			(ROW_BITS + COL_BITS) - 2 // BA_BITS_LOW

#define BANK_ADDR_RSHIFT		COL_BITS
#define BANK_ADDR_HIGH_RSHIFT		0 //COL_BITS

#define ROW_ADDR_RSHIFT			(COL_BITS + BA_BITS)
#define COL_ADDR_HIGH_RSHIFT		BA_BITS

/* Address Mapping Mask */
#define CS_ADDR_MASK			(((1 << MEM_CS_BITS)  - 1) << BA_BITS)
#define BG_ADDR_MASK			((1 << BG_BITS) - 1)
#define BANK_ADDR_MASK			((1 << (BA_BITS)) - 1)
#define BANK_ADDR_HIGH_MASK		(((1 << (BA_BITS - BA_BITS_LOW)) - 1) << BA_BITS_LOW)

#define ROW_ADDR_MASK			((1 << ROW_BITS) - 1)
#define COL_ADDR_HIGH_MASK		(((1 << COL_BITS_HIGH) - 1) << COL_BITS_LOW)
#define COL_ADDR_LOW_MASK		((1 << COL_BITS_LOW)  - 1)

#elif defined(DDR4)
/* Address Mapping RSHIFT */
#define COL_BITS			CONFIG_DDR4_COLUMN_NUM
#define COL_BITS_LOW			10
#define COL_BITS_HIGH			(CONFIG_DDR4_COLUMN_NUM - COL_BITS_LOW)
#define ROW_BITS			CONFIG_DDR4_ROW_NUM
#define BA_BITS				CONFIG_DDR4_BANK_NUM
#define BA_BITS_LOW			2					// why?
#define BG_BITS				CONFIG_DDR4_BG_NUM
#define MEM_CS_BITS			CONFIG_DDR4_CS_NUM

// Case 0: BA - RA - CA
#define CS_ADDR_RSHIFT			(ROW_BITS + COL_BITS + BA_BITS + BG_BITS - 3)
#define BG_ADDR_RSHIFT			3

#define BANK_ADDR_RSHIFT		(BA_BITS + BG_BITS + COL_BITS_LOW)
#define BANK_ADDR_HIGH_RSHIFT		0
//#define BANK_ADDR_LOW_RSHIFT		15

#define ROW_ADDR_RSHIFT			(COL_BITS + BG_BITS)
#define COL_ADDR_HIGH_RSHIFT		(BA_BITS + BG_BITS + COL_BITS_LOW - COL_BITS_LOW)

/* Address Mapping Mask */
#define CS_ADDR_MASK			(((1 << MEM_CS_BITS) - 1) << 3)
#define BG_ADDR_MASK			((1 << BG_BITS) - 1)
#define BANK_ADDR_MASK			((1 << (BA_BITS_LOW)) - 1)
#define BANK_ADDR_HIGH_MASK		(((1<<(BA_BITS - BA_BITS_LOW)) - 1) << BA_BITS_LOW)

#define ROW_ADDR_MASK			((1 << ROW_BITS) - 1)

#define COL_ADDR_HIGH_MASK     		(((1 << COL_BITS_HIGH) - 1) << COL_BITS_LOW)
#define COL_ADDR_LOW_MASK		((COL_BITS_LOW > BG_ADDR_RSHIFT) ?		\	// Check if BG is within COL_BITS_LOW range
                                       (((1 << (COL_BITS_LOW + BG_BITS)) - 1)		\
                                       	& (~(BG_ADDR_MASK << BG_ADDR_RSHIFT)))		\
                                       	: ((1 << COL_BITS_LOW) - 1))
#endif

#endif // #if (USE_NSIH_HEADER == 0)

void reg_write_ctrl(int addr, int reg_value)
{
#if defined(DDR_DBG_ON)
	printf("[ctrl-write] addr: 0x%08X, reg: 0x%08X \r\n", (DCTRL_BASE_ADDR + addr), reg_value);
#endif
	mmio_write_32((void*)(DCTRL_BASE_ADDR + addr), reg_value);
}

void reg_set_ctrl(int addr, int reg_value)
{
#if defined(DDR_DBG_ON)
	printf("[ctrl-set] addr: 0x%08X, reg: 0x%08X \r\n", (DCTRL_BASE_ADDR + addr), reg_value);
#endif
	mmio_set_32((void*)(DCTRL_BASE_ADDR + addr), reg_value);
}

int reg_read_ctrl(int addr)
{
	int reg_value;
	reg_value = mmio_read_32((void*)(DCTRL_BASE_ADDR + addr));
#if defined(DDR_DBG_ON)
	printf("[ctrl-read] addr: 0x%08X, reg: 0x%08X \r\n", (DCTRL_BASE_ADDR + addr), reg_value);
#endif
	return reg_value;
}

void reg_clear_ctrl(int addr, int reg_value)
{
	mmio_clear_32((void*)(DCTRL_BASE_ADDR + addr), reg_value);
#if defined(DDR_DBG_ON)
	printf("[ctrl-clear] addr: 0x%08X, reg: 0x%08X \r\n", (DCTRL_BASE_ADDR + addr),
		mmio_read_32(DCTRL_BASE_ADDR + addr));
#endif
}



// wait_cycle <-- wait_ps(tMOD)/ctrl_clk_period
void host_cmd_load_mr(unsigned int offset, unsigned char mr_num,
					unsigned short mr_value, unsigned int wait_cycle)
{
	union host_cmd hcmd1, hcmd2;

	hcmd1.reg 		= 0;
	hcmd1.cmd1.op_code1	= ((mr_value >> 0) & 0xFF);
	hcmd1.cmd1.cmd		= CMD_LOAD_MODE & 0xFFFFFF;

	hcmd2.reg 		= 0;
	hcmd2.cmd2.wait_cycle	= ((max(wait_cycle, 1) - 1) & 0x1FF);
	hcmd2.cmd2.mr		= (mr_num >> 0) & 0x3;
	hcmd2.cmd2.op_code2	= ((mr_value >> 8) & 0xFF);

#if defined(DDR_DBG_ON)
	printf("1. Addr: 0x%08X hcmd1: 0x%08X \r\n", DCTRL_BASE_ADDR + (offset + 0x00), hcmd1.reg);
	printf("2. Addr: 0x%08X hcmd2: 0x%08X \r\n", DCTRL_BASE_ADDR + (offset + 0x40), hcmd2.reg);
#endif
	mmio_write_32(DCTRL_BASE_ADDR + (offset + 0x00), hcmd1.reg);
	mmio_write_32(DCTRL_BASE_ADDR + (offset + 0x40), hcmd2.reg);
}

void host_cmd_load(unsigned int offset, unsigned int cmd_high, unsigned int cmd_low,
				int wait_cycle)
{
	int reg_value;

	wait_cycle = (max(wait_cycle, 1) - 1);
	reg_value = ((wait_cycle << WAIT_SHIFT) | cmd_high);
#if defined(DDR_DBG_ON)
	printf("1. Addr: 0x%08X, cmd_low  : 0x%08X \r\n", DCTRL_BASE_ADDR + (offset + 0x00), cmd_low);
	printf("2. Addr: 0x%08X, reg_value: 0x%08X \r\n", DCTRL_BASE_ADDR + (offset + 0x40), reg_value);
#endif
	mmio_write_32(DCTRL_BASE_ADDR + (offset + 0x00), cmd_low);
	mmio_write_32(DCTRL_BASE_ADDR + (offset + 0x40), reg_value);
}

static int ctrl_state_check(void)
{
	return ((reg_read_ctrl(MEM_WIDTH) >> 12) & 0x7);
}

#if (USE_NSIH_HEADER == 1)
static void ctrl_set_addr_map (void)
{
#if defined(DDR3)
	/* Address Mapping RSHIFT */
	int col_bits = g_nsih->dii.col_num;
	int col_bits_low = g_nsih->dii.col_num;// ((col_bits > 8) ? 8 : col_bits)
	int col_bits_high = (col_bits - col_bits_low);
	int row_bits = 	g_nsih->dii.row_num;
	int ba_bits = g_nsih->dii.bank_num;
	int ba_bits_low = 3; // 2					// why?
	int mem_cs_bits	= g_nsih->dii.chip_num;

	// Case 1: {row_address, bank_addr_part1, column_address_part1, bank_addr_part2, column_address_part2}
//	int cs_addr_rshift = (row_bits + col_bits) - 2;				// ba_bits_low
	int cs_addr_rshift = (row_bits + col_bits);				// ba_bits_low

	int bank_addr_rshift = col_bits;
	int bank_addr_high_rshift = 0;						//col_bits

	int row_addr_rshift = (col_bits + ba_bits);
	int col_addr_high_rshift = ba_bits;

	/* address mapping mask */
	int cs_addr_mask = (((1 << mem_cs_bits)  - 1) << ba_bits);
	int bank_addr_mask = ((1 << (ba_bits)) - 1);
	int bank_addr_high_mask = (((1 << (ba_bits - ba_bits_low)) - 1) << ba_bits_low);

	int row_addr_mask = ((1 << row_bits) - 1);
	int col_addr_high_mask = (((1 << col_bits_high) - 1) << col_bits_low);
	int col_addr_low_mask =  ((1 << col_bits_low)  - 1);
#elif defined(DDR4)
	/* Address Mapping RSHIFT */
	int col_bits = g_nsih->dii.col_num;
	int col_bits_low = g_nsih->dii.col_num;// ((col_bits > 8) ? 8 : col_bits)
	int col_bits_high = (col_bits - col_bits_low);
	int row_bits = 	g_nsih->dii.row_num;
	int ba_bits = g_nsih->dii.bank_num;
	int ba_bits_low = 3; // 2					// why?
	int bg_bits = g_nsih->dii.bg_num;
	int mem_cs_bits	= g_nsih->dii.chip_num;

	// case 0: ba - ra - ca
	int cs_addr_rshift = (row_bits + col_bits + ba_bits + bg_bits - 3);
	int bg_addr_rshift = 3;

	int bank_addr_rshift = (ba_bits + bg_bits + col_bits_low);
	int bank_addr_high_rshift = 0;
//	int bank_addr_low_rshift = 15

	int row_addr_rshift = (col_bits + bg_bits);
	int col_addr_high_rshift = (ba_bits + bg_bits + col_bits_low - col_bits_low);

	/* address mapping mask */
	int cs_addr_mask = (((1 << mem_cs_bits) - 1) << 3);
	int bg_addr_mask = ((1 << bg_bits) - 1);
	int bank_addr_mask = ((1 << (ba_bits_low)) - 1);
	int bank_addr_high_mask = (((1<<(ba_bits - ba_bits_low)) - 1) << ba_bits_low);

	int row_addr_mask = ((1 << row_bits) - 1);
	int col_addr_high_mask = (((1 << col_bits_high) - 1) << col_bits_low);
	int col_addr_low_mask = ((col_bits_low > bg_addr_rshift) ?	// check if bg is within col_bits_low range
                                       (((1 << (col_bits_low + bg_bits)) - 1)
                                       	& (~(bg_addr_mask << bg_addr_rshift)))
                                       	: ((1 << col_bits_low) - 1));
#endif
	int reg_value;

	reg_value = ((row_addr_rshift	   <<  0) |				//row_addr_rshift
		     (bank_addr_rshift	   <<  8) |				//bank_addr_rshift
		     (cs_addr_rshift	   << 16) |				//cs_addr_rshift
		     (col_addr_high_rshift << 24) |				//col_addr_high_rshift
		     (0x0 << 29) |						//ahb_high_priority
		     (EN_AUTO_PRECH << 30) |					//en_auto_prech
		     (0x0 << 31));						//fast_dfi_phyupd_ack

	reg_write_ctrl(MEM_CONFIG_1, reg_value);
	reg_write_ctrl(MEM_CONFIG_2, row_addr_mask);				// row_addr_mask

	// aprech_bit_pos
	reg_write_ctrl(MEM_CONFIG_3,  (0xa << 16) | (col_addr_high_mask << 0x0));
	reg_write_ctrl(MEM_CONFIG_4, col_addr_low_mask);
	reg_write_ctrl(MEM_CONFIG_5, (bank_addr_high_rshift  << 24) |		// bank_addr_high_rshift
				     (bank_addr_high_mask    << 16) |		// bank_addr_high_mask
				     (cs_addr_mask	     <<  8) |		// cs_addr_mask
				     (bank_addr_mask	     <<  0));		// bank_addr_mask
#if defined(DDR4)
	reg_value = ((bg_addr_rshift <<  0) |					//bank_group_shift
		     (bg_addr_mask   <<  8));					//bank_group_mask
	reg_write_ctrl(MEM_CONFIG_7, reg_value);
#endif

}
#endif // #if (USE_NSIH_HEADER == 0)

void ctrl_set_init_values(void)
{
#if defined(DDR3)
	struct ddr3_timing_task * ac_timing = &g_nsih->dii.ac_timing;
#elif defined(DDR4)
	struct ddr4_timing_task * ac_timing = &g_nsih->dii.ac_timing;
#endif

	int wr2prech, wr2rd;
	int wr2rd_csc;

	int RL = ac_timing->RL, WL = ac_timing->WL;
	int rl_d = RL, wl_d = WL;
	int RL_PARAM = round_up((double)rl_d / (double)2);
	int WL_PARAM = round_up(((double)wl_d + (double)1) / (double)2);
	int BL = NATIVE_BURST8 ? 4 : 2;
	int C2D = 1;

	int slice;
	volatile unsigned int reg_value;

	// Step XX. Set the MEM_WIDTH
	reg_value = ((0x0 <<  0) |						//quarter_wide_mode
		     (0x0 <<  1) |						//half_wide_mode
		     (NATIVE_BURST8	<<  2) |				//native_burst8
		     (HALF_RATE_MODE	<<  3) |				//half_rate
		     (0x1 <<  4) |						// dfi_reset_n
		     (TWO_T_TIMING	<<  6) |				//onecmd_perclk_halfrate
#if defined(LPDDR2) || defined(LPDDR3)
		     (0x1  <<  7) |						//lpddr2 or lpddr3
#else
		     (0x0  <<  7) |
#endif

#if defined(DDR4)
		     (0x1 <<  8));						//ddr4 (only ddr4)
#else
		     (0x0 <<  8));						//ddr4
#endif
	reg_write_ctrl(MEM_WIDTH, reg_value);

	/* Step XX. Set the ARB_AGENT_SEL */
	// Arbiter settings. Each of the following settings can be programmed independently for each agent
	/* These values are programmed at reset. So commenting out this block to save initialization time */
	for (slice = 0; slice < 2; slice++) {
#if 0
		reg_write_ctrl(ARB_AGENT_SEL, slice * MIN_LIMIT_WIDTH, slv_ctrl);
		reg_write_ctrl(MIN_LIMIT, 0xFF, slv_ctrl);			//Min number of continuous DDR bursts allowed for agent i (each burst takes 2 cycles)
		reg_write_ctrl(ARB_AGENT_SEL, slice * RQST_PERIOD_WIDTH, slv_ctrl);
		reg_write_ctrl(RQST_PERIOD, RQST_PERIOD_WIDTH 0xFF, slv_ctrl); // Bandwidth calculation window for agent i. 32 => 128 clocks
#endif
		reg_write_ctrl(ARB_AGENT_SEL, (slice * 8));
		reg_write_ctrl(MIN_CMD_ACPT,  0x4);				// Minimum number of DDR bursts that must be granted to agent
	}

	// Step XX. Set the MEM_CONFIG_1
#if (USE_NSIH_HEADER == 0)
	reg_value = ((ROW_ADDR_RSHIFT      <<  0) |				//row_addr_rshift
		     (BANK_ADDR_RSHIFT     <<  8) |				//bank_addr_rshift
		     (CS_ADDR_RSHIFT       << 16) |				//cs_addr_rshift
		     (COL_ADDR_HIGH_RSHIFT << 24) |				//col_addr_high_rshift
		     (0x0 << 29) |						//ahb_high_priority
		     (EN_AUTO_PRECH << 30) |					//en_auto_prech
		     (0x0 << 31));						//fast_dfi_phyupd_ack
	reg_write_ctrl(MEM_CONFIG_1, reg_value);
	reg_write_ctrl(MEM_CONFIG_2, ROW_ADDR_MASK);				// row_addr_mask

	// aprech_bit_pos
	reg_write_ctrl(MEM_CONFIG_3,  (0xA << 16) | (COL_ADDR_HIGH_MASK << 0x0));
	reg_write_ctrl(MEM_CONFIG_4, COL_ADDR_LOW_MASK);
	reg_write_ctrl(MEM_CONFIG_5, (BANK_ADDR_HIGH_RSHIFT  << 24) |		// bank_addr_high_rshift
				     (BANK_ADDR_HIGH_MASK    << 16) |		// bank_addr_high_mask
				     (CS_ADDR_MASK           <<  8) |		// cs_addr_mask
				     (BANK_ADDR_MASK         <<  0));		// bank_addr_mask

#if defined(DDR4)
	reg_value = ((BG_ADDR_RSHIFT <<  0) |					//bank_group_shift
		     (BG_ADDR_MASK   <<  8));					//bank_group_mask
	reg_write_ctrl(MEM_CONFIG_7, reg_value);
#endif

#else
	ctrl_set_addr_map();
#endif

// REF_CONFIG: Address 7¡¯hA
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = (((tREFI - 2)   <<  0) |					//ref_exp_cnt
		     ((tRFC  - 2)   << 16) |					//ref_dly
		     (MAX_PEND_REF  << 24) |					//max_pending_refs
		     (REF_BURST_CNT << 28));					//ref_burst_cnt
#elif defined(DDR4)
	reg_value = (((tREFI - 2)   <<	0) |					//ref_exp_cnt
		     ((tRFC1  - 2)   << 16) |					//ref_dly
		     (MAX_PEND_REF  << 24) |					//max_pending_refs
		     (REF_BURST_CNT << 28));					//ref_burst_cnt
#elif defined(LPDDR3) || defined(LPDDR4)
	reg_value = (((tREFI - 2)   <<  0) |					//ref_exp_cnt
		     ((tRFC  - 2)   << 16) |					//ref_dly
		     (MAX_PEND_REF  << 24) |					//max_pending_refs
		     (REF_BURST_CNT << 28));					//ref_burst_cnt
#endif

#else	// #if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = (((ac_timing->tREFI - 2)   <<  0) |				//ref_exp_cnt
		     ((ac_timing->tRFC  - 2)   << 16) |				//ref_dly
		     (MAX_PEND_REF  << 24) |					//max_pending_refs
		     (REF_BURST_CNT << 28));					//ref_burst_cnt
#endif
#endif
	reg_write_ctrl(REF_CONFIG, reg_value);

	//PWR_SAVE_ECC_CONFIG
	reg_value = ((ECC_EN            <<  0) |				//ecc_en
		     (ERR_CORR_EN       <<  1) |				//err_correct_en
		     (EN_AUTO_PWR_DN    <<  2) |				//en_auto_pwr_dn
		     (EN_AUTO_SELF_REF  <<  3) |				//en_auto_self_ref
		     (PWR_DN_DLY        <<  4) |				//pwr_dn_dly
		     (SELF_REF_DLY      << 12) |				//self_ref_dly
		     (PRECH_PWR_DN_ONLY << 22));				//prech_pwr_dn_only
	reg_write_ctrl(PWR_SAVE_ECC_CONFIG, reg_value);

	//Values programmed across more than 1 register
#if (USE_NSIH_HEADER == 0)

#if defined(DDR3)
	wr2prech  = round_up((wl_d + tWR + 4)/2) ;
	wr2rd     = round_up((wl_d + tWTR + 4)/2) + tWTR;
	wr2rd_csc = max(((WL_PARAM + BL) + tWTR - 1), 3);
#elif defined(DDR4)
	wr2prech  = round_up((wl_d + 4)/2) + tWR + CLK_JITTER_RND_UP;
	wr2rd     = round_up((wl_d + 4)/2) + tWTR_S + 1;			//+1 needed for corner case
	wr2rd_csc = wr2rd;
#elif defined(LPDDR2) // after timing calcurate ///
	wr2prech  = round_up((wl_d + 4)/2) + tWR;
	wr2rd     = round_up((wl_d + 4)/2) + tWTR_L;
	wr2rd_csc = wr2rd;
#elif defined(LPDDR3) // after timing calcurate ///
	wr2prech  = tWR + (WL_PARAM + max(BL, 3));
	//Add an extra clock to wr2prech when BL==2
	wr2rd     = round_up((wl_d + 1)/2) + tWTR;
	wr2rd_csc = max(wr2rd - 4, 4);
#endif
#else	// #if (USE_NSIH_HEADER == 0)

	wr2prech  = ac_timing->WR2PRECH;
	wr2rd     = ac_timing->WR2RD;
	wr2rd_csc = ac_timing->WR2RD_CSC;
#endif

	//DLY_CONFIG_1
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = (((wr2rd & 0xF)			<<  0)		|	// wr2rd_dly[3:0]
		     ((wr2rd_csc & 0xF)			<<  4)		|	// wr2rd_csc_dly[3:0]
		     ((BL - 1)				<<  8)		|	// rd2rd_dly
		     ((BL + 1)				<< 12)		|	// rd2rd_csc_dly
		     ((BL - 1)				<< 16)		|	// wr2wr_dly
		     (BL				<< 20)		|	// wr2wr_csc_dly
		     (round_up(rl_d + tCCD - 2 - WL)	<< 24)		|	// rd2wr_dly
		     ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
		     		+ 4 + (ECC_DLY | 0x1)) << 28));			// rmw_dly
#elif defined(DDR4) // after ac_timing calcurate ///
	reg_value = ((((wr2rd >> 0) & 0xF) 		<<  0)		|	// wr2rd_dly[3:0]
		     (((wr2rd_csc >> 0) & 0xF)		<<  4)		|	// wr2rd_csc_dly[3:0]
		     ((round_up(tCCD_S) - 1)		<<  8)		|	// rd2rd_dly
		     ((round_up(tCCD_S) - 1)		<< 12)		|	// rd2rd_csc_dly
		     ((round_up(tCCD_S) - 1)		<< 16)		|	// wr2wr_dly
		     ((round_up(tCCD_S) - 1)		<< 20)		|	// wr2wr_csc_dly
		     ((round_up((rl_d + 4 - wl_d - 1)/2) - 1) << 24)	|	// rd2wr_dly
		     (((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
		     		+ 4 + (ECC_DLY | 0x1))) << 28));		// rmw_dly

#elif defined(LPDDR2) // after timing calcurate ///
	reg_value = ((((wr2rd >> 0) & 0xF)		<<  0)		|	// wr2rd_dly[3:0]
		     (((wr2rd_csc >> 0) & 0xF)		<<  4)		|	// wr2rd_csc_dly[3:0]
		     ((BL - 1)				<<  8)		|	// rd2rd_dly
		     ((BL + 2)				<< 12)		|	// rd2rd_csc_dly
		     ((BL - 1)				<< 16)		|	// wr2wr_dly
		     ((BL + 1)				<< 20)		|	// wr2wr_csc_dly
		     (round_up(rl + tDQSCK_dclk + 4 + 1 - wl) << 24) 	|	// rd2wr_dly
		     ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
				   + 4 + (ECC_DLY | 0x0)) << 28));		// rmw_dly
#elif defined(LPDDR3) // after timing calcurate ///
	reg_value = ((((wr2rd >> 0) & 0xF)		<<  0)		|	// wr2rd_dly[3:0]
		     (((wr2rd_csc >> 0) & 0xF)		<<  4)		|	// wr2rd_csc_dly[3:0]
		     ((BL - 1)				<<  8)		|	// rd2rd_dly
		     ((BL + 2)				<< 12)		|	// rd2rd_csc_dly
		     ((BL - 1)				<< 16)		|	// wr2wr_dly
		     ((BL + 1)				<< 20)		|	// wr2wr_csc_dly
		     ((BL + 1 + tDQSCKmax + round_down(RL-WL)) << 24) 	|	//rd2wr_dly
		     ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
				+ 4 + (ECC_DLY | 0x0)) << 28));			// rmw_dly
#endif

#else	// #if (USE_NSIH_HEADER == 0)

#if defined(DDR3)
	reg_value = (((wr2rd & 0xF)		<<  0)			|	// wr2rd_dly[3:0]
		     ((wr2rd_csc & 0xF)		<<  4)			|	// wr2rd_csc_dly[3:0]
		     ((BL - 1)			<<  8)			|	// rd2rd_dly
		     ((BL + 1)			<< 12)			|	// rd2rd_csc_dly
		     ((BL - 1)			<< 16)			|	// wr2wr_dly
		     (BL			<< 20)			|	// wr2wr_csc_dly
		     (round_up((double)(rl_d + ac_timing->tCCD_dclk
		     			- 2 - WL)/2) << 24)		|	// rd2wr_dly
		     ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
		     		+ 4 + (ECC_DLY | 0x1)) << 28));			// rmw_dly
//		     (0xC << 28));
#elif defined(DDR4)
	reg_value = ((((wr2rd >> 0) & 0xF)	<<  0)			|	// wr2rd_dly[3:0]
		     (((wr2rd_csc >> 0) & 0xF)	<<  4)			|	// wr2rd_csc_dly[3:0]
		     ((ac_timing->tCCD_S - 1)	<<  8)			|	// rd2rd_dly
		     ((ac_timing->tCCD_S - 1)	<< 12)			|	// rd2rd_csc_dly
		     ((ac_timing->tCCD_S - 1)	<< 16)			|	// wr2wr_dly
		     ((ac_timing->tCCD_S - 1)	<< 20)			|	// wr2wr_csc_dly
		     ((round_up((rl_d + 4 - wl_d - 1)/2) - 1) << 24)	|	// rd2wr_dly
		     ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM
				+ 4 + (ECC_DLY | 0x1)) << 28));			// rmw_dly
#endif

#endif
	reg_write_ctrl(DLY_CONFIG_1, reg_value);

#if (USE_NSIH_HEADER == 0)

#if defined(DDR4)
	//csc delays are set to non-csc versions
	reg_value = ((((round_up(WL + 4 / 2)
				+ max(tWTR_L, 4)) + CLK_JITTER_RND_UP) <<  0) |//wr2rd_dly_l
		      (((round_up(WL + 4 / 2)
		      		+ max(tWTR_L, 4)) + CLK_JITTER_RND_UP) <<  4) |//wr2rd_csc_dly_l
		      ((round_up(tCCD_L) - 1)	<<  8) |			//rd2rd_dly_l
		      ((round_up(tCCD_L) - 1)	<< 16) |			//rd2rd_csc_dly_l
		      ((round_up(tCCD_L) - 1)	<< 20) |			//wr2wr_dly_l
		      ((round_up(tCCD_L) - 1)	<< 24));			//wr2wr_csc_dly_l

	reg_write_ctrl(DDR4_DLY_CONFIG_1, reg_value);
#endif

#else	// #if (USE_NSIH_HEADER == 0)

#if defined(DDR4)
	//csc delays are set to non-csc versions
	reg_value = ((((round_up(WL + 4 / 2)
				+ max(ac_timing->tWTR_L, 4)) + CLK_JITTER_RND_UP) <<  0) |//wr2rd_dly_l
		      (((round_up(WL + 4 / 2)
		      		+ max(ac_timing->tWTR_L, 4)) + CLK_JITTER_RND_UP) <<  4) |//wr2rd_csc_dly_l
		      ((ac_timing->tCCD_L - 1)	<<  8) |			//rd2rd_dly_l
		      ((ac_timing->tCCD_L - 1)	<< 16) |			//rd2rd_csc_dly_l
		      ((ac_timing->tCCD_L - 1)	<< 20) |			//wr2wr_dly_l
		      ((ac_timing->tCCD_L - 1)	<< 24));			//wr2wr_csc_dly_l
	reg_write_ctrl(DDR4_DLY_CONFIG_1, reg_value);
#endif

#endif

	// DLY_CONFIG_2
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	int sref_exit;
	sref_exit = tZQoper;							//tZQoper is in ns for DDR3
	reg_value = (((tCKESR - 1)		<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((tCKE - 1)		<< 16) |			//pwr_dn_min_dly
		     ((tXP  - 1)		<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1)	<< 28) |			//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1)	<< 30));			//self_ref_exit_dly[8]
#elif defined(DDR4) // after ac_timing calcurate ///
	int sref_exit;
	sref_exit = tZQoper;							//tZQoper is in ns for DDR4
	reg_value = (((tCKESR - 1)		<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((tCKE - 1)		<< 16) |			//pwr_dn_min_dly
		     ((tXP  - 1)		<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1) << 28) |				//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1) << 30));				//self_ref_exit_dly[8]
#elif defined(LPDDR2) // after ac_timing calcurate ///
	int sref_exit;
	sref_exit = tXSR;
	reg_value = (((tCKESR - 1)		<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((tCKE - 1)		<< 16) |			//pwr_dn_min_dly
		     ((tXP  - 1)		<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1)	<< 28) |			//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1)	<< 30));			//self_ref_exit_dly[8]
#elif defined(LPDDR3) // after ac_timing calcurate ///
	int sref_exit;
	sref_exit = tXSR;
	reg_value = (((tCKESR - 1)		<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((tCKE - 1)		<< 16) |			//pwr_dn_min_dly
		     ((tXP  - 1)		<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1)	<< 28) |			//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1)	<< 30));			//self_ref_exit_dly[8]
#endif
#else	//  #if (USE_NSIH_HEADER == 0)

#if defined(DDR3)
	int sref_exit;
	sref_exit = ((512/2) - 1);						//tZQoper is in ns for DDR3
	reg_value = (((ac_timing->tCKESR - 1)	<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((ac_timing->tCKE - 1)	<< 16) |			//pwr_dn_min_dly
		     ((ac_timing->tXP  - 1)	<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1)	<< 28) |			//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1)	<< 30));			//self_ref_exit_dly[8]
#elif defined(DDR4)
	int sref_exit;
	sref_exit = ac_timing->tZQoper;						//tZQoper is in ns for DDR4
	reg_value = (((ac_timing->tCKESR - 1)	<<  0) |			//self_ref_min_dly
		     ((sref_exit & 0xFF)	<<  8) |			//self_ref_exit_dly[7:0]
		     ((ac_timing->tCKE - 1)	<< 16) |			//pwr_dn_min_dly
		     ((ac_timing->tXP  - 1)	<< 20) |			//pwr_dn_exit_dly
		     (((wr2prech >> 4) & 0x1)	<< 26) |			//wr2prech_dly[4]
		     (((wr2rd >> 4) & 0x1)	<< 27) |			//wr2rd_dly[4]
		     (((wr2rd_csc >> 4) & 0x1)  << 28) |			//wr2rd_csc_dly[4]
		     (((sref_exit >> 8) & 0x1)  << 30));			//self_ref_exit_dly[8]
#endif

#endif

	reg_write_ctrl(DLY_CONFIG_2, reg_value);

#if defined(DDR4)
		reg_value = (((round_up((rl_d + 4 - wl_d + 2)/2) - 1) << 0) |	//rd2wr_dly_l
			      ((RL_PARAM + C2D + MAX_LATENCY - WL_PARAM + 4
			      		+ (ECC_DLY | 0x1)) << 4));		//rmw_dly_l
		reg_write_ctrl(DDR4_DLY_CONFIG_2, reg_value);
#endif

	//DLY_CONFIG_3
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = ((tRP 		<<  0) |				// prech_all_dly
		     ((tRTP - 1)	<<  8) |				// rd2prech_dly
		     ((wr2prech & 0xF)	<< 12) |				// wr2prech_dly[3:0]
		     ((tRRD - 1)	<< 16) |				// ras2ras_dly
//		     (((((tRCD % 2) == 0) && !SWAP_PHASE)
//		     		? tRCD : (tRCD- 1)) << 20) |			// ras2cas_dly
		     (5			<< 20) |
		     ((tRP - 1)		<< 24) |				// prech2ras_dly
		     ((RL_PARAM + 3 + (NATIVE_BURST8 ? 2 : 0)) << 28));	// rd_burst_end_dly
#elif defined(DDR4) // after ac_timing calcurate ///
	reg_value = (((tRP - 1)		<<  0) |				//prech_all_dly
		     ((tRTP - 1)	<<  8) |				//rd2prech_dly
		     ((wr2prech & 0xF)	<< 12) |				// wr2prech_dly[3:0]
		     ((tRRD_S - 1)	<< 16) |				//ras2ras_dly
		     ((tRCD - 1)	<< 20) |				//ras2cas_dly
		     ((tRP - 1)		<< 24) |				//prech2ras_dly
		     ((((rl + 5)/2) - 1) << 28));				//rd_burst_end_dly
#elif defined(LPDDR2) // after ac_timing calcurate ///
	reg_value = (((tRPab - 1)	<<  0) |				//prech_all_dly
		     ((tRTP - 1)	<<  8) |				//rd2prech_dly
		     ((wr2prech & 0xF)	<< 12) |				//wr2prech_dly[3:0]
		     ((tRRD - 1)	<< 16) |				//ras2ras_dly
		     (((((tRCD % 2) == 0) && !SWAP_PHASE)
		     		? tRCD : (tRCD - 1)) << 20) |			//ras2cas_dly
		     ((tRP - 1)		<< 24) |				//prech2ras_dly
		     ((RL_PARAM + 3 + (NATIVE_BURST8 ? 2 : 0)
		     		+ tDQSCKmax) << 28));				//rd_burst_end_dly

#elif defined(LPDDR3) // after ac_timing calcurate ///
	reg_value = (((tRPab - 1)	<<  0) |				//prech_all_dly
		     ((tRTP - 1)	<<  8) |				//rd2prech_dly
		     ((wr2prech & 0xF)	<< 12) |				//wr2prech_dly[3:0]
		     ((tRRD - 1)	<< 16) |				//ras2ras_dly
		     (((((tRCD % 2) == 0) && !SWAP_PHASE)
		     		? tRCD : (tRCD - 1)) << 20) |			//ras2cas_dly
		     ((tRP - 1)		<< 24) |				//prech2ras_dly
		     ((RL_PARAM + 3 + (NATIVE_BURST8 ? 2 : 0)
		     		+ tDQSCKmax) << 28));				//rd_burst_end_dly
#endif
#else	// #if (USE_NSIH_HEADER == 0)

#if defined(DDR3)
	reg_value = ((ac_timing->tRP		<<  0) |			// prech_all_dly
		     ((ac_timing->tRTP - 1)	<<  8) |			// rd2prech_dly
		     ((wr2prech & 0xF)		<< 12) |			// wr2prech_dly[3:0]
		     (ac_timing->tRRD		<< 16) |			// ras2ras_dly
		     (((((ac_timing->tRCD_dclk % 2) == 0) && !SWAP_PHASE)
		     		? ac_timing->tRCD : (ac_timing->tRCD- 1)) << 20) |	// ras2cas_dly
		     ((ac_timing->tRP - 1)	<< 24) |			// prech2ras_dly
		     ((RL_PARAM + 3 + (NATIVE_BURST8 ? 2 : 0)) << 28));	// rd_burst_end_dly
#elif defined(DDR4)
	reg_value = (((ac_timing->tRP - 1)	<<  0) |			//prech_all_dly
		     ((ac_timing->tRTP - 1)	<<  8) |			//rd2prech_dly
		     ((wr2prech & 0xF)		<< 12) |			// wr2prech_dly[3:0]
		     ((ac_timing->tRRD_S - 1)	<< 16) |			//ras2ras_dly
		     ((ac_timing->tRCD - 1)	<< 20) |			//ras2cas_dly
		     ((ac_timing->tRP - 1)	<< 24) |			//prech2ras_dly
		     ((((RL + 5)/2) - 1)	<< 28));			//rd_burst_end_dly
#endif

#endif
	reg_write_ctrl(DLY_CONFIG_3, reg_value);

#if defined(DDR4)
	reg_value &= ~(0xF << 16);
	reg_value |= (round_up(max(ac_timing->tRRD_L_dclk, 4)/2) - 1);		//ras2ras_dly_l
	reg_write_ctrl(DDR4_DLY_CONFIG_3, reg_value);
#endif

	//DLY_CONFIG_4
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3)
	reg_value = (((tRAS  - 1)   <<   0) |					//ras2prech_dly
		     (SWAP_PHASE     <<  5) |					//swap_phase
		     ((tRC  - 1)     <<  8) |					//ras2ras_same_bank_dly
		     ((tFAW - 1)     << 16) |					//tfaw_dly
		     ((ZQC_SHORT ? (tZQCS - 1) : (tZQoper- 1)) << 24));	//zqc_dly
#elif defined(DDR4) // after ac_timing calcurate ///
	reg_value = (((tRAS -1)  <<  0) |					//ras2prech_dly
		     (SWAP_PHASE <<  5) |					//swap_phase
		     ((tRC - 1)  <<  8) |					//ras2ras_same_bank_dly
		     ((tFAW - 1) << 16) |					//tfaw_dly
		     ((ZQC_SHORT ? (tZQCS -1) : (tZQoper -1)) << 24));		//zqc_dly
#elif defined(LPDDR2) // after ac_timing calcurate ///
	reg_value = (((tRAS -1)  <<  0) |					//ras2prech_dly
		     (SWAP_PHASE <<  5) |					//swap_phase
		     ((tRC - 1)  <<  8) |					//ras2ras_same_bank_dly
		     ((tFAW - 1) << 16) |					//tfaw_dly
		     ((ZQC_SHORT ? (tZQCS -1) : (tZQCL -1)) << 24));		//zqc_dly
#elif defined(LPDDR3) // after ac_timing calcurate ///
	reg_value = (((tRAS -1)  <<  0) |					//ras2prech_dly
		     (SWAP_PHASE <<  5) |					//swap_phase
		     ((tRC - 1)  <<  8) |					//ras2ras_same_bank_dly
		     ((tFAW - 1) << 16) |					//tfaw_dly
		     ((ZQC_SHORT ? (tZQCS -1) : (tZQCL -1)) << 24));		//zqc_dly
#endif

#else	// // #if (USE_NSIH_HEADER == 0)

#if defined(DDR3) || defined(DDR4)
	reg_value = (((ac_timing->tRAS_min - 1) <<  0) |			//ras2prech_dly
		     (SWAP_PHASE		<<  5) |			//swap_phase
		     ((ac_timing->tRC  - 1)	<<  8) |			//ras2ras_same_bank_dly
		     ((ac_timing->tFAW - 1)	<< 16) |			//tfaw_dly
		     ((ZQC_SHORT ? (ac_timing->tZQCS - 1) : (ac_timing->tZQoper- 1)) << 24));	//zqc_dly
#endif

#endif
	reg_write_ctrl(DLY_CONFIG_4, reg_value);


#if defined(LPDDR2)
	reg_value = ((0x2  <<  0) |						//rd2mrr_dly
		     (round_up(WL + 1 + 4 + tWTR) <<  4) |			//wr2mrr_dly
		     (tMRR <<  8) |						//mrr2rd_dly
		     (tMRR << 12));						//mrr2wr_dly

	#ifdef UNIQUIFY_LPDDR2_MRR
	reg_write_ctrl(MRR_DLY_CONFIG, reg_value);
	#endif
#endif

#if defined(LPDDR3)
	reg_value = ((0x2  <<  2) |						//rd2mrr_dly
		     (round_up(WL + 1 + 4 + tWTR) <<  4) |			//wr2mrr_dly
		     (round_up(WL + 1 + 4 + tMRR) <<  8) |			//mrr2rd_dly
		     (round_up(WL + 1 + 4 + tMRR) << 12));			//mrr2wr_dly
	#ifdef UNIQUIFY_LPDDR2_MRR
	reg_write_ctrl(MRR_DLY_CONFIG, reg_value);
	#endif
#endif

	// ZQC_CONFIG_1 : Address 7'h62
#if (USE_NSIH_HEADER == 0)
#if defined(DDR3) || defined(DDR4)
	reg_value = ((ZQC_INTERVAL	<<  0) |				// zqc_exp_cnt
		     (AUTO_ZQC_ENABLE	<< 22) |				// auto_zqc_enable
		     (ZQC_SHORT		<< 23) |				// zqc_short
//		     (tXS		<< 24));				// self_ref_to_non_rd_cmd_dly
		     (0x6C		<< 24));		// 0x88 ?? why?
#elif defined(LPDDR2) || defined(LPDDR3)
	reg_value = ((ZQC_INTERVAL	<<  0) |				// zqc_exp_cnt
		     (AUTO_ZQC_ENABLE	<< 22) |				// auto_zqc_enable
		     (ZQC_SHORT		<< 23) |				// zqc_short
		     (tXSR		<< 24));				// self_ref_to_non_rd_cmd_dly
#endif

#else	// #if (USE_NSIH_HEADER == 0)

#if defined(DDR3) || defined(DDR4)

	reg_value = ((ZQC_INTERVAL	<<  0) |				// zqc_exp_cnt
		     (AUTO_ZQC_ENABLE	<< 22) |				// auto_zqc_enable
		     (ZQC_SHORT		<< 23) |				// zqc_short
		     (ac_timing->tXS	<< 24));				// self_ref_to_non_rd_cmd_dly
#elif defined(LPDDR2) || defined(LPDDR3)
	reg_value = ((ZQC_INTERVAL	<<  0) |				// zqc_exp_cnt
		     (AUTO_ZQC_ENABLE	<< 22) |				// auto_zqc_enable
		     (ZQC_SHORT 	<< 23) |				// zqc_short
		     (tXSR		<< 24));				// self_ref_to_non_rd_cmd_dly
#endif

#endif
	reg_write_ctrl(ZQC_CONFIG, reg_value);

	// AUTO_SCL_CTRL: Address 7'h16
	reg_value = ((AUTO_SCL_INTERVAL	<<  0) |				//auto_scl_exp_cnt
		     (AUTO_SCL_ENABLE	<< 16));				//auto_scl_enable
	reg_write_ctrl(AUTO_SCL_CTRL, reg_value);
#if 0
	for (slice = 0; slice < MEM_CHIP_SELECTS; slice++) {
		reg_set_ctrl(ODT_CONFIG, (slice * MEM_CHIP_SELECTS));
		if (MEM_CHIP_SELECTS > 1) {
			// ODT_EN_CONFIG: Address 7'h13
			reg_set_ctrl(ODT_EN_CONFIG,
				((0x1 << slice) << 16)
					| (((1 << slice << (MEM_CHIP_SELECTS/2))) << 0));
		} else {
			reg_set_ctrl(ODT_EN_CONFIG, ((0x1 << slice) | 0x0) );
		}
	}
#else
	reg_write_ctrl(ODT_CONFIG, 0x00000000);	    				// 0x40
	reg_write_ctrl(ODT_EN_CONFIG, 0x00010002);	    			// 0x4c
	reg_write_ctrl(ODT_CONFIG, 0x00000002);	    				// 0x40
	reg_write_ctrl(ODT_EN_CONFIG, 0x00020001);	    			// 0x4c
#endif

	//ODT_CONFIG
#if defined(DDR3)
	reg_value = ((0x0		<<  0) |				//odt_en_sel
		     (((RL - WL - (RL - WL > 0 ? 1 : 0))/2) << 8) |		//odt_rd_set_dly
		     (0x1		<< 12) |				//odt_wr_set_dly
		     ((2 + (((RL - WL) % 2 == 0)
		     	&& (RL - WL > 0)) ? 1 : 0) << 16) |			//odt_len_rd
		     ((2 + SWAP_PHASE) << 20));					//odt_len_wr
#elif defined(DDR4)
	reg_value = ((0x0		<<  0) |				//odt_len_wr
		     (round_up((rl_d - wl_d + 1) / 2) << 8) |			//odt_len_rd
		     (0x1		<< 12) |				//odt_wr_set_dly
		     (0x2		<< 16) |				//odt_rd_set_dly
		     ((2 + SWAP_PHASE)	<< 20));				//odt_en_sel
#elif defined(LPDDR2)
	reg_value = ((0x0		<<  0) |				//odt_en_sel
		     ((RL_PARAM - 1)	<<  8) |				//odt_rd_set_dly
		     ((WL_PARAM - 3 + WL %  2) << 12) |				//odt_wr_set_dly
		     (0x2		<< 16) |				//odt_len_rd
		     ((3 - WL % 2)	<< 20));				//odt_len_wr
#elif defined(LPDDR3)
	reg_value = ((0x0		<<  0) |				//odt_en_sel
		     ((RL_PARAM - 3)	<<  8) |				//odt_rd_set_dly
		     ((WL_PARAM - 2)	<< 12) |				//odt_wr_set_dly
		     ((RL_PARAM - 3)	<< 16) |				//odt_len_rd
		     (0x2		<< 20));				//odt_len_wr
#endif
	reg_write_ctrl(ODT_CONFIG, reg_value);

	// DATA_XFR_CONFIG : Address 7'h11
	reg_value = (((WL_PARAM - C2D - 1 -
			(WL % 2 == 0 && !SWAP_PHASE)
				+ (WL % 2 == 0 ? 1 : 0))	<<  0)	|	//nxt_dt_rqst_dly
	             ((RL_PARAM - 3 - C2D)			<< 16)	|	//t_rddata_en
	             (0x3 << 24) |						//max_cmd_xfr_cnt
	             (0x7 << 28) |						//enable_bank_rules
		     (BIG_ENDIAN << 31));                                     	//bigendian
 	reg_write_ctrl(DATA_XFR_CONFIG, reg_value);

	// AXI_PORTN_HALT_REG
	reg_value = 0;
	//pval[`NUM_AXI_AGENTS - 1:0] = {`NUM_AXI_AGENTS{1'b1}};   //axi_portn_halt
	reg_write_ctrl(AXI_PORTN_HALT_REG, reg_value);

	// AXI_PORTN_FIFO_REG
#if 0
	if (DEFINED(UNIQUIFY_AXI_PORT_SYNC_FIFO)) {
		reg_value = 0;
//		pval[`NUM_AXI_AGENTS - 1:0] = {`NUM_AXI_AGENTS{1'b1}};		//axi_portn_fifo
	} else {
		reg_value = 0;
	}
#else
	reg_value = 0;
#endif
	reg_write_ctrl(AXI_PORTN_FIFO_REG, reg_value);
}
