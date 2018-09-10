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
#include <pll.h>
#include <pms.h>

#define NUMBER_OF_MAX_PLL			5

/*
 * The value of the oscillator is determined by the external.
 * However, there is no possibility of change at 24Mhz.
 */
static unsigned int g_osc_khz = (24 * 1000);

static int get_pll_baseaddr(int index)
{
	const unsigned int baseaddr[NUMBER_OF_MAX_PLL] = {
		PHY_BASEADDR_PLL0,
		PHY_BASEADDR_PLL1,
		PHY_BASEADDR_PLL_CPU,
		PHY_BASEADDR_PLL_DDR0,
		PHY_BASEADDR_PLL_DDR1
	};

	return baseaddr[index];
}

/* Chage the mux (PLL(Phase Locked Loop)or Oscillator) */
void pll_set_oscmux(unsigned int num, unsigned int muxsel)
{
	struct nx_pll_reg *base
		= (struct nx_pll_reg*)get_pll_baseaddr(num);

	if (muxsel != PLL_MUX_PLL_FOUT)
		mmio_clear_32(&base->pll_ctrl, (1 << 3));
	else
		mmio_set_32(&base->pll_ctrl, (1 << 3));				/* OSC_MUX (0: OSCCLK, 1:PLL_FOUT) */
}

/* Check the PLL-Lock State (PLL(Phase Locked Loop)or Oscillator) */
static int check_pll_lock(int num)
{
	struct nx_pll_reg *base
		= (struct nx_pll_reg*)get_pll_baseaddr(num);

	unsigned int st_count, st_check;
	unsigned int cur_st, lock, mux;

	st_count = mmio_read_32(&base->pll_dbg0);				/* Lock Count				*/
	lock = ((mmio_read_32(&base->pll_ctrl) >> PLL_LOCK_SHIFT) & 0x1);	/* PLL Lock Signal (0:unlock, 1:lock)	*/
	cur_st = ((mmio_read_32(&base->pll_ctrl) >> PLL_CURST_SHFIT) & 0x1F);	/* Current State (for debug) (1: IDLE)	*/

	mux = (mmio_read_32(&base->pll_ctrl) >> 3) & 0x1;

	if (mux)
		st_check = 1;
	else
		st_check = (st_count > PLL_LOCK_COUNT);

	if (((cur_st == 1) && (lock == 1)) && st_check)
		return true;
	else
		return false;
}

/* Check the PLL-Clock Stable */
int clock_is_stable(int num)
{
	int timeout = 0xFFFFFFF;

	while (check_pll_lock(num) == false) {					/* true is (pll locked), false is (pll unlocked) */
		if (timeout-- <= 0)
			return false;
	}

	return true;
}

int get_pll_lock(int num)
{
	struct nx_pll_reg *base
		= (struct nx_pll_reg*)get_pll_baseaddr(num);

	return ((mmio_read_32(&base->pll_ctrl) >> PLL_LOCK_SHIFT) & 0x1);	/* PLL Lock Signal (0:unlock, 1:lock)	*/
}

/* Get the PLL-Frequency */
int get_pll_freq(int num)
{
	struct nx_pll_reg *base
		= (struct nx_pll_reg*)get_pll_baseaddr(num);

	unsigned int reg_value, reg_value1, p, m, s, k;
	unsigned int temp = 0;

	reg_value  = mmio_read_32(&base->pll_cfg1);				// SoC Issue Check
	reg_value  = mmio_read_32(&base->pll_cfg1);
	reg_value1 = mmio_read_32(&base->pll_cfg2);

	p = (reg_value  >> PLL_P_BITPOS) & 0xFFFF;
	m = (reg_value  >> PLL_M_BITPOS) & 0xFFFF;
	s = (reg_value1 >> PLL_S_BITPOS) & 0xFFFF;
	k = (reg_value1 >> PLL_K_BITPOS) & 0xFFFF;

	if (((num == 1) || (num == 4)) && k)
		temp = ((((k * 1000) / 65536) * g_osc_khz) / p) >> s;

	temp = ((((m * g_osc_khz) / p) >> s) * 1000) + temp;

	/* @breif: PLL1/PLLDDRx is based on 2 divisions. */
	if ((num == 1) || (num == 3) || (num == 4))
		temp /= 2;

	return temp;
}

int get_pre_pll_freq(unsigned int num, unsigned int pm,
			unsigned int sk)
{
	unsigned int p = ((pm >>  0) & 0xFFFF);
	unsigned int m = ((pm >> 16) & 0xFFFF);
	unsigned int s = ((sk >>  0) & 0xFFFF);
	unsigned int k = ((sk >> 16) & 0xFFFF);

	unsigned int temp = 0;

	if (((num == 1) || (num == 4)) && k)
		temp = ((((k * 1000) / 65536) * g_osc_khz) / p) >> s;

	temp = ((((m * g_osc_khz) / p) >> s) * 1000) + temp;

	/* @breif: PLL1/PLLDDRx is based on 2 divisions. */
	if ((num == 1) || (num == 3) || (num == 4))
		temp /= 2;

	return temp;
}

/* Get the PLL (P/M/S/K) Value */
static void get_pmsk_parser(int *pm, int *sk, int *sscg)
{
	int i;

	/* step xx. calcurate the phase locked loop (0/1/2/3/4) frequency */
	for (i = 0; i < NUMBER_OF_MAX_PLL; i++) {
		pm[i] = g_nsih->clk[i].pll_pm;
		sk[i] = g_nsih->clk[i].pll_sk;

		/* step xx. check the k-value (set the sscg) */
		if ((sk[i] >> PLL_K_BITPOS) & PLL_VAL_BITMASK)
			sscg[i] = (g_nsih->clk[i].sscg |
					(1 << PLL_SSCG_ENB_BITPOS));
		else
			sscg[i] = 0;
	}
}

/*************************************************************
 * Chage teh PLL(Phase Locked Loop) sequence
 * must go through the following steps:
 *
 * Step 01. Calcurate the PLL(phase locked loop) Frequency.
 *	       -> Get the (P/M/S/K) value.
 * Step 02. Check the k-value (set the sscg).
 * Step 03. Check the PLL Lock State.
 * Step 04. Set the PLL Output.
 * Step 05. Set the PMS value.
 *	      -> Set the Dirty-Flag and RUN_CHAGE after setting the PMS value.
 *************************************************************/

/* change the pll-frequency */
int nx_change_pll(int index, int pm, int sk, int sscg)
{
	struct nx_pll_reg *base =
		(struct nx_pll_reg *)get_pll_baseaddr(index);

	if (clock_is_stable(index))
		return false;

	pll_set_oscmux(index, PLL_MUX_OSCCLK);

	mmio_write_32(&base->pll_cfg1, pm);
	mmio_write_32(&base->pll_cfg2, sk);
	mmio_write_32(&base->pll_ctrl1, sscg);

	mmio_clear_32(&base->pll_cfg0, (LINK_BYPASS | LINK_RESETB));
	mmio_set_32(&base->pll_ctrl, PLL_DIRTYFLAG);
	mmio_set_32(&base->pll_ctrl, UPDATE_CONFIG_DIRECTLY);

	udelay(10);								/* 2us * 5 */

	mmio_set_32(&base->pll_cfg0, LINK_RESETB);
	mmio_set_32(&base->pll_ctrl, PLL_DIRTYFLAG);
	mmio_set_32(&base->pll_ctrl, UPDATE_CONFIG_DIRECTLY);

	udelay(300);								/* 150us * 2 */

	pll_set_oscmux(index, PLL_MUX_PLL_FOUT);

	return true;
}

int pll_initialize(void)
{
	int pm[5], sk[5], sscg[5];
	int i, ret = false;

	get_pmsk_parser(pm, sk, sscg);

	for (i = 2; i < NUMBER_OF_MAX_PLL; i++) {
		ret = nx_change_pll(i, pm[i], sk[i], sscg[i]);
		if (ret != true) {
//			PANIC("pll clock is not stable!!\r\n");
			return false;
		}
		clock_is_stable(i);
	}

	return ret;
}
