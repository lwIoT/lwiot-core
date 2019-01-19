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

#include <lwiot/io/watchdog.h>
#include <lwiot/esp32/esp32watchdog.h>

namespace lwiot { namespace esp32
{
	Watchdog::Watchdog() : lwiot::Watchdog()
	{ }

	bool Watchdog::enable(uint32_t tmo)
	{
		bool ret = esp_task_wdt_init(tmo / 1000, true) == ESP_OK;

		if(ret) {
			lwiot::Watchdog::enable(tmo);
#ifdef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0
			esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));
#endif
#ifdef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1
			esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1));
#endif
		}

		return ret;
	}

	bool Watchdog::disable()
	{
		lwiot::Watchdog::disable();
		return esp_task_wdt_deinit() == ESP_OK;
	}

	void Watchdog::reset()
	{
		TaskHandle_t handle;

		if(!this->enabled())
			return;

		if(esp_task_wdt_reset() == ESP_OK)
			return;

		handle = xTaskGetCurrentTaskHandle();
		esp_task_wdt_add(handle);
		esp_task_wdt_reset();
	}
}
}

lwiot::Watchdog& wdt = lwiot::esp32::Watchdog::instance();
