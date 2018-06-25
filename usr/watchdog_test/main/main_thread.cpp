#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <lwiot.h>
#include <esp_task_wdt.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/watchdog.h>
#include <lwiot/log.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		TaskHandle_t handle;
		print_dbg("Main thread started!\n");

		if(esp_task_wdt_reset() == ESP_OK)
			return;

		handle = xTaskGetCurrentTaskHandle();
		esp_task_wdt_add(handle);
		esp_task_wdt_reset();

		while(true) {
			wdt.reset();
			print_dbg("Main thread ping!\n");
			lwiot_sleep(1000);
		}
	}
};

static MainThread *mt;
const char *arg = "Hello, World! [FROM main-thread]";

extern "C" void main_start(void)
{
	printf("Creating main thread..");
	mt = new MainThread(arg);
	printf(" [DONE]\n");
	mt->start();
}

