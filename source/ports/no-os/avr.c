/*
 * ATmega no RTOS definitions.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <avr/io.h>
#include <avr/interrupt.h>


#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <util/delay.h>

void lwiot_mem_free(void *ptr)
{
	free(ptr);
}

void *lwiot_mem_alloc(size_t size)
{
	return malloc(size);
}

void *lwiot_mem_zalloc(size_t size)
{
	void *ptr = malloc(size);

	memset(ptr, 0, size);
	return ptr;
}

void __attribute__((naked)) enter_critical()
{
	__asm__ __volatile__(
		"in __tmp_reg__, __SREG__"	"\n\t"
		"cli"						"\n\t"
		"push __tmp_reg__"
		:
		:
	);
}

void __attribute__((naked)) exit_critical()
{
	__asm__ __volatile__(
		"pop __tmp_reg__" "\n\t"
		"out __SREG__, __tmp_reg__" "\n\t"
		:
		:
	);
}

void lwiot_sleep(int ms)
{
	for(int num = 0; num < ms; num++)
		_delay_ms(1);
}
