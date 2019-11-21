/*
 * lwIoT - Type wrapper header
 *
 * Author: Michel Megens
 * Date: 15/12/2017
 * Email: dev@bietje.net
 */

#ifndef __ESTACK_TYPES_H__
#define __ESTACK_TYPES_H__

#include "lwiot_opts.h"

#if defined(HAVE_TIME_H) || defined(AVR)
#include <time.h>
#else
#ifndef AVR
typedef unsigned long long time_t;
#endif
#endif

#ifndef HAVE_SIZE_T
#include <stdint.h>
typedef uintptr_t size_t;
#endif

#ifndef HAVE_SSIZE_T
#include <stdint.h>
typedef intptr_t ssize_t;
#endif

typedef enum {
	TIMER_CREATED = 1,
	TIMER_RUNNING,
	TIMER_STOPPED,
} timer_state_t;

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

typedef void (*irq_handler_t)(void);

#ifdef __cplusplus
namespace lwiot
{
	namespace stl
	{
		typedef ::size_t size_t;
		template<typename...> using __void_t = void;
	}
}
#endif

#endif
