/*
 * SoC system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

namespace lwiot
{
	enum SleepState {
		SLEEP_LIGHT,
		SLEEP,
		SLEEP_DEEP
	};

	class System {
	public:
		explicit System(SleepState state);
		virtual ~System() = default;

		virtual void sleep(const uint32_t& ms) const = 0;
		virtual void restart() const = 0;
		const SleepState& sleepState() const;
		void setSleepSate(const SleepState& state);

	private:
		SleepState _state;
	};
}
