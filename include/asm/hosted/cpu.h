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
