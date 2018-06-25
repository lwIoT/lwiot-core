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
#include <lwiot/watchdog.h>

namespace lwiot
{
	class Esp8266Watchdog : public Watchdog {
	public:
		static Esp8266Watchdog& instance()
		{
			static Esp8266Watchdog _instance;
			return _instance;
		}

		virtual ~Esp8266Watchdog() = default;
		Esp8266Watchdog(const Esp8266Watchdog&) = delete;
		const Esp8266Watchdog& operator =(const Esp8266Watchdog&) = delete;

		virtual bool enable(uint32_t tmo = 2000) override;
		virtual bool disable() override;
		virtual void reset() override;

	private:
		volatile uint32_t count;
		volatile uint32_t overflow;

		friend void wdt_timer_irq_handler(void *arg);
		explicit Esp8266Watchdog();
	};
}