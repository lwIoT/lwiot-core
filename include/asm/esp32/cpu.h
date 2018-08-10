/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include <rom/ets_sys.h>

#define lwiot_udelay(__us) ets_delay_us(__us)

#ifdef __cplusplus
extern "C" {
#endif

extern BaseType_t esp32_get_next_coreid();
extern void enter_critical();
extern void exit_critical();

#ifdef __cplusplus
}
#endif
