/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

/* ESP8266 SDK headers */
#include <rom/ets_sys.h>

#include <lwiot/types.h>

void udelay(uint32_t us)
{
	ets_delay_us(us);
}