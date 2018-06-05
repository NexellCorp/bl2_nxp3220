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
#ifndef __ASM_H__
#define __ASM_H__

#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#define SYMBOL_NAME_STR(X)	#X
#define SYMBOL_NAME(X)		X
#define SYMBOL_NAME_LABEL(X)	X:

#ifndef __ALIGN
#define __ALIGN .align		4
#endif

#ifndef __ALIGN_STR
#define __ALIGN_STR		".align 4"
#endif

#define ALIGN			__ALIGN
#define ALIGN_STR		__ALIGN_STR

#define EXPORT(name)			\
	.globl SYMBOL_NAME(name) ASM_NL

#define LENTRY(name) 			\
	ALIGN ASM_NL			\
	SYMBOL_NAME_LABEL(name)

#define ENTRY(name)			\
	.globl SYMBOL_NAME(name) ASM_NL \
	LENTRY(name)

#define WEAK(name)			\
	.weak SYMBOL_NAME(name) ASM_NL	\
	LENTRY(name)

#ifndef END
#define END(name)			\
	.size name, .-name
#endif

#ifndef ENDPROC
#define ENDPROC(name)			\
	.type name STT_FUNC ASM_NL	\
	END(name)
#endif

#endif
