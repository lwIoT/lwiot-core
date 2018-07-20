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

#include <lwiot/esp32/esp32pwm.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void*)arg)
	{ }

protected:
	void startPwm(lwiot::Esp32PwmTimer& timer)
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

	virtual void run(void *arg) override
	{
		lwiot::GpioPin out = 22;
		lwiot::GpioPin out2 = 23;
		lwiot::Esp32PwmTimer timer(0, MCPWM_UNIT_0, 100);
		size_t freesize;
		bool value;

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

		while(true) {
			int i = 0;

			enter_critical();
			while(i++ < 20) {
				out << true;
				lwiot_udelay(3);
				out2 >> value;
				assert(value == true);

				out << false;
				lwiot_udelay(3);
				out2 >> value;
				assert(value == false);
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

