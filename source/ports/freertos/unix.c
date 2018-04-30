/*
 * E/STACK - RTOS unix port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <lwiot.h>

#include <sys/time.h>

time_t lwiot_tick(void)
{
	time_t rv;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	rv = (time_t) (tv.tv_sec * 1000000ULL);
	rv += tv.tv_usec;
	return rv;
}

void __maybe vApplicationMallocFailedHook(void)
{
	fprintf(stderr, "Failed to allocate memory!\n");
}

void __maybe vApplicationStackOverflowHook(TaskHandle_t handle, char *name)
{
	UNUSED(handle);
	fprintf(stderr, "Stack overflow on %s\n", name);
}
