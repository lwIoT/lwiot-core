
#include <lwiot.h>

#include <lwiot/string.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

static lwiot::Thread *tp;
static volatile int irqs = 0;

static void IRAM irq_handler(void)
{
	irqs++;
}

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread("test-thread")
	{ }

protected:
	void run(void *argument) override
	{
		lwiot::GpioPin outPin = gpio[5];

		gpio.attachIrqHandler(14, irq_handler, lwiot::IrqRisingFalling);
		outPin.setOpenDrain();

		while(true) {
			int i = 0;
			while(i++ < 20) {
				outPin.write(true);

				lwiot_sleep(500);

				outPin.write(false);
				lwiot_sleep(500);
				printf("IRQs: %i\n", irqs);
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

