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
#include <lwiot/avrsystem.h>

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#ifdef IDE
/* VS code fix */
#undef wdt_disable()
#undef wdt_enable(x)
#define wdt_enable(x)
#define wdt_disable()
#endif

namespace lwiot
{
	bool AvrSystem::_restart = false;

	AvrSystem::AvrSystem(SleepState state) : System(state)
	{
		wdt_disable();
	}

	AvrSystem::~AvrSystem()
	{
		sleep_disable();
	}

	void AvrSystem::sleep(const uint32_t& ms) const
	{
		uint32_t _ms = ms;
#ifdef NO_OS
		auto idx = ms;
		while(idx) {
			_delay_ms(1);
			idx--;
		}

		if(this->sleepState() == SLEEP_DEEP)
			this->restart();
#else
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

	void AvrSystem::setupWdt() const
	{
		auto sreg = SREG;

		cli();
		wdt_reset();
		wdt_enable(WDTO_30MS);
		SREG = sreg;
	}

	void AvrSystem::restart() const
	{
		/*
		  Use the WDT to reset the MCU.
		 */
		this->setupWdt();
		AvrSystem::_restart = true;
		while(true);
	}

	void wdt_isr_handler()
	{
		if(AvrSystem::_restart)
			return;

		printf("Watchdog timer triggerd!\n");
	}
}

ISR(WDT_vect)
{
	lwiot::wdt_isr_handler();
}
