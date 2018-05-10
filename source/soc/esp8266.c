/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

/* ESP8266 SDK headers */
#include <espressif/esp_misc.h>

#include <lwiot/types.h>

void udelay(uint32_t us)
{
	sdk_os_delay_us(us);
}
