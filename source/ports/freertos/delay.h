/*
 * Microsecond deley definitions for various architectures.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#if defined(ESP32)
#include <rom/ets_sys.h>
#define lwiot_udelay(__us) ets_delay_us(__us)
#elif defined(ESP8266)
#include <espressif/esp_misc.h>
#define lwiot_udelay(__us) sdk_os_delay_us(__us)
#elif defined(AVR)
extern void udelay(uint32_t us);
#define lwiot_udelay(__us) udelay(__us)
#endif
