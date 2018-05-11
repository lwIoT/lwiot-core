/*
 * AVR ATmega specific functions.
 *
 * @author Michel Megens
 * @author Arduino Community
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

void __attribute__((optimize("Os")))  udelay(uint32_t us)
{
#if F_CPU >= 24000000L
	if(!us)
		return;

	us *= 6;
	us -= 5;
#elif F_CPU >= 20000000L
	__asm__ __volatile__(
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop");
	if(us <= 1)
		return;

	us = (us << 2) + us;
	us -= 7;

#elif F_CPU >= 16000000L
	if(us <= 1)
		return;

	us <<= 2;
	us -= 5;

#elif F_CPU >= 12000000L
	if(us <= 1)
		return;

	us = (us << 1) + us;
	us -= 5;

#elif F_CPU >= 8000000L
	if(us <= 2)
		return;

	us <<= 1;
	us -= 4;

#else
	if(us <= 16)
		return;

	if(us <= 25)
		return;

	us -= 22;
	us >>= 2;
#endif

	__asm__ __volatile__(
		"1: sbiw %0,1" "\n\t"
		"brne 1b" : "=w" (us) : "0" (us)
	);
}
