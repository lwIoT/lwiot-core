/*
 * RTC interface / abstract class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/util/datetime.h>

namespace lwiot
{
	/**
	 * @brief Real-time clock.
	 * @ingroup util
	 */
	class RealTimeClock {
	public:
		virtual ~RealTimeClock() = default;

		virtual DateTime now() = 0; //!< Get the current time.
		void time(DateTime& dt); //!< Get the current time in a DateTime object.

		virtual void set(const DateTime& dt) = 0; //!< Set the time.
	};
}
