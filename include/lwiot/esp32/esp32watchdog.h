/*
 * ESP32 watchdog timer definition.
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
	class Esp32Watchdog : public Watchdog {
	public:
		static Esp32Watchdog& instance()
		{
			static Esp32Watchdog _instance;
			return _instance;
		}

		virtual ~Esp32Watchdog() = default;
		Esp32Watchdog(const Esp32Watchdog&) = delete;
		const Esp32Watchdog& operator =(const Esp32Watchdog&) = delete;

		virtual bool enable(uint32_t tmo = 2000) override;
		virtual bool disable() override;
		virtual void reset() override;

	private:
		explicit Esp32Watchdog();
	};
}
