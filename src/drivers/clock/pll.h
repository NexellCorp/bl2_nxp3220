/* Copyright (C) 2018  Nexell Co., Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the Nexell nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL,SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __PLL_H__
#define __PLL_H__

/* PLL(Phase Locked Loop) Register Map */
struct nx_pll_reg {
	volatile unsigned int pll_ctrl;						/* 0x00: PLL Control 0		*/
	volatile unsigned int pll_dbg0;						/* 0x04: PLL Debug 0		*/
	volatile unsigned int rsv0[2];						/* 0x08 ~ 0x0C: Reserved 0	*/
	volatile unsigned int pll_cnt0;						/* 0x10: PLL Count 0		*/
	volatile unsigned int pll_cnt1;						/* 0x14: PLL Count 1		*/
	volatile unsigned int pll_cnt2;						/* 0x18: PLL Count 2		*/
	volatile unsigned int pll_cnt3;						/* 0x1C: PLL Count 3		*/

	volatile unsigned int pll_cfg0;						/* 0x20: PLL Config 0		*/
	volatile unsigned int rsv1[3];						/* 0x24 ~ 0x2C: Reserved 1	*/
	volatile unsigned int pll_cfg1;						/* 0x30: PLL Config 1		*/
	volatile unsigned int rsv2[3];						/* 0x34 ~ 0x3C: Reserved 2	*/
	volatile unsigned int pll_cfg2;						/* 0x40: PLL Config 2		*/
	volatile unsigned int rsv3[3];						/* 0x44 ~ 0x4C: Reserved 3	*/
	volatile unsigned int pll_ctrl1;					/* 0x50: PLL Control 3		*/
	volatile unsigned int rsv4[3];						/* 0x54 ~ 0x5C: Reserved 4	*/
	volatile unsigned int pll_lockinfo;					/* 0x60: PLL Lock Infomration	*/
};

/* for PLL Control 0 Register */
enum oscmux {
	PLL_MUX_OSCCLK,
	PLL_MUX_PLL_FOUT,
};

enum nx_pll {
	PLL0,
	PLL1,
	PLLCPU,
	PLLDDR0,
	PLLDDR1,
};

#define PLL_LOCK_SHIFT		6
#define PLL_CURST_SHFIT		8
#define PLL_MUXSEL_SHIFT	3

#define PLL_LOCK_COUNT		0x200

#define	PLL_DIRTYFLAG		(1 << 1)
#define PLL_RUN_CHANGE		(1 << 0)					/* PLL Run change start. (Write Only) */
#define LINK_RESETB		(1 << 0)
#define LINK_BYPASS		(1 << 1)
#define UPDATE_CONFIG_DIRECTLY	(1 << 15)

/* Function Define */
int clock_is_stable(int num);
int get_pll_lock(int num);
int get_pll_freq(int num);
int get_pre_pll_freq(unsigned int num, unsigned int pm,
			unsigned int sk);
int nx_change_pll(int index, int pm, int sk, int sscg);
int pll_initialize(void);

#endif // #ifndef __PLL_H__
