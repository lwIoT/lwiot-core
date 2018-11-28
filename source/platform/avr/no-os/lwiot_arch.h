/*
 * No RTOS architecture header.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#define CONFIG_NO_OS 1

#ifdef __cplusplus
extern "C" void no_os_init(void);
#else
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
