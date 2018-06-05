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
#ifndef __LOG_H__
#define __LOG_H__

#include <printf.h>

// User Debug Message
#if 1
#define SYSMSG  printf
#else
#define SYSMSG	empty_printf
#endif

// User Debug Message
#if 0
#define DBGOUT  printf
#else
#define DBGOUT	empty_printf
#endif

// User Panic Message
#if 1
#define PANIC(...)	printf("PANIC:  " __VA_ARGS__); while(1);
#else
#define PANIC(...)	while(1);
#endif


#define LOG_LEVEL			50

#define LOG_LEVEL_NONE			0
#define LOG_LEVEL_ERROR			10
#define LOG_LEVEL_NOTICE		20
#define LOG_LEVEL_WARNING		30
#define LOG_LEVEL_INFO			40
#define LOG_LEVEL_VERBOSE		50

#if ((LOG_LEVEL >= LOG_LEVEL_NOTICE) && defined(SYSLOG_ON))
#define NOTICE(...)	printf("NOTICE:  " __VA_ARGS__)
#else
#define NOTICE(...)	empty_printf("NOTICE:  " __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_ERROR) && defined(SYSLOG_ON))
#define ERROR(...)	printf("ERROR:   " __VA_ARGS__)
#else
#define ERROR(...)	empty_printf("ERROR:   " __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_WARNING) && defined(SYSLOG_ON))
#define WARN(...)	printf("WARNING: " __VA_ARGS__)
#else
#define WARN(...)	empty_printf("WARNING: " __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_INFO) && defined(SYSLOG_ON))
#define INFO(...)	printf("INFO:    " __VA_ARGS__)
#else
#define INFO(...)	empty_printf("INFO:    " __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_VERBOSE) && defined(SYSLOG_ON))
#define VERBOSE(...)	printf("VERBOSE: " __VA_ARGS__)
#else
#define VERBOSE(...)	empty_printf("VERBOSE: " __VA_ARGS__)
#endif

#define ASSERT(expr)

#endif // #ifndef __LOG_H__
