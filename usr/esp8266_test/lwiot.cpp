/*
 * ESP8266 basic unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <dhcpserver.h>

#include <lwiot/string.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/watchdog.h>
#include <lwiot/ipaddress.h>
#include <lwiot/wifistation.h>
#include <lwiot/wifiaccesspoint.h>

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
		//auto& sta = lwiot::WifiStation::instance();
		auto& ap = lwiot::WifiAccessPoint::instance();
		lwiot::GpioPin outPin = gpio[5];
		lwiot::IPAddress local(10, 0, 0, 1);
		lwiot::IPAddress gateway(10, 0, 0, 1);
		lwiot::IPAddress subnet(255, 255, 255, 0);
		ip4_addr_t first;


		//sta.connectTo("bietje", "banaan01");
		//while(!sta);

		ap.config(local, gateway, subnet);
		ap.begin("Test-AP", "12345678", 5, false, 4);
		while(!ap);
		IP4_ADDR(&first, 10, 0, 0, 2);
		dhcpserver_start(&first, 15);

		wdt.enable(2000);
		gpio.attachIrqHandler(14, irq_handler, lwiot::IrqRisingFalling);
		outPin.setOpenDrain();

		while(true) {
			int i = 0;
			while(i++ < 20) {
				outPin.write(true);
				lwiot_sleep(500);
				outPin.write(false);
				lwiot_sleep(500);

				wdt.reset();
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
