/*
 * ESP8266 basic unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <dhcpserver.h>

#include <lwiot/string.h>
#include <lwiot/scopedlock.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/watchdog.h>
#include <lwiot/ipaddress.h>
#include <lwiot/wifistation.h>
#include <lwiot/wifiaccesspoint.h>
#include <lwiot/functionalthread.h>

#include <lwip/api.h>

static lwiot::Thread *tp;

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread("tst-thr")
	{ }

protected:
	void run(void *argument) override
	{
		lwiot::Lock lock(false);
		lwiot::GpioPin outPin = 5;

		print_dbg("Main thread started..\n");

		wdt.enable(2000);

		lwiot::FunctionalThread tp("fthread");
		lwiot::Function<void(*)(void)> f = [&lock](void) -> void {
			lwiot::ScopedLock slock(lock);
			int i = 0;

			while(i++ <= 10) {
				print_dbg("Lambda thread ping!\n");
				wdt.reset();
				lwiot_sleep(750);
			}
		};

		tp = f;
		tp.start();

		while(true) {
			int i = 0;
			while(i++ < 20) {
				lock.lock();
				outPin.write(true);
				lwiot_sleep(500);
				outPin.write(false);
				lwiot_sleep(500);

				wdt.reset();
				print_dbg("Ping..\n");
				lock.unlock();
				lwiot::Thread::yield();
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
