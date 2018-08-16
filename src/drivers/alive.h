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
#ifndef __ALIVE_H__
#define __ALIVE_H__

/* Alive Scratch Register 0 ~ 7 */
struct nx_alive_scratch_reg {
	volatile unsigned int reserved[2];					// 0xAC + (0xXX ~ 0xXX) : reservedx
	volatile unsigned int scratch_value;					// 0xB4 + 0xXX : Scrach Registerx
};

/* Alive(->Normal) GPIOA ~ GPIOH Control Register */
struct nx_alive_gpio_reg {
	volatile unsigned int gpio_pwrdn;					// 0xXXX : Alive GPIOx Power Down Register
	volatile unsigned int gpio_outenb;					// 0xXXX : Alive GPIOx Output Enable
	volatile unsigned int gpio_out;						// 0xXXX : Alive GPIOx Output
};

/* Alive GPIO Related */
struct nx_alive_reg {
	volatile unsigned int reserved0;					// 0x00 : Reserved 0
	volatile unsigned int gpio_async_detect_mode_rst0;			// 0x04 : Alive GPIO ASync Detect Mode Reset Register0
	volatile unsigned int gpio_async_detect_mode_set0;			// 0x08 : Alive GPIO ASync Detect Mode Set Register0
	volatile unsigned int gpio_low_async_detect_mode_read;			// 0x0C : Alive GPIO Low ASync Detect Mode Read Register

	volatile unsigned int gpio_async_detect_mode_rst1;			// 0x10 : Alive GPIO ASync Detect Mode Reset Register1
	volatile unsigned int gpio_async_detect_mode_set1;			// 0x14 : Alive GPIO ASync Detect Mode Set Register1
	volatile unsigned int gpio_high_async_detect_mode_read;			// 0x18 : Alive GPIO High ASync Detect Mode Read Register

	volatile unsigned int gpio_detectmode_rst0;				// 0x1C : Alive GPIO Detect Mode Reset Register0
	volatile unsigned int gpio_detectmode_set0;				// 0x20 : Alive GPIO Detect Mode Reset Register0
	volatile unsigned int gpio_falldetect_mode_read;			// 0x24 : Alive GPIO Falling Edge Detect Mode Read Register

	volatile unsigned int gpio_detectmode_rst1;				// 0x28 : Alive GPIO Detect Mode Reset Register1
	volatile unsigned int gpio_detectmode_set1;				// 0x2C : Alive GPIO Detect Mode Reset Register1
	volatile unsigned int gpio_rise_detectmode_read;			// 0x30 : Alive GPIO Rising Edge Detect Mode Read Register

	volatile unsigned int gpio_detectmode_rst2;				// 0x34 : Alive GPIO Detect Mode Reset Register2
	volatile unsigned int gpio_detectmode_set2;				// 0x38 : Alive GPIO Detect Mode Reset Register2
	volatile unsigned int gpio_low_detectmode_read;				// 0x3C : Alive GPIO Low Level Detect Mode Read Register

	volatile unsigned int gpio_detectmode_rst3;				// 0x40 : Alive GPIO Detect Mode Reset Register3
	volatile unsigned int gpio_detectmode_set3;				// 0x44 : Alive GPIO Detect Mode Reset Register3
	volatile unsigned int gpio_high_detectmode_read;			// 0x48 : Alive GPIO High Level Detect Mode Read Register

	volatile unsigned int gpio_detectenb_rst;				// 0x4C : Alive GPIO Detect Enable Reset Register
	volatile unsigned int gpio_detectenb_set;				// 0x50 : Alive GPIO Detect Enable Set Register
	volatile unsigned int gpio_detectenb_read; 				// 0x54 : Alive GPIO Detect Enable Read Register

	volatile unsigned int gpio_intenb_rst;					// 0x58 : Alive GPIO Interrupt Enable Reset Register
	volatile unsigned int gpio_intenb_set;					// 0x5C : Alive GPIO Interrupt Enable Set Register
	volatile unsigned int gpio_intenb_read;					// 0x60 : Alive GPIO Interrupt Enable Read Register

	volatile unsigned int gpio_detect_pend;					// 0x64 : Alive GPIO Detect Pending Register

	volatile unsigned int reserved1[2];					// 0x68 ~ 0x6C : reserved1
	volatile unsigned int scratch;						// 0x70 : Alive Scratch Read Register

	volatile unsigned int gpio_pad_outenb_rst;				// 0x74 : Alive GPIO PAD Out Enable Reset Register
	volatile unsigned int gpio_pad_outenb_set;				// 0x78 : Alive GPIO PAD Out Enable Set Register
	volatile unsigned int gpio_pad_outenb_read; 				// 0x7C : Alive GPIO PAD Out Enable Read Register

	volatile unsigned int gpio_pullupenb_rst;				// 0x80 : Alive GPIO PAD Pullup Reset Register
	volatile unsigned int gpio_pullupenb_set;				// 0x84 : Alive GPIO PAD Pullup Set Register
	volatile unsigned int gpio_pullupenb_read; 				// 0x88 : Alive GPIO PAD Pullup Read Register

	volatile unsigned int gpio_padout_rst;					// 0x8C : Alive GPIO PAD Out Reset Register
	volatile unsigned int gpio_padout_set;					// 0x90 : Alive GPIO PAD Out Set Register
	volatile unsigned int gpio_padout_read;					// 0x94 : Alive GPIO PAD Out Read Register

	volatile unsigned int reserved2[2];					// 0x98 ~ 0x9C : reserved2
	volatile unsigned int vddctrl_read;					// 0xA0 : VDD Control Read Register

	volatile unsigned int clear_wakeup_status;				// 0xA4
	volatile unsigned int wake_upstatus;					// 0xA8

	struct nx_alive_scratch_reg sch[8];					// 0xAC ~ 0x108 : Alive Scratch Register

	volatile unsigned int reserved3[2];					// 0x10C ~ 0x110: reserved3
	volatile unsigned int vddoffcnt_value;					// 0x114 : Vdd Off Delay Value

	volatile unsigned int reserved4;					// 0x118 : reserved4
	volatile unsigned int gpio_inputvalue;					// 0x11C : Alive GPIO Input Value
	volatile unsigned int reserved5[2];					// 0x120 ~ 0x124 : reserved5

	volatile unsigned int n_pad_hold_enb;					// 0x128 : Alive N PAD HOLD Enable Reigster
	volatile unsigned int reserved6[2];					// 0x12C ~ 0x130 : reserved6
	volatile unsigned int n_pad_hold;					// 0x134 :

	volatile unsigned int gpio_pullsel_rst;					// 0x138 : Alive Pull-Sel Reset Register
	volatile unsigned int gpio_pullsel_set;					// 0x13C : Alive Pull-Sel Set Register
	volatile unsigned int gpio_pullsel_read;				// 0x140 : Alive Pull-Sel Read Register

	volatile unsigned int gpio_drv0_rst;					// 0x144 : Alive Drive Strength0 Reset Register
	volatile unsigned int gpio_drv0_set;					// 0x148 : Alive Drive Strength0 Set Register
	volatile unsigned int gpio_drv0_read;					// 0x14C : Alive Drive Strength0 Read Register

	volatile unsigned int gpio_drv1_rst;					// 0x150 : Alive Drive Strength1 Reset Register
	volatile unsigned int gpio_drv1_set;					// 0x154 : Alive Drive Strength1 Set Register
	volatile unsigned int gpio_drv1_read;					// 0x158 : Alive Drive Strength1 Read Register

	volatile unsigned int gpio_inputenb_rst;				// 0x15C : Alive Input Enable Reset Register
	volatile unsigned int gpio_inputenb_set;				// 0x160 : Alive Input Enable Set Register
	volatile unsigned int gpio_inputenb_read;				// 0x164 : Alive Input Enable Read Register

	volatile unsigned int reserved7[38];					// 0x168 ~ 0x1FC : reserved7

	/* Alive(->Normal) GPIOA ~ GPIOH Control Register */
	struct nx_alive_gpio_reg gpio[8];					// 0x200 ~ 0x25C :
};

/* VDD Power */
struct nx_vddpwr_reg {
	volatile unsigned int vddoff_delay_for_wakeup_mask;			// 0x000: VDD Off Delay for Wakeup Masking Register
	volatile unsigned int vddoff_start;					// 0x004: VDD Off Starg Register
	volatile unsigned int vddpwr_on_xti;					// 0x008: VDD Power On XTI Register
	volatile unsigned int reserved0;					// 0x00C: Reserved
	volatile unsigned int alive_gpio_altfn_sel_low;				// 0x010: Alive GPIO Alternate Function Select Low Register
	volatile unsigned int alive_sw_reset;					// 0x014: Alive Software Reset Register
	volatile unsigned int alive_sw_reset_enb;				// 0x018: Alive S/W Reset Enable Register
	volatile unsigned int alive_reset_status;				// 0x01C: Alive Reset Status Register
	volatile unsigned int vddpwron_count;					// 0x020: Alive VDD On Count
	volatile unsigned int reset_count;					// 0x024: Reset Count
	volatile unsigned int alive_gpio_altfn_sel_high;			// 0x028: Alive GPIO Alternate Function
	volatile unsigned int disable_cpu_wfi;					// 0x02C: Disable the CPU WFI
	volatile unsigned int alive_slow_osc_enb;				// 0x030: Alive Slow OSC Enable Register
	volatile unsigned int alive_force_slow_osc_enb;				// 0x034: Alive Force Slow OSC Enable Register
	volatile unsigned int alive_osc_stable_val;				// 0x038: Alive OSC Stable Value Register
	volatile unsigned int alive_hard_reset_enb;				// 0x03C: Alive Hard Reset Enable Register
	volatile unsigned int vddoff_delay_for_repower_on;			// 0x040: VDD Off Delay For Repower On Register
	volatile unsigned int reg_core_swrst_rst;				// 0x044: Core Software Reset Register
	volatile unsigned int reg_core_swrst_rst_enb;				// 0x048: Core Software Reset Enable Register
	volatile unsigned int wdt_rst_enb;					// 0x04C: Watchdog Reset Enable Register
	volatile unsigned int repoweron_enb_reg;				// 0x050: Repower On Enable Register
	volatile unsigned int core_rst_status;					// 0x054: Core Reset Status Register
	volatile unsigned int alive_pad_stable;					// 0x058: Pad Stable Register (ramping time)
	volatile unsigned int norm_div;						// 0x05C: Normal Mode OSC Clock Select
	volatile unsigned int stop_div;						// 0x060: Stop Mode OSC Clock Select
	volatile unsigned int reg_clken_debug;					// 0x064: OSC Select Debug Value
	volatile unsigned int reg_clksel_done;					// 0x068: OSC Clock Select Done
	volatile unsigned int reg_rst_config;					// 0x06C: Read the Boot Option
	volatile unsigned int reserved1[36];					// 0x070 ~ 0x0FC
	volatile unsigned int new_scratch[31];					// 0x100 ~ 0x17C
};

#endif /* #ifndef __ALIVE_H__ */
