
#include <esp8266.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/kernel/thread.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/watchdog.h>

#include <lwiot/esp8266/esp8266watchdog.h>

static const char *name = "tst-tp";

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread(name)
	{
	}

protected:
	void run() override
	{
		print_dbg("Main thread started..\n");

		while(true) {
			lwiot_sleep(1000);
		}
	}
};

static TestThread *tp;

extern "C" void lwiot_setup()
{
	printf("Starting..\n");
	tp = new TestThread();
	tp->start();
}
