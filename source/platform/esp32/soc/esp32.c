/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <freertos/FreeRTOS.h>

/* ESP8266 SDK headers */
#include <rom/ets_sys.h>

#include <lwiot/types.h>

static portMUX_TYPE xCoreMutex[] = {
	portMUX_INITIALIZER_UNLOCKED,
#if portNUM_PROCESSORS == 2
	portMUX_INITIALIZER_UNLOCKED
#endif
};

void RAM_ATTR lwiot_udelay(uint32_t us)
{
	ets_delay_us(us);
}

void RAM_ATTR enter_critical()
{
	int id;

	id = xPortGetCoreID();
	portENTER_CRITICAL(&xCoreMutex[id]);
}

void RAM_ATTR exit_critical()
{
	int id;

	id = xPortGetCoreID();
	portEXIT_CRITICAL(&xCoreMutex[id]);
}

BaseType_t esp32_get_next_coreid()
{
	static bool coreid = false;
	BaseType_t retval;

	retval = coreid;
	coreid = !coreid;

	return retval;
}
