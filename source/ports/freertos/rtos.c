/*
 * E/STACK - RTOS unix port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <lwiot.h>

#ifdef ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#else
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#endif

#include <lwiot/log.h>
#include <lwiot/error.h>

static inline TickType_t vPortTickToMs(TickType_t ticks)
{
	return ticks * portTICK_PERIOD_MS;
}

static void vPortTaskStarter(void *arg)
{
	struct thread *tp;

	tp = (struct thread *)arg;
	tp->handle(tp->arg);
	tp->task = NULL;
	vTaskDelete(NULL);
}

void *lwiot_mem_alloc(size_t size)
{
	return pvPortMalloc(size);
}

void *lwiot_mem_zalloc(size_t size)
{
	void *ptr = lwiot_mem_alloc(size);

	memset(ptr, 0, size);
	return ptr;
}

extern void vApplicationMallocFailedHook( void );
void *lwiot_mem_realloc(void *ptr, size_t size)
{
	void *retval;

	vTaskSuspendAll();
	retval = realloc(ptr, size);
	xTaskResumeAll();


#if configUSE_MALLOC_FAILED_HOOK == 1
	if(!retval) 
		vApplicationMallocFailedHook();
#endif

	return retval;
}

void lwiot_mem_free(void *ptr)
{
	vPortFree(ptr);
}

int lwiot_thread_create_raw(lwiot_thread_t *tp, const struct lwiot_thread_attributes *attrs)
{
	BaseType_t bt;

	assert(tp);
	assert(attrs);

	tp->handle = attrs->handle;
	tp->arg = attrs->argument;

#ifdef ESP32
	BaseType_t coreid = esp32_get_next_coreid();
	bt = xTaskCreatePinnedToCore(
		vPortTaskStarter, tp->name,
		attrs->stacksize, tp,
		attrs->priority, &tp->task, coreid
	);
#else
	bt = xTaskCreate(
		vPortTaskStarter, tp->name,
		attrs->stacksize, tp,
		attrs->priority, &tp->task
	);
#endif

	if(bt == pdPASS)
		return -EOK;
	
	print_dbg("Could not create task!\n");
	return -EINVALID;
}

#define STACK_DEPTH CONFIG_STACK_SIZE
#define TASK_PRIO CONFIG_TASK_PRIO
int lwiot_thread_create(lwiot_thread_t *tp, thread_handle_t handle, void *arg)
{
	struct lwiot_thread_attributes attrs;

	attrs.priority = TASK_PRIO;
	attrs.stacksize = STACK_DEPTH;
	attrs.handle = handle;
	attrs.argument = arg;

	return lwiot_thread_create_raw(tp, &attrs);
}

int lwiot_thread_destroy(lwiot_thread_t *tp)
{
	assert(tp);

	if(tp->task) {
		vTaskDelete(tp->task);
		tp->task = NULL;
	}

	return -EOK;
}

void lwiot_thread_yield()
{
	taskYIELD();
}

int lwiot_mutex_create(lwiot_mutex_t *mtx, const uint32_t flags)
{
	assert(mtx);
	mtx->sem = xSemaphoreCreateMutex();

	if(!mtx->sem)
		return -EINVALID;

	mtx->recursive = (flags & MTX_RECURSIVE) != 0;
	return -EOK;
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	assert(mtx);
	assert(mtx->sem);

	vQueueDelete(mtx->sem);
	return -EOK;
}

int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo)
{
	bool rv;
	TickType_t ms;

	assert(mtx);
	assert(mtx->sem);

	if(tmo) {
		ms = tmo / portTICK_RATE_MS;

		if(mtx->recursive)
			rv = (bool) (xSemaphoreTakeRecursive(mtx->sem, ms) == pdTRUE);
		else
			rv = (bool) (xSemaphoreTake(mtx->sem, ms) == pdTRUE);
	} else {
		if(mtx->recursive)
			while(xSemaphoreTakeRecursive(mtx->sem, portMAX_DELAY) != pdTRUE);
		else
			while(xSemaphoreTake(mtx->sem, portMAX_DELAY) != pdTRUE);

		rv = true;
	}

	return rv ? -EOK : -EINVALID;
}

void lwiot_mutex_unlock(lwiot_mutex_t *mtx)
{
	assert(mtx);
	assert(mtx->sem);

	if(mtx->recursive)
		xSemaphoreGiveRecursive(mtx->sem);
	else
		xSemaphoreGive(mtx->sem);
}

void lwiot_sleep(int ms)
{
	vTaskDelay(ms / portTICK_PERIOD_MS);
}

void lwiot_event_create(lwiot_event_t *event, int length)
{
	event->evq = xQueueCreate(length, sizeof(void*));
}

int lwiot_event_wait(lwiot_event_t *event, int tmo)
{
	lwiot_event_t *ev;
	BaseType_t bt;
	int rv;

	if(tmo == FOREVER) {
		while(xQueueReceive(event->evq, (void*)&ev, portMAX_DELAY) != pdTRUE);
		rv = -EOK;
	} else {
		bt = xQueueReceive(event->evq, (void*)&ev, tmo / portTICK_PERIOD_MS);
		rv = bt == pdTRUE ? -EOK : -ETMO;
	}

#ifndef NDEBUG
	if(rv == -EOK)
		assert(ev == event);
#endif

	return rv;
}

void lwiot_event_signal_irq(lwiot_event_t *event)
{
	xQueueSendFromISR(&event->evq, &event, (TickType_t) 0);
}

void lwiot_event_signal(lwiot_event_t *event)
{
	xQueueSend(event->evq, &event, (TickType_t) 0);
}

void lwiot_event_destroy(lwiot_event_t *e)
{
	vQueueDelete(e->evq);
}

/*
 * TIMER API WRAPPERS
 */

void lwiot_timers_init(void)
{
}

void lwiot_timers_destroy(void)
{
}

static void vTimerCallbackHook(TimerHandle_t xTimer)
{
	lwiot_timer_t *tmr;

	tmr = pvTimerGetTimerID(xTimer);
	tmr->handle(tmr, tmr->arg);

	if(tmr->oneshot) {
		tmr->state = TIMER_STOPPED;
	} else {
		tmr->expiry = lwiot_tick_ms() + tmr->period;
	}
}

void lwiot_timer_create(lwiot_timer_t *timer, const char *name, int ms,
	uint32_t flags, void *arg, void (*cb)(lwiot_timer_t *timer, void *arg))
{
	timer->handle = cb;
	timer->period = ms / portTICK_PERIOD_MS;
	timer->arg = arg;

	if(flags & TIMER_ONSHOT_FLAG)
		timer->oneshot = true;
	else
		timer->oneshot = false;

	timer->timer = xTimerCreate(name, ms / portTICK_PERIOD_MS, !timer->oneshot, timer, vTimerCallbackHook);
	timer->created = true;
	timer->state = TIMER_CREATED;
}

int lwiot_timer_start(lwiot_timer_t *timer)
{
	BaseType_t bt;

	if(timer->state == TIMER_RUNNING || !timer->created)
		return -EINVALID;

	bt = pdFAIL;
	timer->state = TIMER_RUNNING;

	while(bt != pdPASS)
		bt = xTimerStart(timer->timer, 0);

	timer->expiry = lwiot_tick_ms() + vPortTickToMs(timer->period);
	return -EOK;
}

int lwiot_timer_stop(lwiot_timer_t *timer)
{
	BaseType_t bt;

	if(timer->state != TIMER_RUNNING || !timer->created)
		return -EINVALID;

	bt = pdFAIL;
	while(bt != pdPASS)
		bt = xTimerStop(timer->timer, 0);

	timer->state = TIMER_STOPPED;
	return -EOK;
}

int lwiot_timer_destroy(lwiot_timer_t *timer)
{
	BaseType_t bt = pdFAIL;

	if(!timer->created)
		return -EINVALID;

	lwiot_timer_stop(timer);

	while(bt != pdPASS)
		bt = xTimerDelete(timer->timer, 0);

	timer->state = TIMER_STOPPED;
	timer->created = false;
	return -EOK;
}

bool lwiot_timer_is_running(lwiot_timer_t *timer)
{
	return timer->state == TIMER_RUNNING;
}

int lwiot_timer_set_period(lwiot_timer_t *timer, int ms)
{
	BaseType_t bt = pdFAIL;

	if(!timer->created)
		return -EINVALID;

	while(bt != pdPASS)
		bt = xTimerChangePeriod(timer->timer, ms / portTICK_PERIOD_MS, 0);

	timer->expiry = lwiot_tick_ms() + vPortTickToMs(timer->period);
	return -EOK;
}

time_t lwiot_timer_get_expiry(lwiot_timer_t *timer)
{
	return timer->expiry;
}

#ifdef CONFIG_NO_SYS
time_t lwiot_tick(void)
{
	time_t ticks;

	ticks = xTaskGetTickCount();
	ticks *= (double)1/configTICK_RATE_HZ * 1000ULL * 1000ULL;
	return ticks;
}

time_t lwiot_tick_ms(void)
{
	time_t ticks;

	ticks = xTaskGetTickCount();
	ticks *= (double)1/configTICK_RATE_HZ * 1000ULL;
	return ticks;
}
#endif
