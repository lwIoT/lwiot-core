/*
 * ESP8266 basic unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/log.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/dhtsensor.h>

static lwiot::Thread *tp;

class TestThread : public lwiot::Thread {
public:
	explicit TestThread() : Thread("test-thread")
	{ }

protected:
	void run() override
	{
		float h, t;
		lwiot::GpioPin dhtpin(2);
		lwiot::DhtSensor dht(dhtpin);

		h = t = 0.0f;

		while(true) {
			dht.read(h, t);
			print_dbg("DHT readout: h: %f - t: %f\n", h, t);
			lwiot_sleep(1000);
		}
	}
};

extern "C" void lwiot_setup()
{
	printf("Starting..\n");
	tp = new TestThread();
	tp->start();
}
