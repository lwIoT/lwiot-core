/*
 * ESP8266 watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <espressif/esp_common.h>
#include <espressif/esp8266/esp8266.h>

#include <lwiot/watchdog.h>
#include <lwiot/esp8266watchdog.h>

#define WDT_FREQ 10

namespace lwiot
{
	void wdt_timer_irq_handler(void *arg)
	{
		lwiot::Esp8266Watchdog *wd = (lwiot::Esp8266Watchdog*)arg;
		wd->count++;

		if(wd->count >= wd->overflow) {
			printf("WDT triggered!\n");
			sdk_system_restart();
		}
	}

	Esp8266Watchdog::Esp8266Watchdog() : Watchdog()
	{ }

	bool Esp8266Watchdog::enable(uint32_t tmo)
	{
		double divider;

		/* First, disable timer IRQ 1 */
		timer_set_interrupts(FRC1, false);
		timer_set_run(FRC1, false);
		_xt_isr_attach(INUM_TIMER_FRC1, wdt_timer_irq_handler, (void*)this);

		divider = 1.0f / WDT_FREQ;
		auto tick = static_cast<uint32_t>(divider * 1000.0f);
		this->overflow = tmo / tick;
		this->count = 0;

		timer_set_frequency(FRC1, WDT_FREQ);
		timer_set_interrupts(FRC1, true);
		timer_set_run(FRC1, true);

		return true;
	}

	bool Esp8266Watchdog::disable()
	{
		timer_set_interrupts(FRC1, false);
		timer_set_run(FRC1, false);
		this->count = this->overflow = 0;

		return true;
	}

	void Esp8266Watchdog::reset()
	{
		this->count = 0;
	}
}

lwiot::Watchdog& wdt = lwiot::Esp8266Watchdog::instance();
