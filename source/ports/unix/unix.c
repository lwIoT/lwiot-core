/*
 * E/STACK - UNIX port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <lwiot.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/time.h>

#include <lwiot/list.h>
#include <lwiot/log.h>
#include <lwiot/error.h>

time_t lwiot_utime(void)
{
	time_t rv;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	rv = (time_t) (tv.tv_sec * 1000000ULL);
	rv += tv.tv_usec;
	return rv;
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

int lwiot_thread_create(lwiot_thread_t *tp, thread_handle_t handle, void *arg)
{
	int rv;

	assert(tp);
	assert(handle);

	tp->arg = arg;
	tp->handle = handle;
	rv = pthread_create(&tp->tid, NULL, unix_thread_starter, tp);

	return rv;
}

int lwiot_thread_destroy(lwiot_thread_t *tp)
{
	assert(tp);

	pthread_join(tp->tid, NULL);
	tp->handle = NULL;
	tp->arg = NULL;

	return -EOK;
}

/*
 * MUTEX FUNCTIONS
 */

int lwiot_mutex_create(lwiot_mutex_t *mtx, const uint32_t flags)
{
	pthread_mutexattr_t attr;

	assert(mtx);
	pthread_mutexattr_init(&attr);

	if(flags & MTX_RECURSIVE) {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	} else {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	}

	pthread_mutex_init(&mtx->mtx, &attr);
	return -EOK;
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	assert(mtx);

	pthread_mutex_destroy(&mtx->mtx);
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
	usleep(us);
}

void lwiot_event_create(lwiot_event_t *event, int length)
{
	assert(event);

	event->size = length;
	event->length = 0;
	event->signalled = false;
	pthread_mutex_init(&event->mtx, NULL);
	pthread_cond_init(&event->cond, NULL);
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
	assert(++event->length < event->size);
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
	event->signalled = true;
	pthread_cond_signal(&event->cond);
	pthread_mutex_unlock(&event->mtx);
}

void lwiot_event_signal_irq(lwiot_event_t *event)
{
	/* ISR don't exist in UNIX userspace, which means that
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
}
