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
#if defined(DDR3)
#include <ddr3_sdram.h>
#elif defined(DDR4)
#include <ddr4_sdram.h>
#endif
#include <memory.h>
#include <memtester.h>

/* Extern Function */
extern int ddr3_initialize (unsigned int is_resume);
extern int ddr4_initialize (unsigned int is_resume);

struct dram_device_info g_ddr_info;

static void get_dram_information(struct dram_device_info *me)
{
	int byte = 8;

	/* Nexell Step XX. Memory Address (for Write Training (DRAM)) */
	me->bank_num	= g_nsih->dii.bank_num;
	me->row_num	= g_nsih->dii.row_num;
	me->column_num	= g_nsih->dii.col_num;

	me->column_size	= (1 << me->column_num)/byte;				// Unit : Byte
	me->row_size	= (1 << me->row_num);
	me->bank_size	= (me->row_size * me->column_size);
	me->chip_size	= (((me->bank_size * (1 << me->bank_num))
				* g_nsih->dii.bus_width)/ 1024 / 1024);		// Unit: MB
	me->sdram_size	= (me->chip_size * (g_nsih->dii.chip_num
				* 32 / g_nsih->dii.bus_width));
#if 0
	SYSMSG("############## [SDRAM] Memory Specification ###############\r\n");
	SYSMSG("[Bit] Bank Address   : %d \r\n", me->bank_num);
	SYSMSG("[Bit] Column Address : %d \r\n", me->column_num);
	SYSMSG("[Bit] Row Address    : %d \r\n", me->row_num);
	SYSMSG("[Bit] Data Line      : %d \r\n", g_nsih->dii.bus_width);
	SYSMSG("[BYTE] Column    Size: %d \r\n", me->column_size);
	SYSMSG("[BYTE] Row(Page) Size: %d \r\n", me->row_size);
	SYSMSG("[BYTE] Bank      Size: %d \r\n", me->bank_size);
#if 1
	SYSMSG("[MB]   Chip      Size: %d \r\n", me->chip_size);
	SYSMSG("[MB]   SDRAM     Size: %d \r\n", me->sdram_size);
#endif
	SYSMSG("############################################################\r\n");
#endif
}

int memory_initialize(int is_resume)
{
	int ret = 0;

	/* step xx. memory initialize  */
#if defined(DDR3)
	ret = ddr3_initialize(is_resume);
#elif defined(DDR4)
	ret = ddr4_initialize(is_resume);
#endif

	NOTICE("Memory Initialize %s! (%d:%d) \r\n\n",
			(ret >= 0) ? "Done" : "Failed", ret, is_resume);

	/* @brief: auto write-leveling  */
	if ((g_nsih->cal_mode >> 2) & 0x1)
		hw_write_leveling();

	/* @brief: auto bit-leveling */
	if ((g_nsih->cal_mode >> 0) & 0x1)
		hw_bit_leveling();

	/* @brief: "libddr.a" in manual bit-leveling */
	if ((g_nsih->cal_mode >> 1) & 0x1) {
		trimtest(0x40000000,
				(1 << 0) |					/* Bit Cal state	*/
				(1 << 1) |					/* Center, Margin value */
				(1 << 2) |					/* Lock Value		*/
				(1 << 3) |					/* Read Cal enable	*/
				(1 << 4));					/* Write Cal enable	*/
	}

	if (ret >= 0)
		return -1;

	get_dram_information(&g_ddr_info);

#if defined(SIMPLE_MEMTEST)
	simple_memtest();
#elif defined(STANDARD_MEMTEST)
	standard_memtester();
#endif
	return ret;
}
