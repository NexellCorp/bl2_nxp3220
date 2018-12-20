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
#include <sysheader.h>
#include <libnx.h>
#include <memory.h>
#include <plat_load.h>

#define SIP_PLATFORM_LOAD		0x82000002

/* external function define */
extern int smc_call(unsigned int r0, unsigned int r1,
			unsigned int r2, unsigned int r3);

extern void self_refresh_entry(void);
extern void pmic_poweroff(void);

int plat_load(void)
{
	struct nx_bootmanager *pbm
		= ((struct nx_bootmanager *)RE_HEADER_BASEADDR);
	struct platform_info pi;

	pi.is_loadmark = ((pbm->bi.is_sss_f << 8) |
			  (pbm->bi.is_n_secure_os << 4) |
			  (pbm->bi.is_secure_os << 0));
	pi.s_dev_addr = pbm->bi.dbi.s_device_addr;
	pi.n_dev_addr = pbm->bi.dbi.n_device_addr;
	pi.sf_dev_addr = pbm->bi.dbi.sf_device_addr;
	pi.ensr_func = self_refresh_entry;
	pi.exsr_func = exit_self_refresh;

	pi.pmic_poweroff = pmic_poweroff;

	return smc_call(SIP_PLATFORM_LOAD, (int)&pi, 0, 0);
}
