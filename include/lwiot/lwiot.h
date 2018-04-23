/*
 * lwIoT project header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <config.h>

#include <lwiot/compiler.h>
#include <lwiot/types.h>

typedef enum {
	TIMER_CREATED = 1,
	TIMER_RUNNING,
	TIMER_STOPPED,
} timer_state_t;

#include <lwiot/port.h>

#define UNUSED(x) (void)x
