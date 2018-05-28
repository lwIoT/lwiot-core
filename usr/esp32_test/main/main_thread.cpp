#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		lwiot::GpioPin out = 33;
		printf("Main thread started!\n");

		out.setOpenDrain();

		while(true) {
			int i = 0;

			enter_critical();
			while(i++ < 20) {
				out.write(true);
				lwiot_udelay(3);

				out.write(false);
				lwiot_udelay(3);
			}
			exit_critical();
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

