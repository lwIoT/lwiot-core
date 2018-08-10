/*
 * ATmega no RTOS definitions.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#include <avr/io.h>
#include <avr/interrupt.h>
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

int lwiot_mutex_create(lwiot_mutex_t *mtx, const uint32_t flags)
{
	mtx->lock = 0;
	return -EOK;
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	return -EOK;
}

int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo)
{
	uint8_t value = 1;

	while(value) {
		enter_critical();
		value = mtx->lock;
		exit_critical();

		lwiot_sleep(1);
	}

	enter_critical();
	mtx->lock = 1;
	exit_critical();

	return -EOK;
}

void lwiot_mutex_unlock(lwiot_mutex_t *mtx)
{
	enter_critical();
	mtx->lock = 0;
	exit_critical();
}
