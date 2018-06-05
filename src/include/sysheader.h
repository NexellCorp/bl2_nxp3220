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
#ifndef __SYSHEADER_H__
#define __SYSHEADER_H__

#include <type.h>
#include <chip.h>
#include <libnx.h>
#include <libarmv7.h>
#include <armv7_pmu.h>
#include <delay.h>
#include <log.h>

#include <bootheader.h>

#ifdef CHIPID_NXP3220
#include <nxp3220.h>
#include <sip_s31nx_io_mux.h>
#endif

#ifdef SET_GLOBAL_VARIABLES

struct sbi_header *g_nsih
	= ((struct sbi_header *const)RE_HEADER_BASEADDR);
#else

extern struct sbi_header *g_nsih;
#endif

#endif // #ifndef __SYSHEADER_H__
