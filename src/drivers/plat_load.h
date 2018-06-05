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
#ifndef __PLAT_LOAD_H__
#define __PLAT_LOAD_H__

struct platform_info {
	unsigned int is_sss_f;
	unsigned int s_dev_addr;
	unsigned int n_dev_addr;
	void (*esr_func)(void);
};

/* Function Define */
 int plat_load(void);

#endif /* #ifndef __PLAT_LOAD_H__ */
