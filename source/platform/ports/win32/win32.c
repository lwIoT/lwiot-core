/*
 * lwIoT - WIN32 port
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 *
 * This file contains a windows port of the lwIoT system handles. These 
 * handles are wrappers around WINAPI. If you dislike WINAPI as much as I
 * do, get yourself a bucket now, puking is inevitable. YOU HAVE BEEN WARNED.
 */

#include "lwiot_arch.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/error.h>

#include <WinSock2.h>
#include <Windows.h>

#define WIN32_EPOCH_ADJUSTMENT 11644473600000000ULL
time_t lwiot_tick(void)
{
	time_t rv;
	FILETIME ft;

	GetSystemTimeAsFileTime(&ft);
	rv = ft.dwHighDateTime;
	rv <<= 32;
	rv |= ft.dwLowDateTime;
	rv /= 10;

	return rv - WIN32_EPOCH_ADJUSTMENT;
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

static DWORD WINAPI EStackThreadStarter(LPVOID lpParam)
{
	lwiot_thread_t *tp;

	tp = (lwiot_thread_t*)lpParam;
	tp->handle(tp->arg);
	return TRUE;
}

lwiot_thread_t* lwiot_thread_create_raw(const struct lwiot_thread_attributes *attrs)
{
	return lwiot_thread_create(attrs->handle, attrs->name, attrs->argument);
}

lwiot_thread_t* lwiot_thread_create(thread_handle_t handle, const char *name, void *arg)
{
	lwiot_thread_t *tp;

	tp = lwiot_mem_zalloc(sizeof(*tp));
	assert(tp);
	assert(handle);

	tp->handle = handle;
	tp->arg = arg;

	memset(tp->name, 0, sizeof(tp->name));
	memcpy(tp->name, name, strlen(name));

	tp->tp = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)EStackThreadStarter,
		tp,
		0,
		&tp->tid
	);

	if(!tp->tp) {
		print_dbg("Couldn't create thread!\n");
		return NULL;
	}

	return tp;
}

int lwiot_thread_destroy(lwiot_thread_t *tp)
{
	assert(tp);

	WaitForSingleObject(tp->tp, INFINITE);
	CloseHandle(tp->tp);
	tp->tp = NULL;
	tp->arg = NULL;
	tp->tid = 0;
	lwiot_mem_free(tp);

	return -EOK;
}

/*
 * MUTEX FUNCTIONS
 */

lwiot_mutex_t* lwiot_mutex_create(const uint32_t flags)
{
	lwiot_mutex_t *mtx;

	mtx = lwiot_mem_zalloc(sizeof(*mtx));
	assert(mtx);

	if(flags & MTX_RECURSIVE) {
		print_dbg("Recursive mutexes not supported on this platform!\n");
	}

	mtx->mtx = CreateMutex(NULL, FALSE, NULL);
	if(!mtx->mtx) {
		print_dbg("Couldn't create mutex object\n");
		return NULL;
	}

	return mtx;
}

void lwiot_thread_yield()
{
	SwitchToThread();
}

int lwiot_mutex_destroy(lwiot_mutex_t *mtx)
{
	assert(mtx);

	CloseHandle(mtx->mtx);
	lwiot_mem_free(mtx);
	return -EOK;
}

int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo)
{
	DWORD result;
	int rv;

	if(tmo)
		result = WaitForSingleObject(mtx->mtx, tmo);
	else
		result = WaitForSingleObject(mtx->mtx, INFINITE);

	switch(result) {
	case WAIT_OBJECT_0:
		rv = -EOK;
		break;

	case WAIT_TIMEOUT:
		print_dbg("Mutex unlock timeout!\n");
		rv = -EINVALID;
		break;

	case WAIT_ABANDONED:
	default:
		print_dbg("Unable to lock mutex! (Error code: %u)\n", result);
		rv = -EINVALID;
		break;
	}

	return rv;
}

void lwiot_mutex_unlock(lwiot_mutex_t *mtx)
{
	if(!ReleaseMutex(mtx->mtx))
		print_dbg("Unable to release mutex!\n");
}

void lwiot_sleep(int ms)
{
	Sleep(ms);
}

/*
 * EVENT / WAIT QUEUE HANDLING
 */

lwiot_event_t* lwiot_event_create(int length)
{
	lwiot_event_t* event;

	event = lwiot_mem_zalloc(sizeof(*event));
	assert(event);

	event->size = length;
	event->length = 0;
	event->signalled = false;
	InitializeConditionVariable(&event->cond);
	InitializeCriticalSection(&event->cs);
	return event;
}

void lwiot_event_destroy(lwiot_event_t *e)
{
	assert(e);

	WakeAllConditionVariable(&e->cond);
	DeleteCriticalSection(&e->cs);

	e->size = 0;
	e->length = 0;
	e->signalled = false;

	lwiot_mem_free(e);
}

void lwiot_event_signal(lwiot_event_t *event)
{
	EnterCriticalSection(&event->cs);

	if(event->length == 0) {
		LeaveCriticalSection(&event->cs);
		return;
	}

	event->signalled = true;
	LeaveCriticalSection(&event->cs);

	WakeConditionVariable(&event->cond);
}

void lwiot_event_signal_irq(lwiot_event_t *event)
{
	/* ISR don't exist in UNIX userspace, which means that
	   something or somebody fucked up. Let them know. */
	print_dbg("lwiot_event_signal_irq() called on Win32 port!\n");
	lwiot_event_signal(event);
}

int lwiot_event_wait(lwiot_event_t *event, int tmo)
{
	BOOL rv;
	assert(event);

	EnterCriticalSection(&event->cs);
	event->length++;
	assert(event->length < event->size);

	if(tmo == FOREVER)
		tmo = INFINITE;

	rv = TRUE;
	while(!event->signalled) {
		rv = SleepConditionVariableCS(&event->cond, &event->cs, tmo);
		if(tmo != INFINITE)
			break;
	}

	event->length--;

	if(rv)
		event->signalled = false;
	LeaveCriticalSection(&event->cs);

	return (rv) ? -EOK : -ETMO;
}

void enter_critical()
{ }

void exit_critical()
{ }
