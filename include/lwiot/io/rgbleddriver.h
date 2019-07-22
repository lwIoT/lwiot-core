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
		constexpr RgbLedDriver() : _r(0), _g(0), _b(0)
		{
		}

		explicit RgbLedDriver(PwmChannel &r, PwmChannel &g, PwmChannel &b);
		explicit RgbLedDriver(RgbLedDriver &&other) noexcept = default;
		explicit RgbLedDriver(const RgbLedDriver &other) = default;

		virtual ~RgbLedDriver() = default;

		RgbLedDriver &operator=(const RgbLedDriver &rhs) = default;
		RgbLedDriver &operator=(RgbLedDriver &&rhs) = default;

		void setLedDrivers(const LedDriver &r, const LedDriver &g, const LedDriver &b);
		void set(uint8_t r, uint8_t g, uint8_t b);
		void fade(uint8_t r, uint8_t g, uint8_t b, int ms = 30, double stepsize = 50.0);
		void fade(bool out, int ms = 30, double stepsize  = 50.0);
		void setHSV(int h, double s, double v);

		void rotate(int ms = 30);

	private:
		LedDriver _red;
		LedDriver _green;
		LedDriver _blue;

		double _r, _g, _b;

		static constexpr void clamp(double &value, double min, double max)
		{
			if(value < min)
				value = min;
			else if(value > max)
				value = max;
		}

		/* Private methods */
		void updateDiffValue(double &diff, double &step);
		void fade(double r, double g, double b, int ms, double stepsize);
	};
}
