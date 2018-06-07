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
#include <cpupmu.h>

#define BUFFER_LENGTH ((64+16)*2)

unsigned short PMU_PROGRAM_BUFFER[BUFFER_LENGTH] = { 0, };
unsigned int CPU_PWR_UP, CLUSTER_PWR_UP, CPU_PWR_DN, CLUSTER_PWR_DN;
int MCU_DONE = FALSE;

#define MC_PMU_SET_INSTRUCTION(CMD, OP, CNT)
		((((unsigned int)OP) << 14) | (((unsigned int)CMD) << 8) | CNT)

struct nx_cpupmu_reg *g_cpupmu_reg =
	((struct nx_cpupmu_reg*)PHY_BASEADDR_CPUPMU0_MODULE);

static int mc_pmu_is_busy(void)
{
	unsigned int reg_val;

	reg_val = (mmio_read_32(&g_cpupmu_reg->pc_cur) & 0x1);

	if (reg_val)
		return FALSE;

	return TRUE;
}

static void mc_pmu_set_opr(cpu_pmu_instruction cmd, cpu_pmu_op op, unsigned int value)
{
	unsigned int opsel = ((((unsigned int)op) - 2) * 4);

	unsigned int cmd_num = (((unsigned int)cmd) >> 2);
	unsigned int cmd_sel = ((((unsigned int)cmd) & 0x3) * 8);
	unsigned int mask = (0xff << (opsel + cmd_sel));
	unsigned int reg_val, masked_regval;

	reg_val = mmio_read_32(&g_cpupmu_reg->opr[cmd_num]);
	masked_regval = (reg_val & ~(mask));

	mmio_write_32(&g_cpupmu_reg->opr[cmd_num],
		(masked_regval | (value << (opsel + cmd_sel))));
}

static void mc_pmu_set_mask_op(cpu_pmu_instruction cmd, unsigned int mask)
{
	unsigned int cmd_num = (((unsigned int)cmd) >> 3);
	unsigned int cmd_sel = (((unsigned int)cmd) & 0x7)*4;

	unsigned int mask_op   = 0xf << cmd_sel;
	unsigned int reg_val, masked_regval;

	reg_val = mmio_read_32(&g_cpupmu_reg->opmask);
	masked_regval = reg_val & ~(mask_op)

	mmio_write_32(&g_cpupmu_reg->opmask[cmd_num],
		(masked_regval | (mask << cmd_sel)));
}

static unsigned short mc_pmu_set_instruction(cpu_pmu_instruction cmd,
			cpu_pmu_op op, unsigned int cnt)
{
	return ((((unsigned int)op) << 14) | (((unsigned int)cmd) << 8) | cnt);
}

static void mc_pmu_pc_set_to_safe(void)
{
	unsigned int inst;
	unsigned short safe_inst;

	inst = mmio_read_32(&g_cpupmu_reg->program[MC_PMU_PROGRAM_REGION - 1]);
	safe_inst = MC_PMU_SET_INSTRUCTION(PMU_CMD_END, PMU_OP_0, 0x1);

	inst = inst & ((0xffff)) | (unsigned int)(safe_inst << 16);

	while(mc_pmu_is_busy());

	mmio_write_32(&g_cpupmu_reg->pc_move, (MC_PMU_PROGRAM_REGION * 2 ) - 1);
}

static int mc_pmu_download_program(unsigned short *src)
{
	unsigned int code_l, code_m, code;
	unsigned int i;

	for (i = 0; i < (MC_PMU_PROGRAM_REGION - 2); i++) {
		code = 0;
		if (!(i & 0x1)) {
			code_l = (unsigned int) src[i];
		} else {
			code_m = src[i];
			code = code_l | (code_m << 16);
			mmio_write_32(&g_cpupmu_reg->program[i >> 1], code);
		}
	}
}

void indiv_cpu_pwr_up_op_set (unsigned int cpu_field, cpu_pmu_op op)
{
	/* CPUPWRDOWN, RESET, CLAMP */
	unsigned int active_low_value = (~(cpu_field) & 0xf);
	unsigned int active_low_mask = cpu_field;
	unsigned int active_high_value = cpu_field;
	unsigned int active_high_mask = (~(cpu_field) & 0xf);
	unsigned int op_mask = active_high_mask;

	mc_pmu_set_opr(PMU_CMD_REG_CPUPWRDOWNPRE, op, active_low_value);
	mc_pmu_set_opr(PMU_CMD_REG_CPUPWRDOWNALL, op, active_low_value);
	mc_pmu_set_opr(PMU_CMD_REG_nCOREPORESET , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_nCORERESET   , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_nDBGRESET    , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_nETMRESET    , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_CLAMPCPUOUT  , op, active_low_value);

	mc_pmu_set_mask_op(PMU_CMD_REG_CPUPWRDOWNPRE, op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_CPUPWRDOWNALL, op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nCOREPORESET , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nCORERESET   , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nDBGRESET    , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nETMRESET    , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_CLAMPCPUOUT  , op_mask);
}

void indiv_cpu_pwr_down_op_set (unsigned int cpu_field, cpu_pmu_op op)
{
	/* STANDBYWFI, CLAMPCPUOUT, CPUPWRDOWN, RESET */
	unsigned int active_low_value = (~(cpu_field) & 0xf);
	unsigned int active_low_mask = cpu_field;
	unsigned int active_high_value = cpu_field;
	unsigned int active_high_mask = (~(cpu_field) & 0xf);
	unsigned int op_mask = active_high_mask;

	mc_pmu_set_opr(PMU_CMD_WAIT_STANDBYWFI  , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_CLAMPCPUOUT  , op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_CPUPWRDOWNPRE, op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_CPUPWRDOWNALL, op, active_high_value);
	mc_pmu_set_opr(PMU_CMD_REG_nCOREPORESET , op, active_low_value);
	mc_pmu_set_opr(PMU_CMD_REG_nCORERESET   , op, active_low_value);
	mc_pmu_set_opr(PMU_CMD_REG_nDBGRESET    , op, active_low_value);
	mc_pmu_set_opr(PMU_CMD_REG_nETMRESET    , op, active_low_value);

	mc_pmu_set_mask_op(PMU_CMD_WAIT_STANDBYWFI  , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_CLAMPCPUOUT  , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_CPUPWRDOWNPRE, op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_CPUPWRDOWNALL, op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nCOREPORESET , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nCORERESET   , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nDBGRESET    , op_mask);
	mc_pmu_set_mask_op(PMU_CMD_REG_nETMRESET    , op_mask);
}

// POWER ON: op 2
unsigned int cpu_power_up_sequence (unsigned short *dst, unsigned int index)
{
#if 1
	/* step xx. signal powerdup set to 1 */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP       , PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF       , PMU_OP_ALL_0, 8);

	/* step xx. pre charge, all charge */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNPRE, PMU_OP_2, 100);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNALL, PMU_OP_2, 255);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_NOP              , PMU_OP_2, 32);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN    , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_PWRDNACKN   , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF       , PMU_OP_ALL_1, 16);

	/* step xx. release the reset */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCORERESET   , PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCOREPORESET , PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nDBGRESET    , PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nETMRESET    , PMU_OP_2, 1);

	/* step xx. clamp release */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCPUOUT  , PMU_OP_2, 8);

	/* step xx. clock on */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF       , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN    , PMU_OP_ALL_1, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_END              , PMU_OP_ALL_0, 2);
#else	// Debug
	DBGOUT("PMU_CMD_REG_PWRDUP        : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP        , PMU_OP_ALL_1, 8) );
	DBGOUT("PMU_CMD_REG_CLKOFF        : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_0, 8) );
	DBGOUT("PMU_CMD_REG_CPUPWRDOWNPRE : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNPRE , PMU_OP_2, 100)   );
	DBGOUT("PMU_CMD_REG_CPUPWRDOWNALL : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNALL , PMU_OP_2, 255)   );
	DBGOUT("PMU_CMD_NOP               : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_NOP               , PMU_OP_2, 32)    );
	DBGOUT("PMU_CMD_REG_PWRDNREQN     : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_0, 16));
	DBGOUT("PMU_CMD_WAIT_PWRDNACKN    : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_PWRDNACKN    , PMU_OP_ALL_0, 16));
	DBGOUT("PMU_CMD_REG_CLKOFF        : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_1, 16));
	DBGOUT("PMU_CMD_REG_nCORERESET    : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCORERESET    , PMU_OP_2, 4)     );
	DBGOUT("PMU_CMD_REG_nCOREPORESET  : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCOREPORESET  , PMU_OP_2, 4)     );
	DBGOUT("PMU_CMD_REG_nDBGRESET     : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nDBGRESET     , PMU_OP_2, 4)     );
	DBGOUT("PMU_CMD_REG_nETMRESET     : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nETMRESET     , PMU_OP_2, 4)     );
	DBGOUT("PMU_CMD_REG_CLAMPCPUOUT  ,: %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCPUOUT   , PMU_OP_2, 8)     );
	DBGOUT("PMU_CMD_REG_CLKOFF        : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_0, 16));
	DBGOUT("PMU_CMD_REG_PWRDNREQN     : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_1, 16));
	DBGOUT("PMU_CMD_END               : %x\r\n", MC_PMU_SET_INSTRUCTION(PMU_CMD_END               , PMU_OP_ALL_0, 2 ));
#endif
	return index;
}

// POWER ON: op 2
unsigned int cluster_power_up_sequence (unsigned short *dst, unsigned int index)
{
	/* step xx. signal powerdup set to 1 */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP, PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_EFUSEDONE, PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF, PMU_OP_ALL_0, 8);

	/* step xx. cluster precharge, all charge */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_COREPWRDOWNPRE, PMU_OP_ALL_1, 100);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_COREPWRDOWNALL, PMU_OP_ALL_1, 255);

	/* step xx. pre charge, all charge */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNPRE, PMU_OP_2, 100);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNALL, PMU_OP_2, 255);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_NOP, PMU_OP_2, 32);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN, PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_PWRDNACKN, PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF, PMU_OP_ALL_1, 16);

	/* step xx. release the reset */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCORERESET, PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCOREPORESET, PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nDBGRESET, PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nETMRESET, PMU_OP_2, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nL2RESET, PMU_OP_ALL_1, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nMBISTRESET, PMU_OP_ALL_1, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nSOCDBGRESET, PMU_OP_ALL_1, 8);

	/* step xx. clamp release */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCOREOUT, PMU_OP_2, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCPUOUT, PMU_OP_2, 8);

	/* step xx. clock on */
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF, PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN, PMU_OP_ALL_1, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_END, PMU_OP_ALL_0, 2);

	return index;
}

// POWER DOWN: op 3
unsigned int cpu_power_down_sequence (unsigned short* dst, unsigned int index)
{
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP	 , PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_MASKIRQ	 , PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_STANDBYWFI	 , PMU_OP_3, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_NOP               , PMU_OP_3, 32);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_PWRDNACKN    , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_1, 16);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCPUOUT	 , PMU_OP_3,  8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNPRE , PMU_OP_3, 10);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNALL , PMU_OP_3, 10);

	//CPU ALL reset
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCORERESET    , PMU_OP_3, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCOREPORESET  , PMU_OP_3, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nDBGRESET     , PMU_OP_3, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nETMRESET     , PMU_OP_3, 1);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_MASKIRQ       , PMU_OP_ALL_0, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_1, 16);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_END               , PMU_OP_ALL_0, 2);

	return index;
}

// POWER DOWN: op 3
unsigned int cluster_down_sequence (unsigned short* dst, unsigned int index)
{
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP	, PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_MASKIRQ	, PMU_OP_ALL_1, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_STANDBYWFI	, PMU_OP_ALL_1, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_ACINACTM	, PMU_OP_ALL_1, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_STANDBYWFIL2 , PMU_OP_ALL_1, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_NOP               , PMU_OP_3, 32);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_WAIT_PWRDNACKN    , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_1, 16);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCPUOUT   , PMU_OP_ALL_1,  8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLAMPCOREOUT  , PMU_OP_ALL_1,  8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_DFTRAMHOLD    , PMU_OP_ALL_1, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNPRE , PMU_OP_ALL_1, 24);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CPUPWRDOWNALL , PMU_OP_ALL_1, 24);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_COREPWRDOWNPRE, PMU_OP_ALL_1, 24);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_COREPWRDOWNALL, PMU_OP_ALL_1, 24);

	//CPU ALL reset
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCORERESET    , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nCOREPORESET  , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nDBGRESET     , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nETMRESET     , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nL2RESET      , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nMBISTRESET   , PMU_OP_ALL_0, 1);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_nSOCDBGRESET  , PMU_OP_ALL_0, 8);

	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_CLKOFF        , PMU_OP_ALL_0, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_MASKIRQ       , PMU_OP_ALL_0, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDNREQN     , PMU_OP_ALL_1, 16);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_REG_PWRDUP        , PMU_OP_ALL_0, 8);
	dst[index++] = MC_PMU_SET_INSTRUCTION(PMU_CMD_END               , PMU_OP_ALL_0, 2);

	return index;
}
