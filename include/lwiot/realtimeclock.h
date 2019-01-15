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
	class RealTimeClock {
	public:
		virtual ~RealTimeClock() = default;

		virtual DateTime now() = 0;
		void time(DateTime& dt);

		virtual void set(const DateTime& dt) = 0;
	};
}
