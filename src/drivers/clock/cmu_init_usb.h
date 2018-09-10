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
#ifndef __CMU_INIT_USB_H__
#define __CMU_INIT_USB_H__

struct nx_clk_priv cmu_usb[] = {
	CMU_INIT(USB_0_AHB_CLK                     , 0, CMU_TYPE_MAINDIV , (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 0, SRC_USB_0_AHB_CLK, 0),
	CMU_INIT(SYSREG_USB_0_APB_CLK              , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 1, USB_0_AHB_CLK, 0),
	CMU_INIT(USB20OTG_0_AHB_CLK                , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 2, USB_0_AHB_CLK, 0),
	CMU_INIT(USB20HOST_0_AHB_CLK               , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 3, USB_0_AHB_CLK, 0),
};

#endif /* #ifndef __CMU_INIT_USB_H__ */
