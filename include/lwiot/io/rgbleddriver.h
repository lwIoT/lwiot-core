/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/leddriver.h>
#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>

namespace lwiot
{
	class RgbLedDriver {
	public:
		explicit RgbLedDriver();
		explicit RgbLedDriver(PwmChannel& r, PwmChannel& g, PwmChannel& b);

		explicit RgbLedDriver(RgbLedDriver&& other) noexcept  = default;
		explicit RgbLedDriver(const RgbLedDriver& other)  = default;

		virtual ~RgbLedDriver() = default;

		RgbLedDriver& operator=(const RgbLedDriver& rhs) = default;
		RgbLedDriver& operator=(RgbLedDriver&& rhs) = default;

		void setLedDrivers(const LedDriver& r, const LedDriver& g, const LedDriver& b);

		void set(uint8_t r, uint8_t g, uint8_t b);
		void fade(uint8_t r, uint8_t g, uint8_t b, int ms = 30, double stepsize = 50.0);

	private:
		LedDriver _red;
		LedDriver _green;
		LedDriver _blue;

		/* Private methods */
		void updateDiffValue(double& diff, double& step);
	};
}
