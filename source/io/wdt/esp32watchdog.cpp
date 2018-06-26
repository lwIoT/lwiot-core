/*
 * ESP32 watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <esp_task_wdt.h>

#include <lwiot/watchdog.h>
#include <lwiot/esp32/esp32watchdog.h>

namespace lwiot
{
	Esp32Watchdog::Esp32Watchdog() : Watchdog()
	{ }

	bool Esp32Watchdog::enable(uint32_t tmo)
	{
		bool ret = esp_task_wdt_init(tmo / 1000, true) == ESP_OK;

#ifdef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0
		esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(0));
#endif
#ifdef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1
		esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(1));
#endif

		return ret;
	}

	bool Esp32Watchdog::disable()
	{
		return esp_task_wdt_deinit() == ESP_OK;
	}

	void Esp32Watchdog::reset()
	{
		TaskHandle_t handle;

		if(esp_task_wdt_reset() == ESP_OK)
			return;

		handle = xTaskGetCurrentTaskHandle();
		esp_task_wdt_add(handle);
		esp_task_wdt_reset();
	}
}

lwiot::Watchdog& wdt = lwiot::Esp32Watchdog::instance();
