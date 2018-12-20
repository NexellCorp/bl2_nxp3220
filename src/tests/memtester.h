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
/* Baic Sizes */
static unsigned int y = 1U;

unsigned int rand_r(unsigned int *seedp)
{
	*seedp ^= (*seedp << 13);
	*seedp ^= (*seedp >> 17);
	*seedp ^= (*seedp << 5);

	return *seedp;
}

unsigned int rand(void)
{
	return rand_r(&y);
}

void srand(unsigned int seed)
{
	y = seed;
}
#if 0
#define rand32() ((unsigned int) rand() | ( (unsigned int) rand() << 16))
#define rand_ul() rand32()
#else
unsigned int rand_ul(void)
{
	return (rand() | rand() << 16);
//	return 1;
}
#endif

#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define CHECKERBOARD1 0x55555555
#define CHECKERBOARD2 0xaaaaaaaa
#define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))

/* Basic Types  */
typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;

struct test {
	char *name;
	int (*fp)(ulv*, ulv*, int);
};

union {
	unsigned char bytes[UL_LEN/8];
	ul val;
} mword8;

union {
	unsigned short u16s[UL_LEN/16];
	ul val;
} mword16;

#define TEST_NARROW_WRITES

/* Function declaration. */
int test_stuck_address(unsigned long volatile *bufa, int count);
int test_random_value(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_xor_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_sub_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_mul_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_div_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_or_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_and_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_seqinc_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_solidbits_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_checkerboard_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_blockseq_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_walkbits0_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_walkbits1_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_bitspread_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_bitflip_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
#ifdef TEST_NARROW_WRITES
int test_8bit_wide_random(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
int test_16bit_wide_random(unsigned long volatile *bufa, unsigned long volatile *bufb, int count);
#endif

 int standard_memtester(void);
void simple_memtest(void);

