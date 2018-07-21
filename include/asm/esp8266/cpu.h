/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <espressif/esp_misc.h>
#define lwiot_udelay(__us) sdk_os_delay_us(__us)

#ifdef __cplusplus
extern "C" {
#endif

extern void enter_critical();
extern void exit_critical();

#ifdef __cplusplus
}
#endif
