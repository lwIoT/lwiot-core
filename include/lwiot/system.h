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
	/**
	 * @brief Sleep state.
	 * @ingroup util
	 */
	enum SleepState {
		SLEEP_LIGHT,
		SLEEP,
		SLEEP_DEEP
	};

	/**
	 * @brief System base class.
	 * @ingroup util
	 */
	class System {
	public:
		/**
		 * @brief Create a new system object.
		 * @param state Sleep state.
		 */
		explicit System(SleepState state);
		virtual ~System() = default;

		/**
		 * @brief Sleep for a number of miliseconds.
		 * @param ms Time to sleep for in miliseconds.
		 */
		virtual void sleep(const uint32_t& ms) const = 0;

		/**
		 * @brief Restart the system.
		 */
		virtual void restart() const = 0;

		/**
		 * @brief Get the current sleep state.
		 * @return
		 */
		const SleepState& sleepState() const;

		/**
		 * @brief Set the sleep state.
		 * @param state Sleep state to set.
		 */
		void setSleepSate(const SleepState& state);

		/**
		 * @brief Timeout the current thread for a period.
		 * @param ms Timeout length in miliseconds.
		 */
		static void delay(int ms);

	private:
		SleepState _state;
	};
}
