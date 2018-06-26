
#include <lwiot.h>

#include <lwiot/string.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/esp8266/esp8266uart.h>

static const char *name = "tst-tp";

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread(name), uart(0, 5, 4, 4800)
	{
	}

protected:
	void run(void *argument) override
	{
		char *buffer = (char*)malloc(32);
		lwiot::String text("Test message");

		printf("Main thread started!\n");
		while(true) {
			memset((void*)&buffer[0], 0, 32);

			uart.read((uint8_t*)&buffer[0], 13);
			uart.write(text);
			printf("Received: %s\n", buffer);
			lwiot_sleep(1000);
		}
	}

	lwiot::Esp8266Uart uart;
};

static TestThread *tp;

extern "C" void lwiot_setup()
{
	lwiot_init();
	printf("Starting..\n");
	tp = new TestThread();
	tp->start();
}

extern "C" void __cxa_pure_virtual()
{
	fprintf(stderr, "Pure virtual not implemented!\n");
}

extern "C" void __gxx_personality_v0()
{
	
}
