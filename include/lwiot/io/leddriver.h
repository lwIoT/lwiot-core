/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file leddriver.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief LED driver.
	 * @ingroup io
	 */
	class LedDriver {
	public:
		/**
		 * @brief Construct a new LedDriver object.
		 */
		constexpr LedDriver() : _brightness(0.0)
		{ }

		/**
		 * @brief Construct a new LedDriver object.
		 * @param brightness Brightness to set.
		 */
		explicit LedDriver(double brightness);

		/**
		 * @brief Construct a new LedDriver object.
		 * @param channel PWM channel reference.
		 * @param brightness Brightness to set.
		 */
		explicit LedDriver(PwmChannel& channel, double brightness = 0.0);

		/**
		 * @brief Set the PWM channel.
		 * @param channel PWM channel.
		 */
		void setChannel(PwmChannel& channel);

		/**
		 * @brief Set the LED brightness.
		 * @param brightness Brightness to set.
		 */
		void setBrightness(double brightness);

		/**
		 * @brief Fade the LED brightness.
		 * @param target Target brightness.
		 * @param ms Step delay.
		 * @param step_size Brightness step size.
		 */
		void fade(double target, int ms, int step_size = 50);

		/**
		 * @brief Getter for the brightness.
		 * @return The LED brightness.
		 */
		constexpr double brightness() const
		{
			return this->_brightness;
		}

		/**
		 * @brief Getter for the PWM channel.
		 * @return The PWM channel.
		 */
		constexpr PwmChannel& channel() const
		{
			return this->_channel.get();
		}

	private:
		stl::ReferenceWrapper<PwmChannel> _channel;
		double _brightness;
	};
}
