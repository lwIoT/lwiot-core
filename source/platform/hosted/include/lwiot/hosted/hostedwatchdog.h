/*
 * Hosted watchdog implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/watchdog.h>

namespace lwiot
{
	namespace hosted
	{
		class Watchdog : public lwiot::Watchdog {
		public:
			explicit Watchdog(const Watchdog&) = delete;
			Watchdog& operator=(const Watchdog&) = delete;

			static Watchdog& instance()
			{
				static Watchdog wdt;
				return wdt;
			}

			void reset() override;

		private:
			explicit Watchdog() : lwiot::Watchdog() { }
		};
	}
}
