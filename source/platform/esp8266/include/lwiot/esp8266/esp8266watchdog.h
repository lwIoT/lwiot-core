/*
 * ESP8266 watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/io/watchdog.h>

namespace lwiot { namespace esp8266
{
	class Watchdog : public lwiot::Watchdog {
	public:
		static Watchdog& instance()
		{
			static Watchdog _instance;
			return _instance;
		}

		virtual ~Watchdog() = default;
		Watchdog(const Watchdog&) = delete;
		const Watchdog& operator =(const Watchdog&) = delete;

		virtual bool enable(uint32_t tmo = 2000) override;
		virtual bool disable() override;
		virtual void reset() override;

	private:
		volatile uint32_t count;
		volatile uint32_t overflow;

		friend void wdt_timer_irq_handler(void *arg);
		explicit Watchdog();
	};
}
}
