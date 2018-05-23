
#include <lwiot.h>

#include <lwiot/string.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

static lwiot::Thread *tp;

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread("test-thread")
	{ }

protected:
	void run(void *argument) override
	{
		lwiot::GpioPin outPin = gpio[5];

		outPin.setOpenDrain();
		while(true) {
			int i = 0;
			while(i++ < 20) {
				outPin.write(true);

				lwiot_sleep(500);

				outPin.write(false);
				lwiot_sleep(500);
			}
		}
	}
};

extern "C" void lwiot_setup()
{
	printf("Starting..\n");
	tp = new TestThread();
	tp->start();
}

extern "C" void __cxa_pure_virtual()
{
	while(true);
}

