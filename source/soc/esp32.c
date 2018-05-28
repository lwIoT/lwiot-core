/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <FreeRTOS.h>

/* ESP8266 SDK headers */
#include <rom/ets_sys.h>

#include <lwiot/types.h>

static portMUX_TYPE xCoreMutex[] = {
	portMUX_INITIALIZER_UNLOCKED,
#if portNUM_PROCESSORS == 2
	portMUX_INITIALIZER_UNLOCKED
#endif
};

void enter_critical()
{
	int id;

	id = xPortGetCoreID();
	portENTER_CRITICAL(&xCoreMutex[id]);
}

void exit_critical()
{
	int id;

	id = xPortGetCoreID();
	portEXIT_CRITICAL(&xCoreMutex[id]);
}
