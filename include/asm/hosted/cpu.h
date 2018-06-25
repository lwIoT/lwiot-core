/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>

extern DLL_EXPORT void enter_critical();
extern DLL_EXPORT void exit_critical();

#ifdef __cplusplus
extern "C" void udelay(uint32_t us);
#else
extern void udelay(uint32_t us);
#endif

#define lwiot_udelay(__us) udelay(__us)
