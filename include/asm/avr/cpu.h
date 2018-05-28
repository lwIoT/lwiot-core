/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" void udelay(uint32_t us);
#else
extern void udelay(uint32_t us);
#endif

#define lwiot_udelay(__us) udelay(__us)
