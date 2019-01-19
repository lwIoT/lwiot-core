#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <lwiot.h>
#include <esp_task_wdt.h>

#include <lwiot/thread.h>
#include <lwiot/stl/string.h>
#include <lwiot/io/watchdog.h>
#include <lwiot/log.h>

#include <lwiot/esp32/esp32watchdog.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run()
	{
		wdt.enable(5000);

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

