/*
 * AVR watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/io/watchdog.h>
#include <lwiot/avr/avrwatchdog.h>
#include <lwiot/log.h>

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#ifdef IDE
/* VS code fix */
#undef wdt_disable()
#undef wdt_enable(x)
#define wdt_enable(x)
#define wdt_disable()
#endif

static inline bool is_between(uint32_t x, uint32_t a, uint32_t b)
{
	return x >= a && x <= b;
}

namespace lwiot { namespace avr
{
	extern void wdt_isr_handler();
	Watchdog::Watchdog() : ::lwiot::Watchdog()
	{ }

	bool Watchdog::enable(uint32_t tmo)
	{
		uint8_t wdtsetting;

		if(is_between(tmo, 0, 15))
			wdtsetting = WDTO_15MS;
		else if(is_between(tmo, 15, 30))
			wdtsetting = WDTO_30MS;
		else if(is_between(tmo, 30, 60))
			wdtsetting = WDTO_60MS;
		else if(is_between(tmo, 60, 120))
			wdtsetting = WDTO_120MS;
		else if(is_between(tmo, 120, 250))
			wdtsetting = WDTO_250MS;
		else if(is_between(tmo, 250, 500))
			wdtsetting = WDTO_500MS;
		else if(is_between(tmo, 500, 1000))
			wdtsetting = WDTO_1S;
		else if(is_between(tmo, 1000, 2000))
			wdtsetting = WDTO_2S;
		else if(is_between(tmo, 2000, 4000))
			wdtsetting = WDTO_4S;
		else if(is_between(tmo, 4000, 0xFFFFFFFF))
			wdtsetting = WDTO_8S;

		auto old = SREG;
		cli();
		wdt_reset();
		_WD_CONTROL_REG |= 1 << WDIE;
		wdt_enable(wdtsetting);
		SREG = old;

		lwiot::Watchdog::enable(tmo);
		return true;
	}

	bool Watchdog::disable()
	{
		auto old = SREG;

		Watchdog::disable();
		cli();
		wdt_reset();
		wdt_disable();
		SREG = old;

		return true;
	}

	void Watchdog::reset()
	{
		if(!this->enabled())
			return;

		wdt_reset();
	}
}
}

extern "C" {
	ISR(WDT_vect)
	{
		lwiot::avr::wdt_isr_handler();
	}
}

::lwiot::Watchdog& wdt = lwiot::avr::Watchdog::instance();
