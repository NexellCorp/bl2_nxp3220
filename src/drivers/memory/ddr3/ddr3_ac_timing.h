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
#ifndef __DDR3_AC_TIMING_H__
#define __DDR3_AC_TIMING_H__

/* Support the DDR3 Clock*/
#define DDR3_MEMCLK_533Mhz	(533)	// 0
#define DDR3_MEMCLK_666Mhz	(666)	// 1
#define DDR3_MEMCLK_800Mhz	(800)	// 3

/* Support the DDR3 Memory Size */
#define DDR3_MEMSIZE_1GB	(0x40000000)
#define DDR3_MEMSIZE_512MB	(0x20000000)
#define DDR3_MEMSIZE_256MB	(0x10000000)

/* User Define - DDR3 Device Specifiacation */
#define DDR3_MEMORY_CLOCK	800//CONFIG_DDR3_MEMCLK

/* Refer to Memory(DREX, DPHY) Datasheet - Register Set*/
#define DDR3_CS_PERSIZE		(CONFIG_DDR3_CS_PERSIZE >> 24)
#define DDR3_CS_MEMMASK		(0x800 - DDR3_CS_PERSIZE)			// Capacity per nCS: 2G=0x780, 1G=0x7C0(Tabletx2, VTK)
#define DDR3_TOTAL_MEMSIZE	(CONFIG_DDR3_MEMSIZE)				// Total Byte Size

#define DDR3_CS_NUM		(CONFIG_DDR3_CS_NUM)
#define DDR3_BUS_WIDTH		(CONFIG_DDR3_BUS_WIDTH)
#define DDR3_ROW_NUM		(CONFIG_DDR3_ROW_NUM - 12)			// ROW address bit : 15bit : 3(Tabletx2), 16bit : 4(Tabletx4, Elcomtech)
#define DDR3_COL_NUM		(CONFIG_DDR3_COLUMN_NUM -  7)			// Column Address Bit. 2:9bit, 3:10bit, others:Reserved
#define DDR3_BANK_NUM		(CONFIG_DDR3_BANK_NUM)				// Bank bit : 3:8bank, 2:4bank
#define DDR3_CS0_BASEADDR	(0x0)						// S5P6818: AXI Bus Adress Confirm
#define DDR3_CS1_BASEADDR	(DDR3_CS0_BASEADDR + DDR3_CS_PERSIZE)

#define RDFETCH			0x1						// CONCONTROL.rd_fetch[14:12]

/* DDR3 AC Timing */
#if 0
#if (DDR3_MEMORY_CLOCK == DDR3_MEMCLK_800Mhz)
#define AL			0
#define WL			6
#define RL			8

#define nCWL			WL + AL					// CAS Write Latency(CWL).
#define nCL			RL + AL					// CAS Latency(CL). Sometimes plus is needed.

#define MR0_nWR 		0x6

#define tMOD			1
#define tXPR			1
#define tZQINIT			256		//512

#define tCCD			2		// 4
#define tZQoper			128 		// 256
#define tCKESR			3 // 2		// 4
#define tZQCS			32		// 64
#define tXS			7		// 14

#define tPZQ			0x401B

#define tREFIPB			0x62
#define tREFI			0xC30

#define tRRD			0x4// 0x3
#define tRP			0x6
#define tRCD			0x6
#define tRC			0x14
#define tRAS			0xE

#define tWTR			0x3
#define tWR			0x6
#define tRTP			0x3
#define tPPD			0x0
#define W2W_C2C			0x1
#define R2R_C2C			0x1
#define tDQSCK			0x0

#define tFAW			0xE // 0x10
#define tXSR			0x100
#define tXP			0x3 // 0x2		// 0x3
#define tCKE			0x2
#define tMRD			0x2		//0x4

#define tADR			0x3						// Samsung (15ns)
#define tWLO			(tADR)

#define WR2PRECH		0xC // 24 - round_up(WL + 4 + tWR)
#define WR2RD			0x9 // 18 - round_up(WL + 4+ tWTR)
#define WR2RD_CSC		0x9 // 18 - max(WL + BL + round_up(tWTR) - 1), 3)

#endif

/* Timing parameters that depend on memory size.*/
#define tREFI			0xC30

#define tRFC		(0x68)			// 512MB, 800Mhz

#define tXPR_MAX	(tRFC+10)		// 1Gb:120ns, 2Gb:170ns, 4Gb:270ns, 8Gb:360ns + 10 ns / tCK_HALF_ns =

#endif

#endif  //#ifndef __DDR3_AC_TIMING_H__
