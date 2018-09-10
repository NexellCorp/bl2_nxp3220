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
#ifndef __ARMV7_PMU_H__
#define __ARMV7_PMU_H__

/* Function Define */
void armv7_pmu_init(void);
void pmu_delay_us(volatile unsigned int us);

#endif	/* __ARMV7_PMU_H__ */
