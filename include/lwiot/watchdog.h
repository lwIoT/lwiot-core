/*
 * Watchdog timer base clas definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	class Watchdog {
	public:
		virtual bool enable(uint32_t tmo = 2000) = 0;
		virtual bool disable() = 0;
		virtual void reset() = 0;

	protected:
		explicit Watchdog() { }
	};
}

extern lwiot::Watchdog& wdt;
