/*
 * Watchdog timer base clas definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file watchdog.h

#pragma once

#include <stdint.h>

namespace lwiot
{
	/**
	 * @brief WDT abstraction layer.
	 * @ingroup io
	 */
	class Watchdog {
	public:
		/**
		 * @brief Enable the watchdog.
		 * @param tmo Timeout.
		 * @return Success indicator.
		 */
		virtual bool enable(uint32_t tmo);

		/**
		 * @brief Disable the watchdog.
		 * @return Success indicator.
		 */
		virtual bool disable();

		/**
		 * @brief Reset the watchdog.
		 */
		virtual void reset() = 0;

	protected:
		/**
		 * @brief Construct new watchdog object.
		 */
		explicit Watchdog();

		/**
		 * @brief Check if the watchdog is enabled.
		 * @return True or false based on the status of the WDT.
		 */
		bool enabled() const;

	private:
		bool _enabled;
	};
}

extern lwiot::Watchdog& wdt; //!< Global watchdog timer.
