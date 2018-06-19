/*
 * lwIoT - CPU level abstraction.
 *
 * Author: Michel Megens
 * Date:   18/12/2017
 * Email:  dev@bietje.net
 */

#pragma once

#include <rom/ets_sys.h>
#define lwiot_udelay(__us) ets_delay_us(__us)

extern "C" void enter_critical();
extern "C" void exit_critical();
