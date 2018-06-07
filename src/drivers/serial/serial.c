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
#include <sysheader.h>
#include <cmu.h>
#include <uart.h>
#include <gpio.h>
#include <serial.h>

#define CONFIG_REQ_UART_SRC_FREQ		(200000000)
#define CONFIG_REQ_UCLK_FREQ			(100000000)			// 100Mhz
#define CONFIG_SERIAL_BAUDRATE			(115200)

/* External Function */
static volatile unsigned int g_uart_reg;

static union nx_gpio_pad g_uart_pad[7][2] = {
	{ {PAD_INDEX_UART0_TXD}, {PAD_INDEX_UART0_RXD_SMC} },
	{ {PAD_INDEX_UART1_TXD}, {PAD_INDEX_UART1_RXD_SMC} },
	{ {PAD_INDEX_UART2_TXD}, {PAD_INDEX_UART2_RXD_SMC} },
	{ {PAD_INDEX_UART3_TXD}, {PAD_INDEX_UART3_RXD_SMC} },
	{ {PAD_INDEX_UART4_TXD}, {PAD_INDEX_UART4_RXD_SMC} },
	{ {PAD_INDEX_UART5_TXD}, {PAD_INDEX_UART5_RXD_SMC} },
	{ {PAD_INDEX_UART6_TXD}, {PAD_INDEX_UART6_RXD_SMC} },
};

static const unsigned int g_clk_num[2] = {
	UART_0_APB_CLK, UART_0_CORE_CLK
};

static int serial_get_baseaddr(unsigned int channel)
{
	return (PHY_BASEADDR_UART_CH0 + (channel * 0x10000));
}

static int get_uart_source_clk(void)
{
	return cmu_get_rate(g_clk_num[1]);
}

static int set_uclk_source(int freq)
{
	cmu_set_rate(g_clk_num[0], CONFIG_REQ_UART_SRC_FREQ);
	cmu_set_rate(g_clk_num[1], freq);

	return 0;
}

static int set_uclk_enb(int enb)
{
//	cmu_clk_enable(g_clk_num[0], enb);
	cmu_clk_enable(g_clk_num[1], enb);

	return 0;
}

void serial_set_baudrate (int channel, int uclk, int baud_rate)
{
	volatile unsigned int base;

	int ibrd, fbrd;

	base = serial_get_baseaddr(channel);

	/* Divisor Latch Access Bit. */
	mmio_set_32((base + LCR), DLAB);

	/* step xx. calculates an integer at the baud rate */
	ibrd = (uclk / ((baud_rate/1) * 16));					// ibrd = 8, 115200bps

	/* step xx. calculates an fractional at the baud rate */
	fbrd = ((uclk % ((((baud_rate/1) * 16) + 32) * 64))			\
					/ (baud_rate / 1) * 16);		// fbrd = 0,

//	mmio_write_32((base + DLH), ((ibrd >> 8) & 0xFF));			// Divider Latch High 8bit
	mmio_write_32((base + DLL), ((ibrd >> 0) & 0xFF));			// Divider Latch Low 8bit

	// DLF - After confirming the number of bits in the field, write it down.
	mmio_write_32((base + DLF), fbrd);

	/* Divisor Latch Access Bit. */
	mmio_clear_32((base + LCR), DLAB);
}

int serial_init(unsigned int channel)
{
	volatile unsigned int reg_value = 0;

	g_uart_reg = serial_get_baseaddr(channel);

	/* step xx. set the cmu (source clock) */
	set_uclk_source(CONFIG_REQ_UCLK_FREQ);

	/* step xx. enable the (ext:uart clock)clock in uart block*/
	set_uclk_enb(TRUE);

	/* step xx. calculates an integer at the baud rate */
	serial_set_baudrate(channel, get_uart_source_clk(),
			CONFIG_SERIAL_BAUDRATE);

	/* step xx. change the (tx, rx)gpio-alternative function */
	nx_gpio_set_alt(&g_uart_pad[channel][0]);
	nx_gpio_set_alt(&g_uart_pad[channel][1]);

	/* step xx. set the serial (:uart) */
#if 0
	reg_value = (XMIT_FIFO_RESET|RCVR_FIFO_RESET|UART_RESET);		// Software - Tx, Rx FIFO Reset, Uart Reset
	mmio_write_32((g_uart_reg + SRR), reg_value);
#endif

	reg_value = (XFIFOR|RFIFOR|FIFO_ENB);					// Tx, Rx FIFO Reset, FIFO Enable (Rx:0x2 Tx:0x1)
	mmio_write_32((g_uart_reg + FCR), reg_value);

	reg_value = DATA_LENGTH(0x3);						// Parity Bit: Even, Stop Bit: 1Bit, Data Length: 8Bit
	mmio_write_32((g_uart_reg + LCR), reg_value);

	return 0;
}

int serial_is_tx_empty(void)
{
	return (int)((mmio_read_32(g_uart_reg + USR) & TX_FIFO_NOTEMPTY));
}

int serial_is_rx_empty(void)
{
	return (int)(!(mmio_read_32(g_uart_reg + USR) & RX_FIFO_NOTEMPTY));
}

char serial_getc(void)
{
	while (!(mmio_read_32(g_uart_reg + USR) & RX_FIFO_NOTEMPTY));
	return (char)mmio_read_32(g_uart_reg + RBR);
}

void serial_putc(char ch)
{
	while (!(mmio_read_32(g_uart_reg + USR) & TX_FIFO_NOTFULL));
	mmio_write_32((g_uart_reg + THR), (unsigned int)ch);
}

int serial_is_busy(void)
{
	return (int)(mmio_read_32(g_uart_reg + USR) & UART_BUSY);
}

int serial_is_uart_tx_done(void)
{
	int tx_empty = (int)(!(mmio_read_32(g_uart_reg + USR) & TX_FIFO_NOTEMPTY));
	int busy = (int)(mmio_read_32(g_uart_reg + USR) & UART_BUSY);

	if (tx_empty && !busy)
		return 1;

	return 0;
}