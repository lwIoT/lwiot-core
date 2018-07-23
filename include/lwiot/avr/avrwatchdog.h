/*
 * AVR WDT class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/watchdog.h>

namespace lwiot
{
	class AvrWatchdog : public Watchdog {
	public:
		static AvrWatchdog& instance()
		{
			static AvrWatchdog _instance;
			return _instance;
		}

		virtual ~AvrWatchdog() = default;
		AvrWatchdog(const AvrWatchdog&) = delete;
		const AvrWatchdog& operator =(const AvrWatchdog&) = delete;

		virtual bool enable(uint32_t tmo = 2000) override;
		virtual bool disable() override;
		virtual void reset() override;

	private:
		/* Methods */
		explicit AvrWatchdog();
		friend void wdt_trigger_handler();
	};
}
