/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" void udelay(uint32_t us);
extern "C" void extmem_enable(int pin);
#else
extern void udelay(uint32_t us);
extern void extmem_enable(int pin);
#endif

#define lwiot_udelay(__us) udelay((uint32_t)__us)

#define enter_critical() \
	__asm__ __volatile__( \
		"in __tmp_reg__, __SREG__"	"\n\t" \
		"cli"						"\n\t" \
		"push __tmp_reg__"          "\n\t"\
		: \
		: \
	);

#define exit_critical() \
	__asm__ __volatile__( \
		"pop __tmp_reg__" "\n\t" \
		"out __SREG__, __tmp_reg__" "\n\t" \
		: \
		: \
	);
