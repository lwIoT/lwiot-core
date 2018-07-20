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
		auto& ap = lwiot::WifiAccessPoint::instance();
		lwiot::GpioPin outPin = gpio[5];
		lwiot::IPAddress local(10, 0, 0, 1);
		lwiot::IPAddress gateway(10, 0, 0, 1);
		lwiot::IPAddress subnet(255, 255, 255, 0);
		lwiot::Lock lock(false);
		ip4_addr_t first;

		ap.config(local, gateway, subnet);
		ap.begin("Test-AP", "12345678", 5, false, 4);
		while(!ap);
		IP4_ADDR(&first, 10, 0, 0, 2);
		dhcpserver_start(&first, 15);

		wdt.enable(2000);
		gpio.attachIrqHandler(14, irq_handler, lwiot::IrqRisingFalling);
		outPin.setOpenDrain();

		FUNC_THREAD(tp, "fthread", [&lock](void) -> void {
			lwiot::ScopedLock slock(lock);
			int i = 0;

			while(i++ <= 10) {
				print_dbg("Lambda thread ping!\n");
				wdt.reset();
				lwiot_sleep(750);
			}
		});

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
				printf("IRQs: %i\n", irqs);
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
