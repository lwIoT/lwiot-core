/*
 * ESP8266 SoC definitions.
 */

#include <stdlib.h>
#include <lwiot.h>
#include <FreeRTOS.h>

void enter_critical()
{
	taskENTER_CRITICAL();
}

void exit_critical()
{
	taskEXIT_CRITICAL();
}
