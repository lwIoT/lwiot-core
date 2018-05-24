/*
 * Microsecond deley definitions for various architectures.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#if defined(ESP32)
#include <rom/ets_sys.h>
#define lwiot_udelay(__us) ets_delay_us(__us)
#elif defined(ESP8266)
#include <espressif/esp_misc.h>
#define lwiot_udelay(__us) sdk_os_delay_us(__us)
#elif defined(AVR)

#ifdef __cplusplus
extern "C" void udelay(uint32_t us);
#else
extern void udelay(uint32_t us);
#endif

#define lwiot_udelay(__us) udelay(__us)
#endif
