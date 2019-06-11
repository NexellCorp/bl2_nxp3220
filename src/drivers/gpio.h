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
#ifndef __GPIO_H__
#define __GPIO_H__

struct	nx_gpio_reg {
	volatile unsigned int out;						// 0x00 : Output Register
	volatile unsigned int outenb;						// 0x04 : Output Enable Register
	volatile unsigned int detmode[2];					// 0x08 : Event Detect Mode Register
	volatile unsigned int intenb;						// 0x10 : Interrupt Enable Register
	volatile unsigned int det;						// 0x14 : Event Detect Register
	volatile unsigned int pad;						// 0x18 : PAD Status Register
	volatile unsigned int puenb;						// 0x1C : Pull Up Enable Register
	volatile unsigned int altfn[2];						// 0x20 : Alternate Function Select Register
	volatile unsigned int detmodeex;					// 0x28 : Event Detect Mode extended Register

	volatile unsigned int reserved[4];					// 0x2C ~ 0x38 :
	volatile unsigned int detenb;						// 0x3C : IntPend Detect Enable Register

	volatile unsigned int slew;						// 0x40 : Slew Register
	volatile unsigned int slew_disalbe_default;				// 0x44 : Slew set On/Off Register
	volatile unsigned int drv1;						// 0x48 : drive strength LSB Register
	volatile unsigned int drv1_disable_default;				// 0x4C : drive strength LSB set On/Off Register
	volatile unsigned int drv0;						// 0x50 : drive strength MSB Register
	volatile unsigned int drv0_disable_default;				// 0x54 : drive strength MSB set On/Off Register
	volatile unsigned int pullsel;						// 0x58 : Pull UP/DOWN Selection Register
	volatile unsigned int pullsel_disable_default;				// 0x5C : Pull UP/DOWN Selection On/Off Register
	volatile unsigned int pullenb;						// 0x60 : Pull Enable/Disable Register
	volatile unsigned int pullenb_disable_default;				// 0x64 : Pull Enable/Disable selection On/Off Register
	volatile unsigned int input_mux_select0;				// 0x68 :
	volatile unsigned int input_mux_select1;				// 0x6C :
	volatile unsigned int secure_marking_reg;				// 0x70 : GPIOx SECURE Marking Register
	volatile unsigned int inputenb;						// 0x74 : GPIOx Input Enable/Disable
	volatile unsigned int inputenb_disable_defalut;				// 0x78 : GPIOx Input Enable/Disable On/Off Register
	volatile unsigned int altfn_ex;						// 0x7C : GPIOx Alternation Function
};

struct nx_gpio_regs {
	struct nx_gpio_reg rw_gpio;
	struct nx_gpio_reg s_gpio;
	struct nx_gpio_reg c_gpio;
	unsigned char reserved0[0x10000 - (3 * sizeof(struct nx_gpio_reg))];
};

struct nx_gpio_padi {
	unsigned int alternate	: 3;
	unsigned int pad	: 5;
	unsigned int group	: 8;
	unsigned int property	: 16;
};

union nx_gpio_pad {
	unsigned int padd;
	struct nx_gpio_padi padi;
};

/* Function Define */
void gpio_set_outenb(int module, int pad, int enb);
void gpio_set_output(int module, int pad, int val);
int gpio_get_pad(int moudle, int pad);

void gpio_set_pullenb(int module, int pad, int enb);
void gpio_set_pull(int module, int pad, int sel);

void gpio_set_alt_function(int module, int pad, int alt);

void nx_gpio_set_alt(const union nx_gpio_pad *pad);

#endif /* __GPIO_H__ */
