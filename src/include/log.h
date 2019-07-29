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
#define PANIC(...)	printf("PANIC:\t" __VA_ARGS__); while(1);
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
#define NOTICE(...)	printf("NOTICE:\t" __VA_ARGS__)
#else
#define NOTICE(...)	empty_printf("NOTICE:\t" __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_ERROR) && defined(SYSLOG_ON))
#define ERROR(...)	printf("ERROR:\t" __VA_ARGS__)
#else
#define ERROR(...)	empty_printf("ERROR:\t" __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_WARNING) && defined(SYSLOG_ON))
#define WARN(...)	printf("WARNING:\t" __VA_ARGS__)
#else
#define WARN(...)	empty_printf("WARNING:\t" __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_INFO) && defined(SYSLOG_ON))
#define INFO(...)	printf("INFO:\t" __VA_ARGS__)
#else
#define INFO(...)	empty_printf("INFO:\t" __VA_ARGS__)
#endif

#if ((LOG_LEVEL >= LOG_LEVEL_VERBOSE) && defined(SYSLOG_ON))
#define VERBOSE(...)	printf("VERBOSE:\t" __VA_ARGS__)
#else
#define VERBOSE(...)	empty_printf("VERBOSE:\t" __VA_ARGS__)
#endif

#define ASSERT(expr)

#endif // #ifndef __LOG_H__
