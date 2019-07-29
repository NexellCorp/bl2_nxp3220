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
#include <type.h>
#include <armv7_pmu.h>
#include <pll.h>

#include <log.h>

/* cpu one cycle -> period (ps) */
#if 0
int g_cycle_period_ps;

unsigned int pmu_get_count(void)
{
	unsigned int value;

	/* Reead CCNT Register (PMCCNTR,) */
	__asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\r\n": "=r"(value));

	return value;
}

static inline void pmu_counter_initialize (int do_reset, int enable_divider)
{
	/* in general enable all counters (including cycle counter) */
	int value = 1;

	/* Perform Reset */
	if (do_reset) {
		value |= 1 << 1;	// reset all counters to zero.
		value |= 1 << 2;	// reset cycle counter to zero.
	}

	if (enable_divider)
		value |= 1 << 3;	// enable "by 64" divider for CCNT.
	value |= 1 << 4;

	/* program the performance-counter control-register: (PMCR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 0\t\r\n" :: "r"(value));

	/* enable user-mode access to the performance counter  (PMUSERENR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 0\r\n\t" :: "r"(1));

	/* enable all counters: (PMCNTENSET) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 1\t\r\n" :: "r"(0x8000000f));

	/* clear overflows: (PMOVSR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 3\t\r\n" :: "r"(0x8000000f));

	/* disable counter overflow interrupts (just in case) (PMINTENCLR) */
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 2\r\n\t" :: "r"(0x8000000f));
}

void pmu_counter_deinitialize (int do_reset, int enable_divider)
{
	/* in general enable all counters (including cycle counter) */
	int value = 1;

	/* Perform Reset */
	if (do_reset) {
		value |= 1 << 1;	// reset all counters to zero.
		value |= 1 << 2;	// reset cycle counter to zero.
	}

	if (enable_divider)
		value |= 1 << 3;	// enable "by 64" divider for CCNT.
	value |= 1 << 4;

	/* program the performance-counter control-register: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 0\t\r\n" :: "r"(0));

	/* enable user-mode access to the performance counter*/
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 0\r\n\t" :: "r"(1));

	/* enable all counters: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 1\t\r\n" :: "r"(0x8000000f));

	/* clear overflows: */
	__asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 3\t\r\n" :: "r"(0x8000000f));

	/* disable counter overflow interrupts (just in case)*/
	__asm__ __volatile__ ("MCR p15, 0, %0, C9, C14, 2\r\n\t" :: "r"(0x8000000f));
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
			pmu_us += (g_cycle_period_ps * cycle / 1000) / 1000;
			overhead = count;
		}
	} while (pmu_us < us);

#if DEBUG_CONSOLE
	printf("pmu_ps: %d, us: %d, cycle: %d \r\r\n", pmu_us, us, cycle);
#endif
}

/* ArmV7 Archtecture */
void armv7_pmu_init(void)
{
	/* step xx. Calcurate the CPU One Cycle -> Period(ps)  */
	g_cycle_period_ps = (1000000 / (get_pll_freq(PLLCPU) / 1000000));

	/* step xx. performance management unit initialize */
	pmu_counter_initialize(1, 0);

	return;
}
#endif
