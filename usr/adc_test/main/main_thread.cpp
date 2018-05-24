#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>
#include <lwiot/adcchip.h>
#include <lwiot/gpiopin.h>

#include <esp_task_wdt.h>

static volatile int irq_counter = 0;

static void IRAM_ATTR ping_handler(void)
{
	//printf("Interrupt happend\n");
	irq_counter++;
}

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		adc.begin();
		gpio.mode(14, lwiot::INPUT);
		gpio.attachIrqHandler(14, ping_handler, lwiot::IrqRising);

		printf("Main thread started!\n");

		while(true) {
			printf("ADC voltage read: %u\n", (adc.read(5) - 500) / 10);
			printf("IRQ counter: %i\n", irq_counter);
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

