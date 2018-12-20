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
#ifndef __DDR4_AC_TIMING_H__
#define __DDR4_AC_TIMING_H__

#define DDR4_BURST_LENGTH	1

#define DDR4_MEMCLK_800Mhz	0

#define DDR4_MEMORY_CLOCK	DDR4_MEMCLK_800Mhz

/* DDR4 - Access Timing */
#if 0
#if (DDR4_MEMORY_CLOCK == DDR4_MEMCLK_800Mhz)
#define tCCD_S					4 				// nCK
#define tCCD_L					(5/2)				// nCK

#define tXPR					1
#define tMRD					1
#define tMOD					1

#define tRP					10				// 12.5 ns
#define tRTP					4				// 7.5 ns
#define tRRD_S					4				// 5ns
#define tRCD					10				// 12.5 ns

#define tRFC					208				// 4Gb - 260ns

#define tRRD_L					4				// 2K - 6ns, 1K - 7.5ns

#define tREFI					0x618
#define tFAW					0x28				// 2K - 35ns, 1K-25ns

#define tRAS					28				// 35ns
#define tRC					38				// 47.5 ns

#define tXP					5				// 6ns

#define tWR					12				// 15ns

#define tWTR_S					2/2				// 2nCK, 2.5ns

#define tWTR_L					2/2				// 2nCK, 2.5ns

// ZQ Calibration Timing
#define tZQoper					512				// nCK
#define tZQCS					128				// nCK

#define tXS					(tRFC + 8)			// tRFC + 10ns


#define tCKE
#define tCKESR					3

#define CWL					1
#define tZQINIT					1

#define CL					1
#define AL					1

#define AUTO_SELF_REFRESH			1

#define DDR4_2_CYCLE_PREAMBLE			1
#define DDR4_TEMP_CTRL_REF			1
#define DDR4_TEMP_CTRL_REF_RANGE		1
#define DDR4_MAX_PD				1
#endif

#else

/* Timing parameters that depend on memory size.*/
#define tREFI			0xC30

#define tRFC1			(0x68)			// 512MB, 800Mhz

#define tXPR_MAX		(tRFC+10)		// 1Gb:120ns, 2Gb:170ns, 4Gb:270ns, 8Gb:360ns + 10 ns / tCK_HALF_ns =


#endif

#endif // __DDR4_AC_TIMING_H__
