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
