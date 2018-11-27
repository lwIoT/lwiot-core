/*
 * E/STACK - WIN32 port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#ifndef __PORT_H__
#error "Do not include this file directly, use \"ports.h\" instead!"
#endif

#ifndef __UNIX_PORT_H__
#define __UNIX_PORT_H__

#include <lwiot/types.h>
#include <lwiot/list.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef DLL_EXPORT struct mutex {
	pthread_mutex_t mtx;
#define HAVE_MUTEX
} lwiot_mutex_t;

typedef DLL_EXPORT struct thread {
	char name[16];
	pthread_t tid;
	void(*handle)(void *arg);
	void  *arg;
#define HAVE_THREAD
} lwiot_thread_t;

typedef DLL_EXPORT struct event {
	pthread_mutex_t mtx;
	pthread_cond_t cond;
	bool signalled;
	int length;
	int size;
#define HAVE_EVENT
} lwiot_event_t;

typedef DLL_EXPORT struct timer {
	struct list_head entry;
	void (*handle)(struct timer *timer, void *arg);
	bool oneshot;
	time_t expiry;
	int tmo;
	void *arg;
	timer_state_t state;
#define HAVE_TIMER
} lwiot_timer_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
