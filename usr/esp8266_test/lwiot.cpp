/*
 * ESP8266 basic unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <esp8266.h>
#include <math.h>
#include <lwiot.h>

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
#include <lwiot/measurementvector.h>
#include <lwiot/wifiaccesspoint.h>
#include <lwiot/application.h>
#include <lwiot/functionalthread.h>

#include <lwip/api.h>
#include <lwip/ip_addr.h>

#include <dhcpserver.h>

class EspTestApplication : public lwiot::Functor {
public:
	virtual void operator()() override
	{
		bool value = false;

		print_dbg("Main thread started..\n");

		lwiot::GpioPin outPin = 5;
		outPin.mode(lwiot::PinMode::OUTPUT);
		wdt.enable(2000);

		while(true) {
			int i = 0;
			while(i++ < 20) {
				outPin.write(value);
				value = !value;

				wdt.reset();
				lwiot_sleep(50);
			}
		}
	}
};

extern "C" void lwiot_setup()
{
	EspTestApplication runner;
	lwiot::Application app(runner);

	app.start();
}
