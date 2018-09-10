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
#ifndef __I2C_GPIO_H__
#define __I2C_GPIO_H__

/* Function Define  */
void i2c_gpio_init(unsigned char gpio_grp,
	unsigned char gpio_scl, unsigned char gpio_sda,
	unsigned int gpio_scl_alt, unsigned int gpio_sda_alt);

int i2c_gpio_read(char dev_addr, char reg_addr, char *pdata, int length);
int i2c_gpio_write(char dev_addr, char reg_addr, char *pdata, int length);
void i2c_gpio_deinit(void);

#define I2C_INIT(GRP, SCL, SDA, SCL_ALT, SDA_ALT)	i2c_gpio_init(GRP, SCL, SDA, SCL_ALT, SDA_ALT)
#define I2C_READ(DEV, REG, DATA, SIZE)			i2c_gpio_read(DEV, REG, DATA, SIZE)
#define I2C_WRITE(DEV, REG, DATA, SIZE)			i2c_gpio_write(DEV, REG, DATA, SIZE)
#define I2C_DEINIT					i2c_gpio_deinit

#endif /* __I2C_GPIO_H__ */
