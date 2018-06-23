/*
 * lwIoT on-chip WiFi test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/ipaddress.h>
#include <lwiot/wifiaccesspoint.h>
#include <lwiot/wifistation.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void run(void *arg)
	{
		auto& ap = lwiot::WifiAccessPoint::instance();

		printf("Main thread started!\n");
		ap.begin("ESP-TEST", "12345678", 4, false, 4);

		while(true) {
			print_dbg("Main thread ping\n");
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

