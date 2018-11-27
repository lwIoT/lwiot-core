/*
 * No RTOS architecture header.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#ifndef AVR
#error The NO_OS configuration is only available for AVR targets!
#endif

#define CONFIG_NO_OS 1

#ifdef __cplusplus
extern "C" void udelay(uint32_t us);
extern "C" void no_os_init(void);
#else
extern void udelay(uint32_t us);
extern void no_os_init(void);
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mutex {
	uint8_t lock;
#define HAVE_MUTEX
} lwiot_mutex_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
