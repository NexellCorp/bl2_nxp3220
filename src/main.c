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
#define SET_GLOBAL_VARIABLES

#include <sysheader.h>
#include <clock.h>
#include <serial.h>
#include <pmic.h>
#include <pmu.h>
#include <memory.h>
#include <tz.h>
#include <plat_load.h>
#include <main.h>

#include <memtester.h>

void main(void)
{
	int serial_ch = g_nsih->serial_ch;
	int is_resume = 0;

#ifdef EARLY_SERIAL
	serial_init(serial_ch);
#endif
	pmic_initialize();

	pmu_initialize();

	tzpc_set_ddr();

	clock_initialize();

	serial_init(serial_ch);

	memory_initialize(is_resume);

	clock_information();

#if defined(SIMPLE_MEMTEST)
	simple_memtest();
#elif defined(STANDARD_MEMTEST)
	standard_memtester()
#endif
	plat_load();
}
