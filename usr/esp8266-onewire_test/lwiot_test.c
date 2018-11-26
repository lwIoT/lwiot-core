/**
 * Very basic example showing usage of access point mode and the DHCP server.
 * The ESP in the example runs a telnet server on 172.16.0.1 (port 23) that
 * outputs some status information if you connect to it, then closes
 * the connection.
 *
 * This example code is in the public domain.
 */

#include <esp8266.h>
#include <string.h>
#include <lwiot.h>

#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <lwip/api.h>

extern void lwiot_setup();

void user_init(void)
{
    uart_set_baud(0, 115200);
    lwiot_init();
    lwiot_setup();
}
