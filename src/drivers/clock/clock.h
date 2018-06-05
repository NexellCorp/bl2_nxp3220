#ifndef __CLOCK_H__
#define __CLOCK_H__

#define CPU_0_AXI_CLK_MAX				(400 * 1000 * 1000)
#define SYS_0_AXI_CLK_MAX				(100 * 1000 * 1000)
#define DDR_0_AXI_CLK_MAX				(200 * 1000 * 1000)
#define DDR_0_APB_CLK_MAX				(100 * 1000 * 1000)

/* Clock Function Define */
int clock_initialize(void);
void clock_information(void);

#endif // _CLOCK_H__
