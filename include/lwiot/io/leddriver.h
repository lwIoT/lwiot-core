/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	class LedDriver {
	public:
		explicit LedDriver(double brightness = 0.0);
		explicit LedDriver(PwmChannel& channel, double brightness = 0.0);

		void setChannel(PwmChannel& channel);
		void setBrightness(double brightness);

		void fade(double target, int ms, int step_size = 50);

		constexpr double brightness() const
		{
			return this->_brightness;
		}

		constexpr PwmChannel& channel() const
		{
			return this->_channel.get();
		}

	private:
		stl::ReferenceWrapper<PwmChannel> _channel;
		double _brightness;
	};
}
