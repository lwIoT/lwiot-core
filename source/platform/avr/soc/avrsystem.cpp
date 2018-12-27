/*
 * AVR system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/system.h>
#include <lwiot/avr/avrsystem.h>

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#ifndef NO_OS
#include <FreeRTOS.h>
#endif

#ifdef IDE
/* VS code fix */
#undef wdt_disable()
#undef wdt_enable(x)
#define wdt_enable(x)
#define wdt_disable()
#endif

namespace lwiot { namespace avr
{
	bool System::_restart = false;

	System::System(SleepState state) : lwiot::System(state)
	{
		wdt_disable();
	}

	System::~System()
	{
		sleep_disable();
	}

	void System::sleep(const uint32_t& ms) const
	{
#ifdef NO_OS
		auto idx = ms;
		while(idx) {
			_delay_ms(1);
			idx--;
		}

		if(this->sleepState() == SLEEP_DEEP)
			this->restart();
#else
		uint32_t _ms = ms;
		time_t now;

		switch(this->sleepState()) {
		case SLEEP_LIGHT:
			set_sleep_mode(SLEEP_MODE_IDLE);
			break;

		case SLEEP:
			set_sleep_mode(SLEEP_MODE_PWR_SAVE);
			break;

		case SLEEP_DEEP:
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			break;
		}

		now = lwiot_tick();
		_ms /= portTICK_PERIOD_MS;
		_ms *= 1000U;
		now += _ms;
		sleep_enable();

		while(now) {
			sleep_cpu();
			auto update = lwiot_tick();
			if(update >= now)
				break;
		}

		sleep_disable();

		if(this->sleepState() == SLEEP_DEEP)
			this->restart();
#endif
	}

	void System::setupWdt() const
	{
		auto sreg = SREG;

		cli();
		wdt_reset();
		wdt_enable(WDTO_30MS);
		SREG = sreg;
	}

	void System::restart() const
	{
		/*
		  Use the WDT to reset the MCU.
		 */
		this->setupWdt();
		System::_restart = true;
		while(true);
	}

	void wdt_isr_handler()
	{
		if(System::_restart)
			return;

		printf("Watchdog timer triggerd!\n");
	}
}
}
