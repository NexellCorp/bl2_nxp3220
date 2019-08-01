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
#include <printf.h>
#include <chip.h>
#include <alive.h>
#include <libnx.h>
#include "dphy.h"
#include "dctrl.h"
#include "ddr3/ddr3_sdram.h"

void wrtest(u32 *dst, u32 *src);
void burstread8(u32 *src, u32 *buf);
void burstwrite8(u32 *dst, u32 *buf);
void burstread4(u32 *src, u32 *buf);
void burstwrite4(u32 *dst, u32 *buf);
#define DPHY_BASE_ADDR                  0x23091000
#define DCTRL_BASE_ADDR			0x23090000

extern union DDR3_SDRAM_MR MR0, MR1, MR2, MR3;

void save_cal_data(int dnum, unsigned int offset, int rw)
{
	struct nx_vddpwr_reg *pvddpwr =
		(struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR;
	const unsigned int scratch_offset = 7;
	unsigned int rv, sb = (dnum >> 2) + rw * 4 + scratch_offset;

	if (dnum & 3) {
		rv = pvddpwr->new_scratch[sb];
//		rv &= ~(0xff << (8 * (dnum & 3)));
	} else
		rv = 0;

	rv |= offset << (8 * (dnum & 3));
	pvddpwr->new_scratch[sb] = rv;
//	printf("%d:%08x\r\n", sb, pvddpwr->new_scratch[sb]);
}
#if 0
static void reg_write_phy(int addr, unsigned int reg_value)
{
	*(volatile unsigned int *)(DPHY_BASE_ADDR + addr) = reg_value;
}
static unsigned int reg_read_phy(int addr)
{
	return *(volatile unsigned int *)(DPHY_BASE_ADDR + addr);
}
static void reg_write_ctrl(int addr, unsigned int reg_value)
{
	*(volatile unsigned int *)(DCTRL_BASE_ADDR + addr) = reg_value;
}
static unsigned int reg_read_ctrl(int addr)
{
	return *(volatile unsigned int *)(DCTRL_BASE_ADDR + addr);
}
#endif


#define READTRIM	0
#define WRITETRIM	1
void settrim(u32 lane, u32 line, u32 offset, int rw)
{
	u32 sign, reg;
	if (rw == READTRIM)
		reg = IP_DQ_DQS_BITWISE_TRIM;
	else
		reg = OP_DQ_DM_DQS_BITWISE_TRIM;

	if (offset <= 63) {
		sign = 0;
		offset = 63 - offset;
	} else {
		sign = 1;
		offset -= 63;
	}

	reg_write_phy(PHY_LANE_SEL,
		line << 8 |			// 0~7:DQ, 8:DQS
		lane * 7);
	reg_write_phy(reg,
		1 << 7 |		// set override
		sign  << 6 |		// 0: dec, 1: enc
		(offset & 0x3F) << 0);
}

void dmc_host_cmd(
		unsigned int offset,	// command number
		unsigned int cmd_1st,	// 1st cycle command
		unsigned int cmd_2nd,	// 2nd cycle command
		unsigned int ba,	// bank address bits value
		unsigned int aa,	// address bits value
		unsigned int wait_time	// wait time after command
		)
{
	unsigned mem_cmd0;
	unsigned mem_cmd1;
	// [23:0] : {cmd_2nd, cmd_1st}
	// [31:24] : address[7:0]
	mem_cmd0 = (aa & 0xFF) << 24 |
		cmd_2nd << 12 |
		cmd_1st << 0;
	// [7:0] : address[15:8]
	// [10:8] : bank address[2:0]
	// [20:12] : wait time (ex, tRP, tMOD)
	mem_cmd1 = (wait_time - 1) << 12 |
		ba << 8 |
		(aa >> 8) << 0;

	reg_write_ctrl(0x80 + (offset * 4), mem_cmd0);
	reg_write_ctrl(0xC0 + (offset * 4), mem_cmd1);
}

void dmc_host_cmd_mr(
		unsigned int offset,	// command number
		unsigned int cmd_1st,	// 1st cycle command
		unsigned int cmd_2nd,	// 2nd cycle command
		unsigned int mr_index,	// bank address bits value
		unsigned int opcode,	// address bits value
		unsigned int wait_time	// wait time after command
		)
{
	unsigned mem_cmd0;
	unsigned mem_cmd1;
	// [23:0] : {cmd_2nd, cmd_1st}
	// [31:24] : address[7:0]
	mem_cmd0 = (opcode & 0xFF) << 24 |
		cmd_2nd << 12 |
		cmd_1st << 0;
	// [7:0] : address[15:8]
	// [10:8] : bank address[2:0]
	// [20:12] : wait time (ex, tRP, tMOD)
	mem_cmd1 = (wait_time - 1) << 12 |
		mr_index << 8 |
		(opcode >> 8) << 0;

	reg_write_ctrl(0x80 + (offset * 4), mem_cmd0);
	reg_write_ctrl(0xC0 + (offset * 4), mem_cmd1);
}

		//[11]:nWE, [10]:CAS, [9]:RAS, [8:1]:nCS[7:0], [0]: CKE
#define MRS	(0 << 11 | 0 << 10 | 0 << 9 | 0x00 << 1 | 1 << 0)
#define PREA	(0 << 11 | 1 << 10 | 0 << 9 | 0x00 << 1 | 1 << 0)
#define ACT	(1 << 11 | 1 << 10 | 0 << 9 | 0x00 << 1 | 1 << 0)
#define NOP	(1 << 11 | 1 << 10 | 1 << 9 | 0xFF << 1 | 1 << 0)
#define SRE	(1 << 11 | 0 << 10 | 0 << 9 | 0x00 << 1 | 0 << 0)
#define SRX	(1 << 11 | 1 << 10 | 1 << 9 | 0x00 << 1 | 1 << 0)

void MPR(int onoff)
{
	unsigned int cmd_1st, cmd_2nd;

	// When reading pattern data, the bank active command should not occur.
	// Therefore, before the MPR command is given,
	// a dummy read is performed to bank active in advance.
	unsigned int rdata = *(volatile unsigned int *)(0x40000000);     // 64 bit align

	cmd_1st = PREA;	// precharge all
	cmd_2nd = NOP;

	dmc_host_cmd(
		0,		// command number
		cmd_1st,	// 1st cycle command
		cmd_2nd,	// 2nd cycle command
		0xF,		// bank address bits value
		1 << 10,	// address bits value : precharge all A10 = 1
		14		// wait time after command (tRP/tCK)
		);

	cmd_1st = MRS;	// Mode reg set

	int opcode;
	if (onoff)
		opcode = 1 << 2;
	else
		opcode = 0;
	dmc_host_cmd_mr(
		1,		//int offset,
		cmd_1st,	//int cmd_1st,
		cmd_2nd,	//int cmd_2nd,
		0x3,		//int mr_index,(MR3 : MPR)
		opcode,		//int opcode,
		12		//int wait_time
		);
	// cmd push
	reg_write_ctrl(HOST_CMD_ISSUE, 0x11);
	// Wait until all commands are executed.
	do {
		rdata = reg_read_ctrl(HOST_CMD_ISSUE);
	} while (rdata & 1 << 4);
}

unsigned int testpattern[8] = {
#if 1
	0xffff0000, 0xffff0000,
	0xffff0000, 0xffff0000,
	0xffff0000, 0xffff0000,
	0xffff0000, 0xffff0000
#else
	0x0000ffff, 0x0000ffff,
	0x0000ffff, 0x0000ffff,
	0x0000ffff, 0x0000ffff,
	0x0000ffff, 0x0000ffff
#endif
};

const unsigned int wtp[8] = {
	0x789b3de0, 0xf10e4a56,
	0xc6a21359, 0x427a9bef,
	0x29a17c9d, 0x02eb36fa,
	0x1b0f36c8, 0xf10e4a56
};

int checkpattern(unsigned int *src, unsigned int *target, unsigned int mask)
{
	int i;
	for (i = 0; i < 8; i++) {
		if ((src[i] & mask) != (target[i] & mask)) {
#if 0
			unsigned int *buf = target;
			printf("%X %X %X %X %X %X %X %X\r\n",
					buf[0], buf[1], buf[2], buf[3],
					buf[4], buf[5], buf[6], buf[7]);
#endif
			return 0;
		}
	}
	return 1;
}

#ifdef DDR_TEST_MODE
static int memtest_checkpattern(unsigned int *src, unsigned int *target,
			 unsigned int mask)
{
	int i;
	int res = 1;
	for (i = 0; i < 8; i++) {
		if ((src[i] & mask) != (target[i] & mask)) {
			unsigned int *buf = target;

			printf("src[%d] %x buf[%d] %x \r\n",0, src[0], 0, buf[0]);
			printf("src[%d] %x buf[%d] %x \r\n",1, src[1], 1, buf[1]);
			printf("src[%d] %x buf[%d] %x \r\n",2, src[2], 2, buf[2]);
			printf("src[%d] %x buf[%d] %x \r\n",3, src[3], 3, buf[3]);
			printf("src[%d] %x buf[%d] %x \r\n",4, src[4], 4, buf[4]);
			printf("src[%d] %x buf[%d] %x \r\n",5, src[5], 5, buf[5]);
			printf("src[%d] %x buf[%d] %x \r\n",6, src[6], 6, buf[6]);
			printf("src[%d] %x buf[%d] %x \r\n",7, src[7], 7, buf[7]);
			return 0;
		}
	}
	return 1;
}
#endif

#if 1
/* 60 120 240 240 */
/* 0001, 0010:240, 0100, 0011:120, 0110, 0101:80, 1000, 0111:60
 * 1001, 1010:48, 1100, 1011:40, 1110, 1101:34, 1111:30
 *
 * 1:240, 3:120, 5:80, 7:60, 9:48, 11:40, 13:34, 15:30
 */
void setreadodt(unsigned int rodt)
{
	unsigned int reg_value = reg_read_phy(PHY_PAD_CTRL);
	reg_value = (reg_value & ~(0xFUL << 0)) | (((rodt << 1) + 1) & 0xF) << 0;
	reg_write_phy(PHY_PAD_CTRL, reg_value);
}

void ddr3mrs(SDRAM_MODE_REG MRx, union DDR3_SDRAM_MR opcode)
{
	unsigned int cmd_1st, cmd_2nd, rdata;

	cmd_1st = MRS;	// Mode reg set
	cmd_2nd = NOP;

	dmc_host_cmd_mr(
		0,		//int offset,
		cmd_1st,	//int cmd_1st,
		cmd_2nd,	//int cmd_2nd,
		MRx,		//int mr_index,
		opcode.REG,	//int opcode,
		12		//int wait_time
		);

	reg_write_ctrl(HOST_CMD_ISSUE, 1 << 4 |		// cmd valid
					0 << 0);	// cmd num, 0: 1
	do {
		rdata = reg_read_ctrl(HOST_CMD_ISSUE);
	} while (rdata & 1 << 4);
}

/* 0: RZQ/6, 1: RZQ/7, 2, 3: revd */
void setreadds(unsigned int rds)
{
	union DDR3_SDRAM_MR opcode;
	opcode.REG = MR1.REG;
	opcode.REG &= ~(1 << 1 | 1 << 5);
	opcode.REG |=
		(rds & 1 << 0) << 1 |		// 1: ODS0
		(rds & 1 << 1) << 4;		// 5: ODS1

	MR1.REG = opcode.REG;

	ddr3mrs(SDRAM_MODE_REG_MR1, opcode);
}

void setwriteds(unsigned int wds)
{
	unsigned int reg_value = reg_read_phy(PHY_PAD_CTRL);
	reg_value = (reg_value & ~(0xFUL << 4)) | (((wds << 1) + 1) & 0xF) << 4;
	reg_write_phy(PHY_PAD_CTRL, reg_value);
}

/* 0: disable, 1: RZQ/4, 2: RZQ/2, 3: RZQ/6, 4: RZQ/12, 5: RZQ/8, 6, 7: revd */
void setwriteodtn(unsigned int wodtn)
{
	union DDR3_SDRAM_MR opcode;
	opcode.REG = MR1.REG;
	opcode.REG &= ~(1 << 2 | 1 << 6 | 1 << 9);
	opcode.REG |=
		(wodtn & 1 << 0) << 2 |		// 2: RTT_Nom0
		(wodtn & 1 << 1) << 5 |		// 6: RTT_Nom1
		(wodtn & 1 << 2) << 7;		// 9: RTT_Nom2

	MR1.REG = opcode.REG;

	ddr3mrs(SDRAM_MODE_REG_MR1, opcode);
}

/* 0: disable, 1: RZQ/4(60ohm), 2: RZQ/2(120ohm), 3: revd */
void setwriteodtw(unsigned int wodtw)
{
	union DDR3_SDRAM_MR opcode;
	opcode.REG = MR2.REG;
	opcode.REG &= ~(3 << 9);
	opcode.REG |= (wodtw & 0x3) << 9;	// 9: RTT_WR

	MR2.REG = opcode.REG;

	ddr3mrs(SDRAM_MODE_REG_MR2, opcode);
}
#endif

#define RETRYCNT	(64 * 1)
int checkresult(unsigned int targetaddr, unsigned int bits,
		int trycnt, int rw)
{
	unsigned int buf[8], *tp;
	if (rw == READTRIM)
		tp = testpattern;
	else
		tp = (unsigned int *)wtp;

	do {
		if (rw == WRITETRIM)
			burstwrite8((unsigned int *)targetaddr, tp);
		burstread8((unsigned int *)targetaddr, buf);
		if (!checkpattern(tp, buf, 1 << (bits) | 1 << (bits + 16)))
			return 0;
	} while (--trycnt);
	return 1;
}

#ifdef DDR_TEST_MODE
int read_checkresult(unsigned int targetaddr, unsigned int bits,
		int trycnt)
{
	unsigned int buf[8], *tp;
	tp = testpattern;
	while (1) {
		burstread8((unsigned int *)targetaddr, buf);
		if (!checkpattern(tp, buf, 0xffffffff))
			return 1;
	}
}
#endif

int findbest(unsigned char dd[], int cnt)
{
	int best = 0, i, bi = 0;

	if (cnt == 0)
		return 0xff;

	for (i = 0; i < cnt; i++) {
		int cd = dd[i * 2 + 1] - dd[i * 2 + 0] + 1;
		if (cd < 3)
			continue;
		if (cd > best) {
			best = cd;
			bi = i;
		}
	}

	if (best == 0)
		return 0xff;

	return bi;
}

int get_write_bit_margin(unsigned int targetaddr, unsigned int option)
{
	unsigned int lane, i;
	unsigned char cm[32];
	unsigned char lr[20];

	if (option & 1 << 0) {
		for (i = 63; i > 0; i--)
			printf("%d", i % 10);
		for (i = 0; i <= 63; i++)
			printf("%d", i % 10);
		printf("\r\n");
	}

	for (lane = 0; lane < 2; lane++) {
		unsigned int line;

		settrim(lane, 8, 59, WRITETRIM);
		settrim(lane, 9, 32, WRITETRIM);
		for (line = 0; line < 8; line++) {
			unsigned int offset, left = 63, right = 63, prev;
			int result, lrc;

			prev = 0;
			lrc = 0;
			for (offset = 0; offset < 127; offset++) {
				settrim(lane, line, offset, WRITETRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						4, WRITETRIM);

				if (result) {
					if (prev == 0)
						lr[lrc * 2 + 0] = (unsigned char)offset;
					prev = 1;
				} else {
					if (prev == 1) {
						lr[lrc * 2 + 1] = (unsigned char)offset - 1;
						lrc++;
					}
					prev = 0;
				}

				if (option & 1 << 0)
					printf("%c", result ? 'O' : '-');
			}
			if (option & 1 << 0)
				printf("\r\n");

			lrc = findbest(lr, lrc);
			if (lrc == 0xff) {
				settrim(lane, line, 63, WRITETRIM);
				cm[(line + lane * 8) * 2 + 0] = 63;
				cm[(line + lane * 8) * 2 + 1] = 0;
				continue;
			}

			unsigned int center, margin;
			/* escalate left side test precision */
//			offset = lr[lrc * 2 + 0];
			left = offset = lr[lrc * 2 + 0];
//			unsigned int limit = lr[lrc * 2 + 1];
			right = lr[lrc * 2 + 1];
			center = right - ((right - left) >> 1);
			do {
				settrim(lane, line, offset, WRITETRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						RETRYCNT, WRITETRIM);

				if (result)
					break;
				offset++;
			} while (offset < center);
			left = offset;

			/* escalate right side test precision */
			offset = lr[lrc * 2 + 1];
//			limit = lr[lrc * 2 + 0];
			do {
				int result;
				settrim(lane, line, offset, WRITETRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						RETRYCNT, WRITETRIM);

				if (result)
					break;
				offset--;
			} while (offset > center);
			right = offset;

			center = right - ((right - left) >> 1);
			if (right - left <= 0) {
				center = 63;
				margin = 0;
			} else
				margin = right - center;

			settrim(lane, line, center, WRITETRIM);
			cm[(line + lane * 8) * 2 + 0] = center;
			cm[(line + lane * 8) * 2 + 1] = margin;
		}
	}

	for (i = 0; i < 16; i++)
		save_cal_data(i, cm[i * 2 + 0], WRITETRIM);

	if (option & 1 << 1) {
		for (i = 0; i < 16; i++) {
			printf("DQ%02d ", i);
		}
		printf("\r\n");
		for (i = 0; i < 16; i++) {
			printf(" %3d ", cm[i * 2 + 0] - 63);
		}
		printf("\r\n");
		for (i = 0; i < 16; i++) {
			printf("  %2d ", cm[i * 2 + 1]);
		}
		printf("\r\n\n");
	}

	int minm = 31;
	for (i = 0; i < 16; i++)
		if (minm > cm[i * 2 + 1])
			minm = cm[i * 2 + 1];
	return minm;
}

int get_read_bit_margin(unsigned int targetaddr, unsigned int option)
{
#if 1
	unsigned int lane, i;
	unsigned char cm[32];
	unsigned char lr[20];

	if (option & 1 << 0) {
		for (i = 63; i > 0; i--)
			printf("%d", i % 10);
		for (i = 0; i <= 63; i++)
			printf("%d", i % 10);
		printf("\r\n");
	}

	MPR(1);
	for (lane = 0; lane < 2; lane++) {
		unsigned int line;

		settrim(lane, 8, 32, READTRIM);
		for (line = 0; line < 8; line++) {
			unsigned int offset, left = 63, right = 63, prev;
			int result, lrc;

			prev = 0;
			lrc = 0;
			for (offset = 0; offset < 127; offset++) {
				settrim(lane, line, offset, READTRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						4, READTRIM);

				if (result) {
					if (prev == 0)
						lr[lrc * 2 + 0] = (unsigned char)offset;
					prev = 1;
				} else {
					if (prev == 1) {
						lr[lrc * 2 + 1] = (unsigned char)offset;
						lrc++;
					}
					prev = 0;
				}

				if (option & 1 << 0)
					printf("%c", prev ? 'O' : '-');
			}
			if (option & 1 << 0)
				printf("\r\n");

			lrc = findbest(lr, lrc);
			if (lrc == 0xff) {
				settrim(lane, line, 63, READTRIM);
				cm[(line + lane * 8) * 2 + 0] = 63;
				cm[(line + lane * 8) * 2 + 1] = 0;
				continue;
			}

			unsigned int center, margin;
			/* escalate left side test precision */
//			offset = lr[lrc * 2 + 0];
			left = offset = lr[lrc * 2 + 0];
//			unsigned int limit = lr[lrc * 2 + 1];
			right = lr[lrc * 2 + 1];
			center = right - ((right - left) >> 1);
			do {
				settrim(lane, line, offset, READTRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						RETRYCNT, READTRIM);

				if (result)
					break;
				offset++;
			} while (offset < center);
			left = offset;

			/* escalate right side test precision */
			offset = lr[lrc * 2 + 1];
//			limit = lr[lrc * 2 + 0];
			do {
				int result;
				settrim(lane, line, offset, READTRIM);
				result = checkresult(targetaddr, lane * 8 + line,
						RETRYCNT, READTRIM);

				if (result)
					break;
				offset--;
			} while (offset > center);
			right = offset;


			center = right - ((right - left) >> 1);
			if (right - left <= 0) {
				center = 63;
				margin = 0;
			} else
				margin = right - center;

			settrim(lane, line, center, READTRIM);
			cm[(line + lane * 8) * 2 + 0] = center;
			cm[(line + lane * 8) * 2 + 1] = margin;
		}
	}
	MPR(0);

	for (i = 0; i < 16; i++)
		save_cal_data(i, cm[i * 2 + 0], READTRIM);

	if (option & 1 << 1) {
		for (i = 0; i < 16; i++) {
			printf("DQ%02d ", i);
		}
		printf("\r\n");
		for (i = 0; i < 16; i++) {
			printf(" %3d ", cm[i * 2 + 0] - 63);
		}
		printf("\r\n");
		for (i = 0; i < 16; i++) {
			printf("  %2d ", cm[i * 2 + 1]);
		}
		printf("\r\n\n");
	}
	int minm = 31;
	for (i = 0; i < 16; i++)
		if (minm > cm[i * 2 + 1])
			minm = cm[i * 2 + 1];
	return minm;
#else
	return 0;
#endif
}

void getdlllockvalue(int cnt)
{
	unsigned int i, lockvalue;
	struct lv {
		unsigned int lockv;
		unsigned int cnt;
	} lockv[8];

	int j;
	for (j = 0; j < 8; j++)
		lockv[j].cnt = 0;

	printf("check clock jitter\r\n");
	i = cnt;
	while (i--) {
		lockvalue = reg_read_phy(PHY_DLL_ADRCTRL) >> 24;
		for (j = 0; j < 16; j++) {
			if (lockv[j].lockv == lockvalue) {
				lockv[j].cnt++;
				break;
			}
			if (lockv[j].cnt == 0) {
				lockv[j].lockv = lockvalue;
				lockv[j].cnt++;
				break;
			}
		}
	}

	j = 0;
	do {
		printf("%d : %d\r\n", lockv[j].lockv, lockv[j].cnt);
	} while (lockv[++j].cnt);
}

void getimpedance(unsigned int testtargetaddr, unsigned int option, int cs)
{
	int i, j, k, cm, maxi = 0, maxj = 0, maxk = 0, maxm = 0;

	printf("read impedance test\r\n");
	for (i = 0; i < 8; i++) {
//		printf("controller odt:%d\r\n", i);
		setreadodt(i);
		for (j = 0; j < 2; j++) {
//			printf("dram ds:%d\r\n", j);
			setreadds(j);
			for (k = (cs ? 1 : 0); k < (1 + 2 * (cs ? 1 : 0)); k++) {
				if (cs) {
//					printf("dram write dynamic odt:%d\r\n", k);
					setwriteodtw(k);
				}
				cm = get_read_bit_margin(testtargetaddr, option);
				printf("co:%d, dds:%d, ddo:%d, cm:%d\r\n\n",
						i, j, k, cm);
				if (maxm < cm) {
					maxm = cm;
					maxi = i;
					maxj = j;
					maxk = k;
				}
			}
		}
	}
	setreadodt(maxi);
	setreadds(maxj);
	if (cs)
		setwriteodtw(maxk);
	printf("max impedance odt:%d, ds:%d, odtw:%d, margin:%d\r\n\n\n",
			maxi, maxj, maxk, maxm);

	maxm = 0;
	printf("write impedance test\r\n");
	for (i = 0; i < 8; i++) {
//		printf("controller ds:%d\r\n", i);
		setwriteds(i);
		for (j = 1; j < 6; j++) {
//			printf("dram odt:%d\r\n", j);
			setwriteodtn(j);
			for (k = (cs ? 1 : 0); k < 1 + 2 * (cs ? 1 : 0); k++) {
				if (cs) {
//					printf("dram write dynamic odt:%d\r\n", k);
					setwriteodtw(k);
				}
				cm = get_write_bit_margin(testtargetaddr, option);
				printf("cds:%d, don:%d, ddo:%d, cm:%d\r\n\n",
						i, j, k, cm);
				if (maxm < cm) {
					maxm = cm;
					maxi = i;
					maxj = j;
					maxk = k;
				}
			}
		}
	}
	printf("max impedance ds:%d, otdn:%d, odtw:%d, margin:%d\r\n\n",
			maxi, maxj, maxk, maxm);
	setwriteds(maxi);
	setwriteodtn(maxj);
	if (cs)
		setwriteodtw(maxk);
}

int trimtest(unsigned int testtargetaddr, unsigned int option)
{
	int ret = 1;
	if (option & 1 << 2)
		getdlllockvalue(100000);

	if (option & 1 << 5)
		getimpedance(testtargetaddr, option, 1);

	printf("phy ver:%x\r\n", *(unsigned int *)0x230911B0);

	if (option & 1 << 3) {
		ret = get_read_bit_margin(testtargetaddr, option);
		printf("read bit cal done\r\n");
	}

	if (option & 1 << 4) {
		ret = get_write_bit_margin(testtargetaddr, option);
		printf("write bit cal done\r\n\n");
	}

//	struct nx_alive_reg *palive = (struct nx_alive_reg *)PHY_BASEADDR_ALIVE;
//	palive->scratch = 0x4d656d43;	/* MemC */
	struct nx_vddpwr_reg *pvddpwr =
		(struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR;
	pvddpwr->new_scratch[6] = 0x4d656d43;   /* MemC */
//	printf("mem cal save marking\r\n");
	return ret;
}

void trimset(char readcal[], char writecal[])
{
	int i;

//	settrim(0, 8, 32, READTRIM);	/* DM */
//	settrim(1, 8, 32, READTRIM);
//	printf("read cal:\r\n");
	for (i = 0; i < 16; i++) {
		settrim(i >> 3, i & 0x7, readcal[i], READTRIM);
//		printf("%02d ", readcal[i] - 63);
	}

//	settrim(0, 8, 59, WRITETRIM);	/* DM */
//	settrim(0, 9, 32, WRITETRIM);	/* DQS */
//	settrim(1, 8, 59, WRITETRIM);
//	settrim(1, 9, 32, WRITETRIM);
//	printf("\r\nwrite cal:\r\n");
	for (i = 0; i < 16; i++) {
		settrim(i >> 3, i & 0x7, writecal[i], WRITETRIM);
//		printf("%02d ", writecal[i] - 63);
	}
//	printf("\r\n\n");
	struct nx_vddpwr_reg *pvddpwr =
		(struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR;
	pvddpwr->new_scratch[6] = 0;   /* clear marking */
}

int checkcaldata(char readcal[], char writecal[])
{
	int i, sum = 0;

	for (i = 0; i < 16; i++) {
		sum += readcal[i];
	}
	if (sum == 0)
		return 0;

	sum = 0;
	for (i = 0; i < 16; i++) {
		sum += writecal[i];
	}
	if (sum == 0)
		return 0;

	return 1;
}

#ifdef DDR_TEST_MODE
int get_read_test(unsigned int targetaddr)
{
	unsigned int result_check, i;
	unsigned int buf[8], *tp;
	tp = wtp;

	printf("Enter: %s !!!!!!\r\n", __func__);
	for (i = 0; i < 100000; i++) {
		burstwrite8((unsigned int *)targetaddr+(0x1000*i), tp);
		burstread8((unsigned int *)targetaddr+(0x1000*i), buf);
		result_check= memtest_checkpattern(tp, buf, 0xFFFFFFFF);

		if (!result_check) {
			printf("read test error!! %d 0x%x\n", i, targetaddr+(0x1000*i));
			return 0;
		}
	}
	printf("End: %s !!!!!!\r\n", __func__);

	return 1;
}
#endif
