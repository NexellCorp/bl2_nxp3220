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
#ifndef __CPU_H__
#define __CPU_H__

#define MC_PMU_PROGRAM_REGION		(128 + 32)

typedef enum {
	PMU_OP_0	= 0UL,
	PMU_OP_1	= 1UL,
	PMU_OP_2	= 2UL,
	PMU_OP_3	= 3UL,
	PMU_OP_ALL_0	= 0UL,
	PMU_OP_ALL_1	= 1UL
} cpu_pmu_op;

typedef enum {
	PMU_CMD_END                   = 0UL,
	PMU_CMD_NOP                   = 1UL,
	PMU_CMD_WAIT_STANDBYWFI       = 2UL,
	PMU_CMD_WAIT_STANDBYWFIL2     = 3UL,
	PMU_CMD_WAIT_PWRDNACKN        = 4UL,
	PMU_CMD_WAIT_EFUSEDONE        = 5UL,
	PMU_CMD_REG_nCORERESET        = 6UL,
	PMU_CMD_REG_nCOREPORESET      = 7UL,
	PMU_CMD_REG_nDBGRESET         = 8UL,
	PMU_CMD_REG_nETMRESET         = 9UL,
	PMU_CMD_REG_nL2RESET          = 10UL,
	PMU_CMD_REG_nMBISTRESET       = 11UL,
	PMU_CMD_REG_nSOCDBGRESET      = 12UL,
	PMU_CMD_REG_L1RSTDISABLE      = 13UL,
	PMU_CMD_REG_L2RSTDISABLE      = 14UL,
	PMU_CMD_REG_NL2RETENTION      = 15UL,
	PMU_CMD_REG_ACINACTM          = 16UL,
	PMU_CMD_REG_CLAMPCPUOUT       = 17UL,
	PMU_CMD_REG_CLAMPCOREOUT      = 18UL,
	PMU_CMD_REG_CPUPWRDOWNPRE     = 19UL,
	PMU_CMD_REG_CPUPWRDOWNALL     = 20UL,
	PMU_CMD_REG_COREPWRDOWNPRE    = 21UL,
	PMU_CMD_REG_COREPWRDOWNALL    = 22UL,
	PMU_CMD_REG_DFTRAMHOLD        = 23UL,
	PMU_CMD_REG_DFTRAMBYP         = 24UL,
	PMU_CMD_REG_PWRDNREQN         = 25UL,
	PMU_CMD_REG_PWRDUP            = 26UL,
	PMU_CMD_REG_MASKIRQ           = 27UL,
	PMU_CMD_REG_EMA               = 28UL,
	PMU_CMD_REG_EMAW              = 29UL,
	PMU_CMD_REG_EMAS              = 30UL,
	PMU_CMD_REG_CLKOFF            = 31UL
}cpu_pmu_instruction;

struct nx_cpupmu_reg {
	volatile unsigned int pc_cur;				// 0x00
	volatile unsigned int pc_move;				// 0x04
	volatile unsigned int pc_wakeup;			// 0x08
	volatile unsigned int secure_config;			// 0x0C
	volatile unsigned int counter_scale;			// 0x10
	volatile unsigned int reserved0[3];			// 0x14 ~ 0x1C
	volatile unsigned int opr[16];				// 0x20 ~ 0x5C
	volatile unsigned int opmask[8];			// 0x60 ~ 0x7C
	volatile unsigned int reserved1[96];			// 0x80 ~ 0x1FC
	volatile unsigned int program[MC_PMU_PROGRAM_REGION/2];	// 0x200 ~ 0x250
};

#endif
