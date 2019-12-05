/*
 * lwIoT timer implementation
 * 
 * Author: Michel Megens
 * Date:   01/05/2018
 * Email:  dev@bietje.net
 */

/// @file timer.h Timer header.

#pragma once

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/stl/string.h>

namespace lwiot
{
	/**
	 * @brief Timer abstraction layer.
	 * @ingroup kernel
	 */
	class Timer {
	public:
		/**
		 * @brief Construct a new timer.
		 * @param name Timer name.
		 * @param ms Interval.
		 * @param flags Timer flags.
		 * @param arg Timer handler argument.
		 */
		explicit Timer(const lwiot::String& name, unsigned long ms, uint32_t flags, void *arg);

		/**
		 * @brief Construct a new timer.
		 * @param name Timer name.
		 * @param ms Interval.
		 * @param flags Timer flags.
		 * @param arg Timer handler argument.
		 */
		explicit Timer(const char *name, unsigned long ms, uint32_t flags, void *arg);

		explicit Timer(const Timer&) = delete;

		/**
		 * @brief Construct a new timer.
		 * @param other Timer to move into \p *this.
		 */
		explicit Timer(Timer&& other) noexcept;
		virtual ~Timer();

		Timer& operator=(const Timer&) = delete;

		/**
		 * @brief Move assignment operator.
		 * @param rhs Timer to move into \p *this.
		 */
		Timer& operator=(Timer&& rhs) noexcept;

		void start(); //!< Start a timer.
		void stop(); //!< Stop a timer.

		/**
		 * @brief Check if a timer has already expired.
		 * @return Expiry indicator.
		 */
		bool isExpired();
		void reset(); //!< Reset a timer.

		/**
		 * @brief Get the expiry timestamp.
		 * @return The timer's expiry timestamp.
		 */
		time_t expiry();

	protected:
		/**
		 * @brief Timer handler.
		 */
		virtual void tick() = 0;

		void *argument; //!< Timer argument.

	private:
		friend void run_timer(lwiot_timer_t *t, void *argument);
		lwiot_timer_t* timer;
		bool running;
	};
}
