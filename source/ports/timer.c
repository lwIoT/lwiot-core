/*
 * lwIoT timer implementation
 * 
 * Author: Michel Megens
 * Date:   16/01/2018
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/error.h>
#include <lwiot/list.h>
#include <lwiot/log.h>

static struct list_head timers = STATIC_INIT_LIST_HEAD(timers);
static lwiot_mutex_t timer_lock;
static lwiot_thread_t timer_thread;
static volatile bool running;

static inline void timers_lock(void)
{
	lwiot_mutex_lock(&timer_lock, 0);
}

static inline void timers_unlock(void)
{
	lwiot_mutex_unlock(&timer_lock);
}

static void timer_thread_handle(void *arg)
{
	struct list_head *entry, *tmp;
	lwiot_timer_t *timer;
	time_t now;

	UNUSED(arg);

	while(true) {
		timers_lock();

		if(unlikely(!running)) {
			timers_unlock();
			break;
		}

		now = lwiot_tick();
		list_for_each_safe(entry, tmp, &timers) {
			timer = list_entry(entry, struct timer, entry);
			if(now >= timer->expiry) {
				timers_unlock();
				timer->handle(timer, timer->arg);
				timers_lock();

				if(timer->state == TIMER_RUNNING && timer->oneshot) {
					list_del(entry);
					timer->state = TIMER_STOPPED;
				} else {
					timer->expiry = now + timer->tmo;
				}
			}
		}
		timers_unlock();

		lwiot_sleep(1);
	}
}

void lwiot_timers_init(void)
{
	const char *tmp = "timer-thread";

	lwiot_mutex_create(&timer_lock, 0);
	memset((void*)timer_thread.name, 0, sizeof(timer_thread.name));
	memcpy((void*)timer_thread.name, tmp, strlen(tmp));
	timers_lock();
	running = true;
	timers_unlock();
	lwiot_thread_create(&timer_thread, timer_thread_handle, NULL);
}

void lwiot_timers_destroy(void)
{
	timers_lock();
	running = false;
	timers_unlock();
	lwiot_thread_destroy(&timer_thread);
	lwiot_mutex_destroy(&timer_lock);
}

void lwiot_timer_create(lwiot_timer_t *timer, const char *name, int ms,
	uint32_t flags, void *arg, void (*cb)(lwiot_timer_t *timer, void *arg))
{
	timer->handle = cb;
	timer->tmo = ms * 1000U;
	timer->arg = arg;

	if(flags & TIMER_ONSHOT_FLAG)
		timer->oneshot = true;
	else
		timer->oneshot = false;

	list_head_init(&timer->entry);
	timer->state = TIMER_CREATED;
}

int lwiot_timer_start(lwiot_timer_t *timer)
{
	if(timer->state == TIMER_RUNNING)
		return -EINVALID;

	timers_lock();
	timer->expiry = lwiot_tick() + timer->tmo;
	timer->state = TIMER_RUNNING;
	list_add(&timer->entry, &timers);
	timers_unlock();

	return -EOK;
}

int lwiot_timer_stop(lwiot_timer_t *timer)
{
	timers_lock();
	if(timer->state != TIMER_RUNNING) {
		timers_unlock();
		return -EINVALID;
	}

	timer->state = TIMER_STOPPED;
	list_del(&timer->entry);
	timers_unlock();

	return -EOK;
}

int lwiot_timer_destroy(lwiot_timer_t *timer)
{
	lwiot_timer_stop(timer);
	return -EOK;
}

bool lwiot_timer_is_running(lwiot_timer_t *timer)
{
	assert(timer);
	return timer->state == TIMER_RUNNING;
}

int lwiot_timer_set_period(lwiot_timer_t *timer, int ms)
{
	assert(timer);
	assert(ms);

	timers_lock();
	if(timer->state != TIMER_CREATED && timer->state != TIMER_RUNNING) {
		timers_unlock();
		return -EINVALID;
	}

	timer->tmo = ms * 1000U;
	timers_unlock();

	return -EOK;
}

time_t lwiot_timer_get_expiry(lwiot_timer_t *timer)
{
	return timer->expiry /= 1000U;
}
