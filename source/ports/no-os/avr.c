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

void enter_critical()
{
	sei();
}

void exit_critical()
{
	cli();
}
