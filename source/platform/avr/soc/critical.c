/*
 * Critical section definitions for ATmega AVR.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>
#include <assert.h>

#include <lwiot/types.h>
#include <lwiot/compiler-gcc.h>

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int depth = 0;
static uint8_t flags = 0;

#define IBIT (1<<7)

void enter_critical()
{
	uint8_t tmp;

	tmp = SREG;
	cli();
	depth += 1;

	if(!flags)
		flags = tmp;
}

void exit_critical()
{
	bool irqs = (flags & IBIT) != 0;

	assert(depth > 0);
	depth -= 1;
	if(!depth) {
		flags = 0;

		if(irqs)
			sei();
	}
}
