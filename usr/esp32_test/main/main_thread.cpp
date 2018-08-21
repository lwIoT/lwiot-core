/*
 * ESP32 device test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
#include <lwiot/watchdog.h>
#include <lwiot/datetime.h>
#include <lwiot/wifiaccesspoint.h>

#include <lwiot/esp32/esp32pwm.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void startPwm(lwiot::esp32::PwmTimer& timer)
	{
		auto& channel = timer[0];
		auto pin = lwiot::GpioPin(5);

		channel.setGpioPin(pin);
		channel.setDutyCycle(75.0f);
		channel.enable();
		lwiot_sleep(2000);

		channel.setDutyCycle(50.0f);
		timer.setFrequency(100);
		channel.reload();
	}

	void startAP(const lwiot::String& ssid, const lwiot::String& passw)
	{
		auto& ap = lwiot::WifiAccessPoint::instance();
		lwiot::IPAddress local(192, 168, 1, 1);
		lwiot::IPAddress subnet(255, 255, 255, 0);
		lwiot::IPAddress gw(192, 168, 1, 1);

		ap.config(local, gw, subnet);
		ap.begin(ssid, passw, 4);
	}

	virtual void run(void *arg) override
	{
		lwiot::GpioPin out = 22;
		lwiot::GpioPin out2 = 23;
		lwiot::esp32::PwmTimer timer(0, MCPWM_UNIT_0, 100);
		size_t freesize;

		printf("Main thread started!\n");
		this->startPwm(timer);
		out.setOpenDrain();
		out2.setOpenDrain();
		lwiot_sleep(1000);

		lwiot::DateTime dt;
		print_dbg("Time: %s\n", dt.toString().c_str());
		freesize = heap_caps_get_free_size(0);
		wdt.enable();

		print_dbg("Free heap size: %u\n", freesize);
		this->startAP("lwIoT test", "testap1234");
		wdt.reset();

		while(true) {
			int i = 0;

			enter_critical();
			while(i++ < 1600) {
				out << true;
				out2 << false;
				lwiot_udelay(3);

				out << false;
				out2 << true;
				lwiot_udelay(3);
			}
			exit_critical();

			wdt.reset();
		}
	}
};

static MainThread *mt;
static const char *arg = "Hello, World! [FROM main-thread]";

extern "C" void main_start(void)
{
	printf("Creating main thread..");
	mt = new MainThread(arg);
	printf(" [DONE]\n");
	mt->start();
}

