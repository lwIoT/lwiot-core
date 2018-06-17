/*
 * ESP8266 SoC definitions.
 */

#include <FreeRTOS.h>

void enter_critical()
{
	taskENTER_CRITICAL();
}

void exit_critical()
{
	taskEXIT_CRITICAL();
}
