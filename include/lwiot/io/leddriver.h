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

		double brightness() const;

	private:
		stl::ReferenceWrapper<PwmChannel> _channel;
		double _brightness;
	};
}
