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
