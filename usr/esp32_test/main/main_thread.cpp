#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <esp_heap_caps.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/datetime.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		lwiot::GpioPin out = 23;
		size_t freesize;

		printf("Main thread started!\n");
		out.setOpenDrain();
		lwiot_sleep(1000);

		lwiot::DateTime dt;
		print_dbg("Time: %s\n", dt.toString().c_str());
		freesize = heap_caps_get_free_size(0);

		print_dbg("Free heap size: %u\n", freesize);

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

