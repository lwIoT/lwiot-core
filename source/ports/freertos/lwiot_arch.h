/*
 * lwIoT - WIN32 port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#ifndef __PORT_H__
#error "Do not include this file directly, use \"ports.h\" instead!"
#endif

#ifndef __RTOS_PORT_H__
#define __RTOS_PORT_H__

#include <lwiot_opts.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

#ifndef HAVE_TASKHANDLE_T
typedef xTaskHandle TaskHandle_t;
#endif

#ifndef HAVE_QUEUEHANDLE_T
typedef xQueueHandle QueueHandle_t;
#endif

#ifndef HAVE_SEMAPHOREHANDLE_T
typedef xSemaphoreHandle SemaphoreHandle_t;
#endif

#ifndef HAVE_TIMERHANDLE_T
typedef xTimerHandle TimerHandle_t;
#endif

#ifndef HAVE_TICKTYPE_T
#define TickType_t portTickType
#define BaseType_t portBASE_TYPE
#endif

#ifndef portTICK_PERIOD_MS
#define portTICK_PERIOD_MS portTICK_RATE_MS
#endif

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

#include <lwiot/compiler.h>
#include <lwiot/lwiot.h>

typedef struct thread {
	const char *name;
	void *arg;
	void (*handle)(void *arg);
	TaskHandle_t task;
#define HAVE_THREAD
} lwiot_thread_t;

typedef struct mutex {
	SemaphoreHandle_t sem;
	bool recursive;
#define HAVE_MUTEX
} lwiot_mutex_t;

typedef struct event {
	QueueHandle_t evq;
#define HAVE_EVENT
} lwiot_event_t;

typedef struct timer {
	TimerHandle_t timer;
	TickType_t period;
	time_t expiry;
	void *arg;
	void (*handle)(struct timer *timer, void *arg);
	bool oneshot;
	timer_state_t state;
	bool created;
#define HAVE_TIMER
} lwiot_timer_t;

#include "delay.h"
#endif
