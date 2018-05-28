/*
 * lwIoT - System portability helper
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#ifndef __PORT_H__
#define __PORT_H__

#include <stdlib.h>
#include <stdint.h>
#include <cpu.h>
#include <lwiot_arch.h>

#ifndef lwiot_udelay
#error "Microsecond delay not defined!"
#endif

CDECL
#ifndef CONFIG_NO_OS
#ifndef HAVE_MUTEX
#error "Missing mutex definition!"
#endif
#ifndef HAVE_THREAD
#error "Missing thread definition!"
#endif

#ifndef HAVE_EVENT
#error "Missing event definition!"
#endif

#ifndef HAVE_TIMER
#error "Missing timer definition!"
#endif

#define MTX_RECURSIVE 1U

typedef void (*thread_handle_t)(void *arg);
extern DLL_EXPORT time_t lwiot_tick(void);

extern DLL_EXPORT int lwiot_thread_create(lwiot_thread_t *tp, thread_handle_t handle, void *arg);
extern DLL_EXPORT int lwiot_thread_destroy(lwiot_thread_t *tp);

extern DLL_EXPORT int lwiot_mutex_create(lwiot_mutex_t *mtx, const uint32_t flags);
extern DLL_EXPORT int lwiot_mutex_destroy(lwiot_mutex_t *mtx);
extern DLL_EXPORT int lwiot_mutex_lock(lwiot_mutex_t *mtx, int tmo);
extern DLL_EXPORT void lwiot_mutex_unlock(lwiot_mutex_t *mtx);
extern DLL_EXPORT void lwiot_sleep(int ms);

extern DLL_EXPORT void lwiot_event_create(lwiot_event_t *event, int length);
extern DLL_EXPORT void lwiot_event_destroy(lwiot_event_t *e);
extern DLL_EXPORT void lwiot_event_signal(lwiot_event_t *event);
extern DLL_EXPORT int lwiot_event_wait(lwiot_event_t *event, int tmo);
extern DLL_EXPORT void lwiot_event_signal_irq(lwiot_event_t *event);

#define TIMER_ONSHOT_FLAG 0x1U
extern DLL_EXPORT void lwiot_timer_create(lwiot_timer_t *timer, const char *name, int ms,
	uint32_t flags, void *arg, void (*cb)(lwiot_timer_t *timer, void *arg));
extern DLL_EXPORT int lwiot_timer_start(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_destroy(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_stop(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_destroy(lwiot_timer_t *timer);
extern DLL_EXPORT void lwiot_timers_init(void);
extern DLL_EXPORT void lwiot_timers_destroy(void);
extern DLL_EXPORT bool lwiot_timer_is_running(lwiot_timer_t *timer);
extern DLL_EXPORT int lwiot_timer_set_period(lwiot_timer_t *timer, int ms);
extern DLL_EXPORT time_t lwiot_timer_get_expiry(lwiot_timer_t *timer);
#else /* CONFIG_NO_OS */
#define lwiot_timers_init()
#define lwiot_timers_destroy()
#endif /* CONFIG_NO_OS */

extern DLL_EXPORT void enter_critical();
extern DLL_EXPORT void exit_critical();

extern DLL_EXPORT void *lwiot_mem_alloc(size_t size);
extern DLL_EXPORT void *lwiot_mem_zalloc(size_t size);
extern DLL_EXPORT void lwiot_mem_free(void *ptr);

#define FOREVER 0
CDECL_END

#endif
