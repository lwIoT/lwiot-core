/*
 * lwIoT - System portability helper
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once
#define __PORT_H__

#include <stdlib.h>
#include <stdint.h>

CDECL
#ifndef HAVE_MUTEX
typedef void* lwiot_mutex_t;
#endif

#ifndef CONFIG_CONFIG_STANDALONE
#ifndef HAVE_THREAD
typedef void* lwiot_thread_t;
#endif

#ifndef HAVE_EVENT
typedef void* lwiot_event_t;
#endif

#ifndef HAVE_TIMER
typedef void* lwiot_timer_t;
#endif

#define MTX_RECURSIVE 1U

typedef void (*thread_handle_t)(void *arg);

struct lwiot_thread_attributes {
	const char *name;
	thread_handle_t handle;
	void *argument;

	int priority;
	size_t stacksize;
};

extern DLL_EXPORT lwiot_thread_t* lwiot_thread_create(thread_handle_t handle, const char *name, void *arg);
extern DLL_EXPORT lwiot_thread_t* lwiot_thread_create_raw(const struct lwiot_thread_attributes *attrs);
extern DLL_EXPORT int lwiot_thread_destroy(lwiot_thread_t *tp);
extern DLL_EXPORT void lwiot_thread_yield();
extern DLL_EXPORT void lwiot_thread_join(lwiot_thread_t *tp);

extern DLL_EXPORT lwiot_event_t* lwiot_event_create(int length);
extern DLL_EXPORT void  lwiot_event_destroy(lwiot_event_t *e);
extern DLL_EXPORT void lwiot_event_signal(lwiot_event_t *event);
extern DLL_EXPORT int lwiot_event_wait(lwiot_event_t *event, int tmo);
extern DLL_EXPORT void lwiot_event_signal_irq(lwiot_event_t *event);

#define TIMER_ONSHOT_FLAG 0x1U
extern DLL_EXPORT lwiot_timer_t* lwiot_timer_create(  const char *name, int ms,
	uint32_t flags, void *arg, void (*cb)(lwiot_timer_t *timer, void *arg));
extern DLL_EXPORT int lwiot_timer_start(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_destroy(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_stop(lwiot_timer_t *timer);
extern DLL_EXPORT void lwiot_timers_init(void);
extern DLL_EXPORT void lwiot_timers_destroy(void);
extern DLL_EXPORT bool lwiot_timer_is_running(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_set_period(lwiot_timer_t *timer, int ms);
extern DLL_EXPORT void lwiot_timer_reset(lwiot_timer_t* timer);
extern DLL_EXPORT time_t lwiot_timer_get_expiry(lwiot_timer_t *timer);
#else /* CONFIG_CONFIG_STANDALONE */
#define lwiot_timers_init()
#define lwiot_timers_destroy()
#endif /* CONFIG_CONFIG_STANDALONE */

extern DLL_EXPORT time_t lwiot_tick(void);
extern DLL_EXPORT time_t lwiot_tick_ms(void);

extern DLL_EXPORT void *lwiot_mem_alloc(size_t size);
extern DLL_EXPORT void *lwiot_mem_zalloc(size_t size);
extern DLL_EXPORT void lwiot_mem_free(void *ptr);
extern DLL_EXPORT void *lwiot_mem_realloc(void *ptr, size_t size);

extern DLL_EXPORT lwiot_mutex_t* lwiot_mutex_create(const uint32_t flags);
extern DLL_EXPORT int lwiot_mutex_destroy(lwiot_mutex_t *mtx);
extern DLL_EXPORT int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo);
extern DLL_EXPORT void lwiot_mutex_unlock(lwiot_mutex_t *mtx);

extern DLL_EXPORT void lwiot_sleep(int ms);

extern DLL_EXPORT int lwiot_hostname_to_ip(const char *host, uint32_t *addr);

extern DLL_EXPORT void enter_critical();
extern DLL_EXPORT void exit_critical();
extern DLL_EXPORT RAM_ATTR void lwiot_udelay(uint32_t us);

#ifdef CONFIG_STANDALONE
extern DLL_EXPORT void no_os_init();
#endif

#define FOREVER 0
CDECL_END
