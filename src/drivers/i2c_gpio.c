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
#include <sysheader.h>
#include <gpio.h>

//#define DEV_ERROR			ERROR
#define DEV_ERROR			printf
//#define DEV_ERROR(x)

#define GPIOX_LOW			0
#define GPIOX_HIGH			1

#define STRETCHING_TIMEOUT		100
#define I2C_DELAY_TIME			2

#define I2C_READ_BIT			1
#define I2C_WRITE_BIT			0

static int g_started;
static unsigned char g_gpio_grp;
static unsigned char g_gpio_scl;
static unsigned char g_gpio_sda;

static inline void i2c_delay(unsigned int us)
{
	volatile unsigned int i = 0, j = 0;
	for (i = 0; i < us * 0x1; i++)
		for (j = 0; j < 0x2; j++);
}

static void sda_low(void)
{
	gpio_set_outenb(g_gpio_grp, g_gpio_sda, GPIOX_HIGH);
}

static void scl_low(void)
{
	gpio_set_outenb(g_gpio_grp, g_gpio_scl, GPIOX_HIGH);
}

static int sda_read(void)
{
	gpio_set_outenb(g_gpio_grp, g_gpio_sda, GPIOX_LOW);

	return gpio_get_pad(g_gpio_grp, g_gpio_sda);
}

static int scl_read(void)
{
	gpio_set_outenb(g_gpio_grp, g_gpio_scl, GPIOX_LOW);

	return gpio_get_pad(g_gpio_grp, g_gpio_scl);
}

/* START: High -> Low on SDA while SCL is High */
static int i2c_gpio_send_start(void)
{
	int timeout = STRETCHING_TIMEOUT;

	if (g_started == TRUE) {
		sda_read();

		i2c_delay(I2C_DELAY_TIME);

		/* clock stretching.... need timeout code */
		while (scl_read() == FALSE) {
			if (timeout-- == 0) {
				DEV_ERROR("(%s) clock timeout arbitration fail! \r\n",
					__func__);
				return FALSE;
			}
			i2c_delay(I2C_DELAY_TIME);
		}
		i2c_delay(I2C_DELAY_TIME);
	}

	if (sda_read() == FALSE) {
		DEV_ERROR("(%s) sda arbitration fail! \r\n", __func__);
		return FALSE;	// arbitration lost
	}

	sda_low();
	i2c_delay(I2C_DELAY_TIME);
	scl_low();

	g_started = TRUE;

	return TRUE;
}

/* STOP: Low -> High on SDA while SCL is High */
static int i2c_gpio_send_stop(void)
{
	int timeout = STRETCHING_TIMEOUT;

	sda_low();

	i2c_delay(I2C_DELAY_TIME);

	/* need timeout code */
	while (scl_read() == FALSE) {
		if (timeout-- == 0) {
			DEV_ERROR("stop bit clock timeout arbitration fail! \r\n");
			return FALSE;
		}
		i2c_delay(I2C_DELAY_TIME);
	}

	i2c_delay(I2C_DELAY_TIME);

	sda_read();

	g_started = FALSE;
	timeout = STRETCHING_TIMEOUT;
	while (sda_read() == FALSE) {
		if (timeout-- == 0) {
			DEV_ERROR("stop bit sda arbitration fail! \r\n");
			return FALSE; // arbitration_lost
		}
		i2c_delay(I2C_DELAY_TIME);
	}

	i2c_delay(I2C_DELAY_TIME);

	return TRUE;
}

static int i2c_read_bit(int *bit)
{
	unsigned int timeout = STRETCHING_TIMEOUT;

	i2c_delay(I2C_DELAY_TIME);
	sda_read();
	i2c_delay(I2C_DELAY_TIME);

	/* clock stretching.... need timeout code */
	while (scl_read() == FALSE) {
		if (timeout-- == 0) {
			DEV_ERROR("(%s) clock timeout arbitration fail! \r\n",
				__func__);
			return FALSE;
		}
		i2c_delay(I2C_DELAY_TIME);
	}

	*bit = sda_read();		// data or nack read
	i2c_delay(I2C_DELAY_TIME);
	scl_low();

	return TRUE;
}

static int i2c_write_bit(int bit)
{
	int timeout = STRETCHING_TIMEOUT;

	i2c_delay(I2C_DELAY_TIME);

	if (bit)
		sda_read();
	else
		sda_low();

	i2c_delay(I2C_DELAY_TIME);

	while (scl_read() == FALSE) {
		if (timeout-- == 0) {
			DEV_ERROR("(%s) clock timeout arbitration fail! \r\n",
					__func__);
			return FALSE;
		}
		i2c_delay(I2C_DELAY_TIME);
	}

	if (bit && (sda_read() == FALSE)) {
		DEV_ERROR("(%s) sda arbitration fail! \r\n", __func__);
		return FALSE;	// arbitration_lost
	}

	i2c_delay(I2C_DELAY_TIME);

	scl_low();

	return TRUE;
}

/* Send 8 bits and look for an acknowledgement */
static int i2c_write_byte(int send_start, int send_stop,
						char data, int *nack)
{
	int bit;
	int arbitration;

	if (send_start) {
		arbitration = i2c_gpio_send_start();
		if (arbitration == FALSE)
			return FALSE;
	}

	for (bit = 0; bit < 8; bit++) {
		arbitration = i2c_write_bit((int)((data & 0x80) != 0));
		if (arbitration == FALSE)
			return FALSE;
		data <<= 1;
	}

	arbitration = i2c_read_bit(nack);
	if (arbitration == FALSE)
		return FALSE;

	if (*nack == TRUE) {
		DEV_ERROR("(%s) nack returned! \r\n", __func__);
		return FALSE;
	}

	if (send_stop) {
		arbitration = i2c_gpio_send_stop();
		if (arbitration == FALSE)
			return FALSE;
	}

	return TRUE;
}

/**
 * if ack == I2C_ACK, ACK the byte so can continue reading, else
 * send I2C_NOACK to end the read.
 */
static int i2c_read_byte(int nack, int send_stop, char *pdata)
{
	char byte = 0;
	unsigned int bit;
	int result, rbit;

	for (bit = 0; bit < 8; bit++) {
		result = i2c_read_bit(&rbit);
		if (result == FALSE)
			return FALSE;
		byte = (byte << 1) | (rbit ? 1 : 0);
	}
	*pdata = byte;

	result = i2c_write_bit(nack);
	if (result == FALSE)
		return FALSE;

	if (send_stop) {
		result = i2c_gpio_send_stop();
		if (result == FALSE)
			return FALSE;
	}

	return TRUE;
}


int i2c_gpio_read(char dev_addr, char reg_addr, char *pdata, int length)
{
	int nack, result;
	int byte;

	result = i2c_write_byte(TRUE, FALSE,
			(dev_addr << 1 | I2C_WRITE_BIT), &nack);
	if (result == FALSE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	if (nack == TRUE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}

	result = i2c_write_byte(FALSE, FALSE, reg_addr, &nack);
	if (result == FALSE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	if (nack == TRUE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	result = i2c_write_byte(TRUE, FALSE,
			(dev_addr << 1 | I2C_READ_BIT), &nack);
	if (result == FALSE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	if (nack == TRUE) {
		DEV_ERROR("i2c-device address write abitration error! \r\n");
		return FALSE;
	}

	for (byte = 0; byte < length; ) {
		byte++;
		result = i2c_read_byte((byte == length) ? TRUE : FALSE,
				 (byte == length) ? TRUE : FALSE, pdata++);
		if (result == FALSE) {
			DEV_ERROR("i2c-device data read abitration error! \r\n");
			return FALSE;
		}
	}

	return TRUE;
}

int i2c_gpio_write(char dev_addr, char reg_addr, char* pdata, int length)
{
	int nack, result;
	int byte;

	result = i2c_write_byte(TRUE, FALSE,
			((dev_addr << 1) & ~(I2C_WRITE_BIT)), &nack);
	if (result == FALSE) {
		DEV_ERROR("1. i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	if (nack == TRUE) {
		DEV_ERROR("2. i2c-device address write abitration error! \r\n");
		return FALSE;
	}

	result = i2c_write_byte(FALSE, FALSE, reg_addr, &nack);
	if (result == FALSE) {
		DEV_ERROR("3. i2c-device address write abitration error! \r\n");
		return FALSE;
	}
	if (nack == TRUE) {
		DEV_ERROR("4. i2c-device address write abitration error! \r\n");
		return FALSE;
	}

	for (byte = 0; byte < length;) {
		byte++;
		result = i2c_write_byte(FALSE, (byte == length) ? TRUE : FALSE,
					*pdata++, &nack);
		if (result == FALSE) {
			DEV_ERROR("1. i2c-device data write abitration error! \r\n");
			return FALSE;
		}

		if (nack == TRUE) {
			DEV_ERROR("2. i2c-device data write abitration error! \r\n");
			return FALSE;
		}
	}

	return TRUE;
}

void i2c_gpio_init(unsigned char gpio_grp, unsigned char gpio_scl, unsigned char gpio_sda,
	unsigned int gpio_scl_alt, unsigned int gpio_sda_alt)
{
	g_started = TRUE;

	g_gpio_grp = gpio_grp;
	g_gpio_scl = gpio_scl;
	g_gpio_sda = gpio_sda;

	gpio_set_output(g_gpio_grp, g_gpio_scl, 0);				// Output - Low
	gpio_set_output(g_gpio_grp, g_gpio_sda, 0);				// Output - Low

	gpio_set_outenb(g_gpio_grp, g_gpio_scl, FALSE);				// SCL (Input Mode)
	gpio_set_outenb(g_gpio_grp, g_gpio_sda, FALSE);				// SDA (Input Mode)

	gpio_set_alt_function(g_gpio_grp, g_gpio_scl, gpio_scl_alt);		// SCL Alt Mode
	gpio_set_alt_function(g_gpio_grp, g_gpio_sda, gpio_sda_alt);		// SDA Alt Mode

	/* GPIOx Internal Pull Mode (ref. i2c) */
	gpio_set_pullenb(g_gpio_grp, g_gpio_scl, TRUE);				// SCL Pull Enable
	gpio_set_pull(g_gpio_grp, g_gpio_scl, TRUE);				// SCL Pull-Sel (Pull-Up)
	gpio_set_pullenb(g_gpio_grp, g_gpio_sda, TRUE);				// SDA Pull Enable
	gpio_set_pull(g_gpio_grp, g_gpio_sda, TRUE);				// SDA Pull-Sel (Pull-Up)
}

void i2c_gpio_deinit(void)
{
	gpio_set_output(g_gpio_grp, g_gpio_scl, 0);
	gpio_set_output(g_gpio_grp, g_gpio_sda, 0);

	gpio_set_outenb(g_gpio_grp, g_gpio_scl, FALSE);
	gpio_set_outenb(g_gpio_grp, g_gpio_sda, FALSE);

	gpio_set_alt_function(g_gpio_grp, g_gpio_scl, 0);
	gpio_set_alt_function(g_gpio_grp, g_gpio_sda, 0);

	gpio_set_pullenb(g_gpio_grp, g_gpio_scl, FALSE);
	gpio_set_pull(g_gpio_grp, g_gpio_scl, FALSE);
	gpio_set_pullenb(g_gpio_grp, g_gpio_sda, FALSE);
	gpio_set_pull(g_gpio_grp, g_gpio_sda, FALSE);
}
