/*
 * ATmega no RTOS definitions.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include "lwiot_arch.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define raw_enter_critical() \
	__asm__ __volatile__( \
		"in __tmp_reg__, __SREG__"	"\n\t" \
		"cli"						"\n\t" \
		"push __tmp_reg__"          "\n\t"\
		: \
		: \
	);

#define raw_exit_critical() \
	__asm__ __volatile__( \
		"pop __tmp_reg__" "\n\t" \
		"out __SREG__, __tmp_reg__" "\n\t" \
		: \
		: \
	);

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

void *lwiot_mem_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void lwiot_sleep(int ms)
{
	for(int num = 0; num < ms; num++)
		_delay_ms(1);
}

static volatile uint64_t sys_tick = 0ULL;

void no_os_init(void)
{
#if F_CPU == 16000000
	OCR0A = 250;
#elif F_CPU == 8000000
	OCR0A = 125;
#endif

	TIMSK0 = _BV(TOIE0);
	TCCR0A = _BV(WGM00) | _BV(WGM01);
	TCCR0B = _BV(WGM02) | _BV(CS00) | _BV(CS01);
}

time_t lwiot_tick(void)
{
	return sys_tick * 1000ULL;
}

time_t lwiot_tick_ms(void)
{
	return sys_tick;
}

#ifndef OLD_STYLE_TIMERS
ISR(TIMER0_OVF_vect)
{
	sys_tick += 1ULL;
}
#endif

lwiot_mutex_t* lwiot_mutex_create(const uint32_t flags)
{
	lwiot_mutex_t* mtx;

	mtx = lwiot_mem_zalloc(sizeof(*mtx));
	assert(mtx);
	mtx->lock = 0;

	return mtx;
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	assert(mtx);
	lwiot_mem_free(mtx);
	return -EOK;
}

int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo)
{
	uint8_t value = 1;

	while(value) {
		raw_enter_critical();
		value = mtx->lock;
		raw_exit_critical();

		lwiot_sleep(1);
	}

	raw_enter_critical();
	mtx->lock = 1;
	raw_exit_critical();

	return -EOK;
}

void lwiot_mutex_unlock(lwiot_mutex_t *mtx)
{
	raw_enter_critical();
	mtx->lock = 0;
	raw_exit_critical();
}
