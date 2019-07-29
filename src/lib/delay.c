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
#include <delay.h>
#include <armv7_pmu.h>

#if 0
void udelay(unsigned int us)
{
#if 1
	pmu_delay_us(us);
#else
	volatile unsigned int i, j;

	for (i = 0; i < 0x1000; i++)
		for (j = 0; j < 0x100; j++);
#endif
}
#else
#define CNTSPEEDUS      24      /* input clock : 24MHz, 24 clock / us */
void udelay(unsigned int utime)
{
	register unsigned long long ccnt, tcnt = utime * CNTSPEEDUS;
	unsigned int lsb, msb;
	unsigned int lsb1, msb1;
	unsigned int lsb2, msb2;
	__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb), "=r"(msb));
	__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb1), "=r"(msb1));
	__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb2), "=r"(msb2));
	lsb2 = lsb ^ lsb1 ^ lsb2;
	if (lsb2 == lsb) {
		lsb = lsb1;
		msb = msb1;
	}
	ccnt = msb;
	ccnt = (ccnt << 32) | lsb;

	tcnt += ccnt;
	do {
		__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb), "=r"(msb));
		__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb1), "=r"(msb1));
		__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14": "=r"(lsb2), "=r"(msb2));
		lsb2 = lsb ^ lsb1 ^ lsb2;
		if (lsb2 == lsb) {
			lsb = lsb1;
			msb = msb1;
		}
		ccnt = msb;
		ccnt = (ccnt << 32) | lsb;
	} while (tcnt > ccnt);
}
#endif

void mdelay(unsigned int ms)
{
	udelay(ms * 1000);
}

void ldelay(volatile unsigned int delay)
{
	volatile unsigned int i, j;

	for (i = 0; i < 1000; i++)
		for (j = 0; j < delay *10; j++);
}
