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
