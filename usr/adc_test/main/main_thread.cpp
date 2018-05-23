#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>
#include <lwiot/adcchip.h>
#include <lwiot/esp32adcchip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/esp32gpiochip.h>

#include <esp_task_wdt.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		adc.begin();

		printf("Main thread started!\n");

		while(true) {
			printf("ADC voltage read: %u\n", (adc.read(5) - 500) / 10);
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

