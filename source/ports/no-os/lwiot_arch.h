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
#else
extern void udelay(uint32_t us);
#endif

#define lwiot_udelay(__us) udelay(__us)
