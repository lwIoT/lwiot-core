#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <esp_attr.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/uart.h>
#include <lwiot/string.h>
#include <lwiot/esp32/esp32uart.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		char *buffer = (char*)malloc(32);
		lwiot::Esp32Uart uart(2, 4800);
		lwiot::GpioPin out = 33;
		lwiot::String text("Test message");

		printf("Main thread started!\n");
		while(true) {
			memset((void*)&buffer[0], 0, 32);
			uart << text;
			uart.read((uint8_t*)&buffer[0], 3);
			printf("Received: %s\n", buffer);
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
	printf("[DONE]\n");
	mt->start();
}

