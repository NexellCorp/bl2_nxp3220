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
#include <type.h>
#include <armv7_pmu.h>
#include <pll.h>

#include <log.h>

/* cpu one cycle -> period (ps) */
int g_cycle_period_ps;

unsigned int pmu_get_count (void)
{
	unsigned int value;

	/* Reead CCNT Register (PMCCNTR,) */
	__asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));

	return value;
}

static inline void pmu_counter_initialize (int do_reset, int enable_divider)
{
	/* in general enable all counters (including cycle counter) */
	int value = 1;

	/* Perform Reset */
	if (do_reset) {
		value |= 2;     // reset all counters to zero.
		value |= 4;     // reset cycle counter to zero.
	}

	if (enable_divider)
		value |= 8;     // enable "by 64" divider for CCNT.
	value |= 16;

	/* program the performance-counter control-register: (PMCR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));

	/* enable user-mode access to the performance counter  (PMUSERENR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

	/* enable all counters: (PMCNTENSET) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));

	/* clear overflows: (PMOVSR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));

	/* disable counter overflow interrupts (just in case) (PMINTENCLR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));
}

void pmu_counter_deinitialize (int do_reset, int enable_divider)
{
	/* in general enable all counters (including cycle counter) */
	int value = 1;

	/* Perform Reset */
	if (do_reset) {
		value |= 2;     // reset all counters to zero.
		value |= 4;     // reset cycle counter to zero.
	}

	if (enable_divider)
		value |= 8;     // enable "by 64" divider for CCNT.
	value |= 16;

	/* program the performance-counter control-register: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(0));

	/* enable user-mode access to the performance counter*/
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

	/* enable all counters: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));

	/* clear overflows: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));

	/* disable counter overflow interrupts (just in case)*/
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));
}

void pmu_delay_us(volatile unsigned int us)
{
	volatile double overhead = pmu_get_count();
	volatile double pmu_us = 0;
	volatile double cycle, count;

	do {
		count = pmu_get_count();
		cycle = (count- overhead);
		if (cycle >= 1000000) {
			pmu_us += (g_cycle_period_ps * cycle/1000)/1000;
			overhead = count;
		}
	} while (pmu_us < us);

#if DEBUG_CONSOLE
	printf("pmu_ps: %d, us: %d, cycle: %d \r\n", pmu_us, us, cycle);
#endif
}

/* ArmV7 Archtecture */
void armv7_pmu_init(void)
{
	/* step xx. Calcurate the CPU One Cycle -> Period(ps)  */
	g_cycle_period_ps = (1000000/(get_pll_freq(PLLCPU)/1000000));

	/* step xx. performance management unit initialize */
	pmu_counter_initialize(1, 0);

	return;
}
