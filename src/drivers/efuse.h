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
#ifndef __EFUSE_H__
#define __EFUSE_H__

struct nx_efuse_reg {
	volatile unsigned int chip_name[12];					/* 0x000 */
	volatile unsigned int _rev0;						/* 0x030 */
	volatile unsigned int guid[4];						/* 0x034 */
	volatile unsigned int ec0;						/* 0x044 */
	volatile unsigned int _rev1;						/* 0x048 */
	volatile unsigned int ec2;						/* 0x04C */
	volatile unsigned int blow[3];						/* 0x050 */
	volatile unsigned int _rev2;						/* 0x05C */
	volatile unsigned int blowd[4];						/* 0x060 */
	volatile unsigned int _rev3[36];					/* 0x070 */
	volatile unsigned int ecid[4];						/* 0x100 */
	volatile unsigned int sbootkey0[4];					/* 0x110 */
	volatile unsigned int sbootkey1[4];					/* 0x120 */
	volatile unsigned int _rev4[8];						/* 0x130 */
	volatile unsigned int sboothash0[8];					/* 0x150 */
	volatile unsigned int _rev5[8];						/* 0x170 */
	volatile unsigned int sboothash1[8];					/* 0x190 */
	volatile unsigned int sboothash2[8];					/* 0x1B0 */
	volatile unsigned int sjtag[4];						/* 0x1D0 */
	volatile unsigned int anti_rb[4];					/* 0x1E0 */
	volatile unsigned int efuse_cfg;					/* 0x1F0 */
	volatile unsigned int efuse_prot;					/* 0x1F4 */
	volatile unsigned int _rev6[2];						/* 0x1F8 */
	volatile unsigned int boot_cfg;						/* 0x200 */
	volatile unsigned int _rev7[3];						/* 0x204 */
	volatile unsigned int back_enc_ek[8];					/* 0x210 */
	volatile unsigned int root_enc_key[8];					/* 0x230 */
	volatile unsigned int cm0_sboot_key[16];				/* 0x250 */
	volatile unsigned int root_priv_key[17];				/* 0x290 */
	volatile unsigned int _rev8[11];					/* 0x2D4 */
	volatile unsigned int puf[136];						/* 0x300 */
	volatile unsigned int puf_cfg;						/* 0x520 */
	volatile unsigned int cm0_anti_rb;					/* 0x524 */
	volatile unsigned int cm0_anti_rb_cfg;					/* 0x528 */
	volatile unsigned int _rev9;						/* 0x52C */
	volatile unsigned int hpm_ids[4];					/* 0x530 */
};

/* Function Define */
 int efuse_get_bootcfg(void);
 int efuse_get_ecid(int index);

#endif //__EFUSE_H__
