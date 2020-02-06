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

#if 0
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
#endif

void trimset(char *, char *);
int checkcaldata(char *, char *);
int trimtest(unsigned int, unsigned int);

int memory_calibration(unsigned int is_resume)
{
	int ret = 0;

	/* @brief: auto write-leveling  */
	if (g_nsih->cal_mode & 1 << 2)
		hw_write_leveling();

	/* @brief: auto bit-leveling */
	if (g_nsih->cal_mode & 1 << 0)
		hw_bit_leveling();

	/* @brief: "libddr.a" in manual bit-leveling */
	if (g_nsih->cal_mode & 1 << 1) {
		if (checkcaldata(g_nsih->readcal, g_nsih->writecal)
			&& (is_resume == FALSE)) {
			trimset(g_nsih->readcal, g_nsih->writecal);
		} else {
			ret = trimtest(0x40000000,
				(1 << 0) |	/* Bit Cal state	*/
				(1 << 1) |	/* Center, Margin value */
				(1 << 2) |	/* Lock Value		*/
				(1 << 3) |	/* Read Cal enable	*/
				(1 << 4) |	/* Write Cal enable	*/
				(0 << 5));	/* r/w impedence test	*/
		}
	}

	return ret;
}

int memory_initialize(unsigned int is_resume)
{
	int ret = 0;
	int i = 0;

	/* step xx. memory initialize  */
#if defined(DDR3)
	ret = ddr3_initialize(is_resume);
#elif defined(DDR4)
	ret = ddr4_initialize(is_resume);
#endif

	NOTICE("Memory Initialize %s! (%d:%d) \r\n",
			(ret >= 0) ? "Done" : "Failed", ret, is_resume);

	ret = memory_calibration(is_resume);

#ifdef DDR_TEST_MODE
	get_read_test(0x40000000);
#endif

	if (ret < 0)
		return -1;

#if defined(SIMPLE_MEMTEST)
	simple_memtest();
#elif defined(STANDARD_MEMTEST)
	standard_memtester();
#endif
	NOTICE("Memory Initialize Done. ret=%d\r\n", ret);
	return ret;
}
