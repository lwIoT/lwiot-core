/*
 * lwIoT - WIN32 port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <lwiot/types.h>
#include <lwiot/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DLL_EXPORT mutex {
	HANDLE mtx;
#define HAVE_MUTEX
} lwiot_mutex_t;

typedef struct DLL_EXPORT thread {
	char name[16];
	HANDLE tp;
	DWORD tid;
	void *arg;
	void(*handle)(void *param);
#define HAVE_THREAD
} lwiot_thread_t;

typedef struct DLL_EXPORT event {
	CRITICAL_SECTION cs;
	CONDITION_VARIABLE cond;
	bool signalled;
	int size,
		length;
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
