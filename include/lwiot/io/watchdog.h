/*
 * Watchdog timer base clas definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>

namespace lwiot
{
	class Watchdog {
	public:
		virtual bool enable(uint32_t tmo = 2000);
		virtual bool disable();
		virtual void reset() = 0;

	protected:
		explicit Watchdog();
		bool enabled() const;

	private:
		bool _enabled;
	};
}

extern lwiot::Watchdog& wdt;
