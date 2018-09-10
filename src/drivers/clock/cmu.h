/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: DeokJin, Lee <truevirtue@nexell.co.kr>
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

#ifndef __CMU_H__
#define __CMU_H__

#define CMU_NAME_SRC				0
#define CMU_NAME_SYS				1
#define CMU_NAME_USB				2
#define CMU_NAME_HSIF				3
#define CMU_NAME_MM				4
#define CMU_NAME_CPU				5
#define CMU_NAME_DDR				6

#define CMU_TYPE_MAINDIV  			0
#define CMU_TYPE_SUBDIV0  			1
#define CMU_TYPE_SUBDIV1  			2
#define CMU_TYPE_SUBDIV2  			3
#define CMU_TYPE_SUBDIV3  			4
#define CMU_TYPE_SUBDIV4  			5
#define CMU_TYPE_SUBDIV5  			6
#define CMU_TYPE_SUBDIV6  			7
#define CMU_TYPE_ONLYGATE 			8

#define MAX_PLL_NUM				4
#define MAX_DIV					255

#define CMU_INIT(clk_num, pset, t, addr, bitidx, pid, g_idx)	\
{								\
	.reg		= addr,					\
	.p_set		= pset,					\
	.clkenbit	= bitidx,				\
	.type		= t,					\
	.p_id		= pid,					\
	.grp_idx	= g_idx					\
}

/* "CMU_SRC " Clock Index */
#define SRC_SYS_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 0)
#define SRC_SYS_0_HSIF_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 1)
#define SRC_CPU_BACKUP_0_CLK			 (((CMU_NAME_SRC) << 28) | 2)
#define SRC_CSSYS_0_HCLK_CLK			 (((CMU_NAME_SRC) << 28) | 3)
#define SRC_CSSYS_0_SCAN_CLK			 (((CMU_NAME_SRC) << 28) | 4)
#define SRC_BLK_CMU_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 5)
#define SRC_VIP_PADPLACE_0_SCAN_CLK		 (((CMU_NAME_SRC) << 28) | 6)
#define SRC_PADOUT_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 7)
#define SRC_PADOUT_1_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 8)
#define SRC_PADOUT_2_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 9)
#define SRC_PADOUT_3_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 10)
#define SRC_PADOUT_4_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 11)
#define SRC_HPM_SYS_0_HPM_CLK			 (((CMU_NAME_SRC) << 28) | 12)
#define SRC_UART_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 13)
#define SRC_UART_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 14)
#define SRC_I2S_0_SCAN_CLK			 (((CMU_NAME_SRC) << 28) | 15)
#define SRC_I2S_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 16)
#define SRC_I2S_1_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 17)
#define SRC_I2S_2_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 18)
#define SRC_I2S_3_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 19)
#define SRC_I2C_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 20)
#define SRC_MP2TSI_0_SCAN_CLK			 (((CMU_NAME_SRC) << 28) | 21)
#define SRC_SDMMC_0_SCAN_CLK			 (((CMU_NAME_SRC) << 28) | 22)
#define SRC_SDMMC_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 23)
#define SRC_SDMMC_1_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 24)
#define SRC_SDMMC_2_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 25)
#define SRC_SPI_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 26)
#define SRC_SPI_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 27)
#define SRC_PDM_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 28)
#define SRC_PDM_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 29)
#define SRC_PWM_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 30)
#define SRC_PWM_0_TCLK0_CLK			 (((CMU_NAME_SRC) << 28) | 31)
#define SRC_PWM_0_TCLK1_CLK			 (((CMU_NAME_SRC) << 28) | 32)
#define SRC_PWM_0_TCLK2_CLK			 (((CMU_NAME_SRC) << 28) | 33)
#define SRC_PWM_0_TCLK3_CLK			 (((CMU_NAME_SRC) << 28) | 34)
#define SRC_CAN_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 35)
#define SRC_CAN_1_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 36)
#define SRC_TIMER_0_APB_CLK			 (((CMU_NAME_SRC) << 28) | 37)
#define SRC_TIMER_0_TCLK0_CLK			 (((CMU_NAME_SRC) << 28) | 38)
#define SRC_TIMER_0_TCLK1_CLK			 (((CMU_NAME_SRC) << 28) | 39)
#define SRC_TIMER_0_TCLK2_CLK			 (((CMU_NAME_SRC) << 28) | 40)
#define SRC_TIMER_0_TCLK3_CLK			 (((CMU_NAME_SRC) << 28) | 41)
#define SRC_SECURE_TIMER_0_APB_CLK		 (((CMU_NAME_SRC) << 28) | 42)
#define SRC_SECURE_TIMER_0_TCLK0_CLK		 (((CMU_NAME_SRC) << 28) | 43)
#define SRC_SECURE_TIMER_0_TCLK1_CLK		 (((CMU_NAME_SRC) << 28) | 44)
#define SRC_SECURE_TIMER_0_TCLK2_CLK		 (((CMU_NAME_SRC) << 28) | 45)
#define SRC_SECURE_TIMER_0_TCLK3_CLK		 (((CMU_NAME_SRC) << 28) | 46)
#define SRC_SMC_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 47)
#define SRC_SPDIFTX_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 48)
#define SRC_GMAC_RGMII_0_TX_CLK_CLK		 (((CMU_NAME_SRC) << 28) | 49)
#define SRC_GMAC_RGMII_0_PTP_REF_CLK		 (((CMU_NAME_SRC) << 28) | 50)
#define SRC_GMAC_RGMII_0_SCAN_CLK		 (((CMU_NAME_SRC) << 28) | 51)
#define SRC_GMAC_RMII_0_PTP_REF_CLK		 (((CMU_NAME_SRC) << 28) | 52)
#define SRC_GMAC_RMII_0_SCAN_RMII_CLK		 (((CMU_NAME_SRC) << 28) | 53)
#define SRC_NANDC_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 54)
#define SRC_MM_0_AXI_CLK			 (((CMU_NAME_SRC) << 28) | 55)
#define SRC_VIP_0_PADOUT0_CLK			 (((CMU_NAME_SRC) << 28) | 56)
#define SRC_VIP_0_PADOUT1_CLK			 (((CMU_NAME_SRC) << 28) | 57)
#define SRC_DPC_0_X2_CLK			 (((CMU_NAME_SRC) << 28) | 58)
#define SRC_LVDS_0_VCLK_CLK			 (((CMU_NAME_SRC) << 28) | 59)
#define SRC_CODA960_0_CORE_CLK			 (((CMU_NAME_SRC) << 28) | 60)
#define SRC_USB_0_AHB_CLK			 (((CMU_NAME_SRC) << 28) | 61)

/* "CMU_SYS " Clock Index */
#define SYS_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 0)
#define SYS_BUS_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 1)
#define ETC_BUS_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 2)
#define FSYS_BUS_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 3)
#define DMA_BUS_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 4)
#define CFG_BUS_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 5)
#define AXISRAM_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 6)
#define MP2TSI_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 7)
#define MP2TSI_1_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 8)
#define TOP_BIST_CLK_0_333MHZ_CLK		 (((CMU_NAME_SYS) << 28) | 9)
#define DMA_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 10)
#define SSS_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 11)
#define DMA_1_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 12)
#define SDMA_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 13)
#define SDMA_1_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 14)
#define MDMA_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 15)
#define AXIM_SDMMC_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 16)
#define AXIM_SDMMC_1_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 17)
#define AXIM_SDMMC_2_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 18)
#define SYS_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 19)
#define DPC_PADPLACE_0_CLK			 (((CMU_NAME_SYS) << 28) | 20)
#define I2S_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 21)
#define I2S_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 22)
#define I2S_2_APB_CLK				 (((CMU_NAME_SYS) << 28) | 23)
#define I2S_3_APB_CLK				 (((CMU_NAME_SYS) << 28) | 24)
#define MP2TSI_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 25)
#define MP2TSI_1_APB_CLK			 (((CMU_NAME_SYS) << 28) | 26)
#define WDT_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 27)
#define WDT_0_POR_CLK				 (((CMU_NAME_SYS) << 28) | 28)
#define SECURE_WDT_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 29)
#define SECURE_WDT_0_POR_CLK			 (((CMU_NAME_SYS) << 28) | 30)
#define SYSREG_SYS_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 31)
#define ECID_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 32)
#define SYSCTRLTOP_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 33)
#define CAN_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 34)
#define CAN_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 35)
#define TMU_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 36)
#define DMA_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 37)
#define SSS_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 38)
#define DMA_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 39)
#define SMC_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 40)
#define GPIO_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 41)
#define SDMA_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 42)
#define GPIO_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 43)
#define SDMA_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 44)
#define GPIO_2_APB_CLK				 (((CMU_NAME_SYS) << 28) | 45)
#define GPIO_3_APB_CLK				 (((CMU_NAME_SYS) << 28) | 46)
#define GPIO_4_APB_CLK				 (((CMU_NAME_SYS) << 28) | 47)
#define ADC_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 48)
#define MDMA_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 49)
#define DUMMY_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 50)
#define SPDIFTX_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 51)
#define SPDIFRX_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 52)
#define AXIM_SDMMC_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 53)
#define AXIM_SDMMC_1_APB_CLK			 (((CMU_NAME_SYS) << 28) | 54)
#define AXIM_SDMMC_2_APB_CLK			 (((CMU_NAME_SYS) << 28) | 55)
#define SYS_0_HSIF_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 56)
#define BLK_HSIF_DATA_BUS_0_AXI_CLK		 (((CMU_NAME_SYS) << 28) | 57)
#define SDMMC_0_AHB_CLK				 (((CMU_NAME_SYS) << 28) | 58)
#define SDMMC_1_AHB_CLK				 (((CMU_NAME_SYS) << 28) | 59)
#define SDMMC_2_AHB_CLK				 (((CMU_NAME_SYS) << 28) | 60)
#define GMAC_RGMII_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 61)
#define GMAC_RMII_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 62)
#define SYS_0_HSIF_APB_CLK			 (((CMU_NAME_SYS) << 28) | 63)
#define BLK_HSIF_DATA_BUS_0_APB_CLK		 (((CMU_NAME_SYS) << 28) | 64)
#define HSIF_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 65)
#define SYSREG_HSIF_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 66)
#define GMAC_RGMII_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 67)
#define GMAC_RMII_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 68)
#define NANDC_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 69)
#define CPU_BACKUP_0_CLK			 (((CMU_NAME_SYS) << 28) | 70)
#define CSSYS_0_HCLK_CLK			 (((CMU_NAME_SYS) << 28) | 71)
#define CSSYS_0_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 72)
#define BLK_CMU_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 73)
#define VIP_PADPLACE_0_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 74)
#define PADOUT_0_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 75)
#define PADOUT_1_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 76)
#define PADOUT_2_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 77)
#define PADOUT_3_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 78)
#define PADOUT_4_AXI_CLK			 (((CMU_NAME_SYS) << 28) | 79)
#define HPM_SYS_0_HPM_CLK			 (((CMU_NAME_SYS) << 28) | 80)
#define UART_0_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 81)
#define UART_1_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 82)
#define UART_2_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 83)
#define UART_3_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 84)
#define UART_4_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 85)
#define UART_5_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 86)
#define UART_6_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 87)
#define UART_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 88)
#define UART_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 89)
#define UART_2_APB_CLK				 (((CMU_NAME_SYS) << 28) | 90)
#define UART_3_APB_CLK				 (((CMU_NAME_SYS) << 28) | 91)
#define UART_4_APB_CLK				 (((CMU_NAME_SYS) << 28) | 92)
#define UART_5_APB_CLK				 (((CMU_NAME_SYS) << 28) | 93)
#define UART_6_APB_CLK				 (((CMU_NAME_SYS) << 28) | 94)
#define I2S_0_SCAN_CLK				 (((CMU_NAME_SYS) << 28) | 95)
#define I2S_1_SCAN_CLK				 (((CMU_NAME_SYS) << 28) | 96)
#define I2S_2_SCAN_CLK				 (((CMU_NAME_SYS) << 28) | 97)
#define I2S_3_SCAN_CLK				 (((CMU_NAME_SYS) << 28) | 98)
#define I2S_0_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 99)
#define I2S_1_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 100)
#define I2S_2_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 101)
#define I2S_3_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 102)
#define I2C_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 103)
#define I2C_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 104)
#define I2C_2_APB_CLK				 (((CMU_NAME_SYS) << 28) | 105)
#define I2C_3_APB_CLK				 (((CMU_NAME_SYS) << 28) | 106)
#define I2C_4_APB_CLK				 (((CMU_NAME_SYS) << 28) | 107)
#define MP2TSI_0_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 108)
#define MP2TSI_1_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 109)
#define SDMMC_0_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 110)
#define SDMMC_1_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 111)
#define SDMMC_2_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 112)
#define SDMMC_0_CORE_CLK			 (((CMU_NAME_SYS) << 28) | 113)
#define SDMMC_1_CORE_CLK			 (((CMU_NAME_SYS) << 28) | 114)
#define SDMMC_2_CORE_CLK			 (((CMU_NAME_SYS) << 28) | 115)
#define SPI_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 116)
#define SPI_1_APB_CLK				 (((CMU_NAME_SYS) << 28) | 117)
#define SPI_2_APB_CLK				 (((CMU_NAME_SYS) << 28) | 118)
#define SPI_0_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 119)
#define SPI_1_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 120)
#define SPI_2_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 121)
#define PDM_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 122)
#define PDM_0_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 123)
#define PWM_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 124)
#define PWM_0_TCLK0_CLK				 (((CMU_NAME_SYS) << 28) | 125)
#define PWM_0_TCLK1_CLK				 (((CMU_NAME_SYS) << 28) | 126)
#define PWM_0_TCLK2_CLK				 (((CMU_NAME_SYS) << 28) | 127)
#define PWM_0_TCLK3_CLK				 (((CMU_NAME_SYS) << 28) | 128)
#define CAN_0_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 129)
#define CAN_1_CORE_CLK				 (((CMU_NAME_SYS) << 28) | 130)
#define TIMER_0_APB_CLK				 (((CMU_NAME_SYS) << 28) | 131)
#define TIMER_0_TCLK0_CLK			 (((CMU_NAME_SYS) << 28) | 132)
#define TIMER_0_TCLK1_CLK			 (((CMU_NAME_SYS) << 28) | 133)
#define TIMER_0_TCLK2_CLK			 (((CMU_NAME_SYS) << 28) | 134)
#define TIMER_0_TCLK3_CLK			 (((CMU_NAME_SYS) << 28) | 135)
#define SECURE_TIMER_0_APB_CLK			 (((CMU_NAME_SYS) << 28) | 136)
#define SECURE_TIMER_0_TCLK0_CLK		 (((CMU_NAME_SYS) << 28) | 137)
#define SECURE_TIMER_0_TCLK1_CLK		 (((CMU_NAME_SYS) << 28) | 138)
#define SECURE_TIMER_0_TCLK2_CLK		 (((CMU_NAME_SYS) << 28) | 139)
#define SECURE_TIMER_0_TCLK3_CLK		 (((CMU_NAME_SYS) << 28) | 140)
#define SMC_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 141)
#define SPDIFTX_0_CORE_CLK			 (((CMU_NAME_SYS) << 28) | 142)
#define GMAC_RGMII_0_TX_CLK_CLK			 (((CMU_NAME_SYS) << 28) | 143)
#define GMAC_RGMII_0_PTP_REF_CLK		 (((CMU_NAME_SYS) << 28) | 144)
#define GMAC_RGMII_0_SCAN_CLK			 (((CMU_NAME_SYS) << 28) | 145)
#define GMAC_RMII_0_PTP_REF_CLK			 (((CMU_NAME_SYS) << 28) | 146)
#define GMAC_RMII_0_SCAN_RMII_CLK		 (((CMU_NAME_SYS) << 28) | 147)
#define GMAC_RMII_0_SCAN_TRXCLK_CLK		 (((CMU_NAME_SYS) << 28) | 148)
#define NANDC_0_AXI_CLK				 (((CMU_NAME_SYS) << 28) | 149)

/* "CMU_CPU" - Clock Index */
#define CPU_0_ARM_CLK				 (((CMU_NAME_CPU) << 28) | 0)
#define BLK_CPU_BIST_CLK_0_800MHZ_CLK		 (((CMU_NAME_CPU) << 28) | 1)
#define CPU_0_AXI_CLK				 (((CMU_NAME_CPU) << 28) | 2)
#define AXIM_CPU_0_AXI_CLK			 (((CMU_NAME_CPU) << 28) | 3)
#define CPU_0_ATCLK_CLK				 (((CMU_NAME_CPU) << 28) | 4)
#define CPU_0_CNTCLK_CLK			 (((CMU_NAME_CPU) << 28) | 5)
#define CPU_0_TSCLK_CLK				 (((CMU_NAME_CPU) << 28) | 6)
#define CPU_0_DBGAPB_CLK			 (((CMU_NAME_CPU) << 28) | 7)
#define CPU_0_APB_CLK				 (((CMU_NAME_CPU) << 28) | 8)
#define SYSREG_CPU_0_APB_CLK			 (((CMU_NAME_CPU) << 28) | 9)
#define AXIM_CPU_0_APB_CLK			 (((CMU_NAME_CPU) << 28) | 10)
#define PLL_CPU_DIV_0_CLK			 (((CMU_NAME_CPU) << 28) | 11)
#define HPM_CPU_0_HPM_CLK			 (((CMU_NAME_CPU) << 28) | 12)


/* "CMU_DDR" - Clock Index */
#define DDR_0_DDR_CLK                      	(((CMU_NAME_DDR)<<28)| 0)
#define DDR_0_AXI_CLK                      	(((CMU_NAME_DDR)<<28)| 1)
#define TZASC_DDR_0_AXI_CLK                	(((CMU_NAME_DDR)<<28)| 2)
#define DDR_0_APB_CLK                      	(((CMU_NAME_DDR)<<28)| 3)
#define SYSREG_DDR_0_APB_CLK               	(((CMU_NAME_DDR)<<28)| 4)
#define PLL_DDR0_DIV_0_CLK                 	(((CMU_NAME_DDR)<<28)| 5)
#define PLL_DDR1_DIV_0_CLK                 	(((CMU_NAME_DDR)<<28)| 6)

/* "CMU_HSIF" - Clock Index */


/* "CMU_MM" - CLock Index */


/* "CMU_USB" - CLock Index */
#define USB_0_AHB_CLK                     	(((CMU_NAME_USB)<<28)| 0)
#define SYSREG_USB_0_APB_CLK               	(((CMU_NAME_USB)<<28)| 1)
#define USB20OTG_0_AHB_CLK                 	(((CMU_NAME_USB)<<28)| 2)
#define USB20HOST_0_AHB_CLK                	(((CMU_NAME_USB)<<28)| 3)

/* CMU TOPC block device structure */
struct nx_clk_topcmu {
	volatile unsigned int clk_mux_sel;		// 0x0
	volatile unsigned int rsv[7];			// 0x04 ~ 0x1C
	volatile unsigned int clkgrp_clk_divstop[4];	// 0x20 ~ 0x2C
	volatile unsigned int oscclk_mux_sel;		// 0x30
	volatile unsigned int rsv0[3];			// 0x34 ~ 0x3C
	volatile unsigned int grp_clk_srcoff_set[4];	// 0x40 ~ 0x4C
	volatile unsigned int grp_clk_srcoff_clr[4];	// 0x50 ~ 0x5C
};

struct nx_clk_cmu {
	volatile unsigned int grp_clk_src;		// 0x0
	volatile unsigned int grp_all_div_rst;		// 0x4
	volatile unsigned int rsv0[2];			// 0x08 ~ 0xC
	volatile unsigned int grp_clkenb[4];		// 0x10 ~ 0x1C
	volatile unsigned int grp_clkenb_clear[4];	// 0x20 ~ 0x2C
	volatile unsigned int grp_rst_set[4];		// 0x30 ~ 0x3C
	volatile unsigned int grp_rst_clr[4];		// 0x40 ~ 0x4C
	volatile unsigned int grp_rst_mode[4];		// 0x50 ~ 0x5C
	volatile unsigned int div_val[7];		// 0x60 ~ 0x78
	volatile unsigned int rsv1;			// 0x7C
	volatile unsigned int cur_divider[7];		// 0x80 ~ 0x98
	volatile unsigned int rsv2;			// 0x100
};

struct nx_clk_priv {
	struct nx_clk_cmu *reg;
	int p_set;
	int type;
	int clkenbit;
	int p_id;
	int grp_idx;
};

/* Define Function */
long get_optimal_div(unsigned int index, int req_freq,
			unsigned int *s_div, unsigned int *y_div);
 long get_src_mux(unsigned int index);

 int cmu_clk_enable (int index, int enable);
void cmu_clk_divstop(int index, unsigned int enable);
void cmu_clkgrp_enable(int index, unsigned int enable);
unsigned long cmu_set_rate(unsigned int index, unsigned long freq);
unsigned long cmu_get_rate(unsigned int index);

#endif //#ifndef __CMU_H__
