/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#ifndef __PRINTF__
#define __PRINTF__

#include <stdarg.h>

#define getc		serial_getc
#define putc		serial_putc

#define serial_done	serial_is_uart_tx_done
#define serial_busy	serial_is_busy
#define serial_tx_empty	serial_is_tx_empty

int printf(const char *format, ...);
int empty_printf(const char *format, ...);

#endif	/* #ifndef __PRINTF__ */
