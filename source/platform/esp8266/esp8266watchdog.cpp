/*
 * ESP8266 watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <esp8266.h>
#include <lwiot.h>

extern "C" {
#include <espressif/esp_common.h>
#include <espressif/esp8266/esp8266.h>
#include <esp/interrupts.h>
}

#include <lwiot/io/watchdog.h>
#include <lwiot/esp8266/esp8266watchdog.h>

#define WDT_FREQ 10

namespace lwiot { namespace esp8266
{
	void wdt_timer_irq_handler(void *arg)
	{
		lwiot::esp8266::Watchdog *wd = (lwiot::esp8266::Watchdog*)arg;
		wd->count++;

		if(wd->count >= wd->overflow) {
			printf("WDT triggered!\n");
			sdk_system_restart();
		}
	}

	Watchdog::Watchdog() : lwiot::Watchdog()
	{ }

	bool Watchdog::enable(uint32_t tmo)
	{
		double divider;

		/* First, disable timer IRQ 1 */
		timer_set_interrupts(FRC1, false);
		timer_set_run(FRC1, false);
		//_xt_isr_attach(INUM_TIMER_FRC1, wdt_timer_irq_handler, (void*)this);

		divider = 1.0f / WDT_FREQ;
		auto tick = static_cast<uint32_t>(divider * 1000.0f);
		this->overflow = tmo / tick;
		this->count = 0;

		timer_set_frequency(FRC1, WDT_FREQ);
		timer_set_interrupts(FRC1, true);
		timer_set_run(FRC1, true);
		lwiot::Watchdog::enable(tmo);

		return true;
	}

	bool Watchdog::disable()
	{
		Watchdog::disable();
		timer_set_interrupts(FRC1, false);
		timer_set_run(FRC1, false);
		this->count = this->overflow = 0;

		return true;
	}

	void Watchdog::reset()
	{
		if(!this->enabled())
			return;

		enter_critical();
		this->count = 0;
		exit_critical();
	}
}
}

lwiot::Watchdog& wdt = lwiot::esp8266::Watchdog::instance();
