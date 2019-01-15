/*
 * E/STACK - UNIX port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <lwiot_arch.h>

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <lwiot.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#include <sys/time.h>

#include <lwiot/util/list.h>
#include <lwiot/log.h>
#include <lwiot/error.h>

time_t lwiot_tick(void)
{
	time_t rv;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	rv = (time_t) (tv.tv_sec * 1000000ULL);
	rv += tv.tv_usec;
	return rv;
}

time_t lwiot_tick_ms(void)
{
	return lwiot_tick() / 1000ULL;
}

void lwiot_mem_free(void *ptr)
{
	free(ptr);
}

void *lwiot_mem_alloc(size_t size)
{
	return malloc(size);
}

void *lwiot_mem_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void *lwiot_mem_zalloc(size_t size)
{
	void *ptr = malloc(size);

	memset(ptr, 0, size);
	return ptr;
}

/*
 * THREAD FUNCTIONS
 */

static void *unix_thread_starter(void *arg)
{
	struct thread *tp;

	tp = (struct thread *)arg;
	tp->handle(tp->arg);

	return NULL;
}

void lwiot_thread_yield()
{
	usleep(1);
	sched_yield();
}

lwiot_thread_t* lwiot_thread_create_raw(const struct lwiot_thread_attributes *attrs)
{
	assert(attrs);
	return lwiot_thread_create(attrs->handle, attrs->name, attrs->argument);
}

lwiot_thread_t* lwiot_thread_create(thread_handle_t handle, const char *name, void *arg)
{
	lwiot_thread_t *tp;

	tp = lwiot_mem_zalloc(sizeof(*tp));
	assert(tp);
	assert(handle);

	tp->arg = arg;
	tp->handle = handle;
	pthread_create(&tp->tid, NULL, unix_thread_starter, tp);

	return tp;
}

int lwiot_thread_destroy(lwiot_thread_t *tp)
{
	assert(tp);

	pthread_join(tp->tid, NULL);
	tp->handle = NULL;
	tp->arg = NULL;
	lwiot_mem_free(tp);

	return -EOK;
}

/*
 * MUTEX FUNCTIONS
 */

lwiot_mutex_t* lwiot_mutex_create(const uint32_t flags)
{
	lwiot_mutex_t *mtx;
	pthread_mutexattr_t attr;

	mtx = lwiot_mem_zalloc(sizeof(*mtx));
	assert(mtx);
	pthread_mutexattr_init(&attr);

	if(flags & MTX_RECURSIVE) {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	} else {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	}

	pthread_mutex_init(&mtx->mtx, &attr);
	return mtx;
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	assert(mtx);

	pthread_mutex_destroy(&mtx->mtx);
	lwiot_mem_free(mtx);
	return -EOK;
}

int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo)
{
	assert(mtx);
	UNUSED(tmo);

	pthread_mutex_lock(&mtx->mtx);
	return -EOK;
}

void lwiot_mutex_unlock(lwiot_mutex_t *mtx)
{
	assert(mtx);
	pthread_mutex_unlock(&mtx->mtx);
}

void lwiot_sleep(int ms)
{
	time_t us;

	us = ms * 1000;
	usleep((__useconds_t) us);
}

lwiot_event_t* lwiot_event_create(int length)
{
	lwiot_event_t *event;

	event = lwiot_mem_zalloc(sizeof(*event));
	assert(event);

	event->size = length;
	event->length = 0;
	event->signalled = false;

	pthread_mutex_init(&event->mtx, NULL);
	pthread_cond_init(&event->cond, NULL);

	return event;
}

#define NANOSECOND (1000 * 1000 * 1000)

static void timespec_create_from_tmo(struct timespec *spec, int tmo)
{
	struct timeval tv;
	time_t tvsec;

	assert(spec);
	gettimeofday(&tv, NULL);
	time(&tvsec);
	spec->tv_sec = tvsec + (tmo / 1000);
	spec->tv_nsec = tv.tv_usec * 1000 + (1000 * 1000 * (tmo % 1000));
	spec->tv_sec += spec->tv_nsec / (NANOSECOND);
	spec->tv_nsec %= NANOSECOND;
}

int lwiot_event_wait(lwiot_event_t *event, int tmo)
{
	struct timespec timeout;

	assert(event);

	pthread_mutex_lock(&event->mtx);
	event->length++;
	assert(event->length < event->size);

	while(!event->signalled) {
		if(tmo == FOREVER) {
			pthread_cond_wait(&event->cond, &event->mtx);
		} else {
			timespec_create_from_tmo(&timeout, tmo);
			if(pthread_cond_timedwait(&event->cond, &event->mtx, &timeout)) {
				event->length--;
				pthread_mutex_unlock(&event->mtx);
				return -ETMO;
			}
		}

	}
	
	event->length--;
	event->signalled = false;
	pthread_mutex_unlock(&event->mtx);
	return -EOK;
}

void lwiot_event_signal(lwiot_event_t *event)
{
	assert(event);
	pthread_mutex_lock(&event->mtx);

	if(event->length == 0) {
		pthread_mutex_unlock(&event->mtx);
		return;
	}

	event->signalled = true;
	pthread_cond_signal(&event->cond);
	pthread_mutex_unlock(&event->mtx);
}

void lwiot_event_signal_irq(lwiot_event_t *event)
{
	/* ISR don't exist in UNIX user space, which means that
	   something or somebody fucked up. Let them know. */
	print_dbg("lwiot_event_signal_irq() called on Unix port!\n");
	lwiot_event_signal(event);
}

void lwiot_event_destroy(lwiot_event_t *e)
{
	assert(e);

	pthread_cond_destroy(&e->cond);
	pthread_mutex_destroy(&e->mtx);
	e->signalled = false;
	e->size = 0;

	lwiot_mem_free(e);
}

void enter_critical()
{ }

void exit_critical()
{ }
