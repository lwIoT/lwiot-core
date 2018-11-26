/**
 * Very basic example showing usage of access point mode and the DHCP server.
 * The ESP in the example runs a telnet server on 172.16.0.1 (port 23) that
 * outputs some status information if you connect to it, then closes
 * the connection.
 *
 * This example code is in the public domain.
 */

#include <string.h>
#include <esp8266.h>
#include <lwiot.h>

#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <lwip/api.h>

extern void lwiot_setup();

static void vTaskMain(void *pvparam)
{
    printf("Main task started..\n");
    lwiot_setup();
}

void user_init(void)
{
    TaskHandle_t xHandle = NULL;

    uart_set_baud(0, 115200);
    xTaskCreate( vTaskMain, "main", CONFIG_STACK_SIZE, NULL, CONFIG_TASK_PRIO, &xHandle );
}
