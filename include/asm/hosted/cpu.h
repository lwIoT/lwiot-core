/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>

#ifdef __cplusplus
extern "C" void enter_critical();
extern "C" void exit_critical();
extern "C" void udelay(uint32_t us);
#else
extern void udelay(uint32_t us);
extern void enter_critical();
extern void exit_critical();
#endif

#define lwiot_udelay(__us) udelay(__us)

#define HAVE_STL
