/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file rgbleddriver.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/leddriver.h>
#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>

namespace lwiot
{
	/**
	 * @brief RGB LED driver.
	 * @ingroup io
	 */
	class RgbLedDriver {
	public:
		/**
		 * @brief Construct a new RGB LED driver.
		 */
		constexpr RgbLedDriver() : _r(0), _g(0), _b(0)
		{
		}

		/**
		 * @brief Construct a new RGB LED driver.
		 * @param r Initial red value.
		 * @param g Initial green value.
		 * @param b Initial blue value.
		 */
		explicit RgbLedDriver(PwmChannel &r, PwmChannel &g, PwmChannel &b);

		/**
		 * @brief Construct a new RGB LED driver.
		 * @param other RgbLedDriver to copy.
		 */
		RgbLedDriver(RgbLedDriver &&other) noexcept = default;

		/**
		 * @brief Construct a new RGB LED driver.
		 * @param other RgbLedDriver to copy.
		 */
		RgbLedDriver(const RgbLedDriver &other) = default;

		virtual ~RgbLedDriver() = default; //!< RGB destructor.

		/**
		 * @brief RGB assignment operator.
		 * @param rhs RgbLedDriver to copy.
		 * @return A reference to \p *this.
		 */
		RgbLedDriver &operator=(const RgbLedDriver &rhs) = default;

		/**
		 * @brief RGB assignment operator.
		 * @param rhs RgbLedDriver to copy.
		 * @return A reference to \p *this.
		 */
		RgbLedDriver &operator=(RgbLedDriver &&rhs) = default;

		/**
		 * @brief Set the LED drivers for each RGB pin.
		 * @param r Red driver.
		 * @param g Green driver.
		 * @param b Blue driver.
		 */
		void setLedDrivers(const LedDriver &r, const LedDriver &g, const LedDriver &b);

		/**
		 * @brief Set intensity levels.
		 * @param r Red value.
		 * @param g Green value.
		 * @param b Blue value.
		 */
		void set(uint8_t r, uint8_t g, uint8_t b);

		/**
		 * @brief Fade leds to a given value.
		 * @param r Target red value.
		 * @param g Target green value.
		 * @param b Target blue value.
		 * @param ms Step delay.
		 * @param stepsize Value step size.
		 */
		void fade(uint8_t r, uint8_t g, uint8_t b, int ms = 30, double stepsize = 50.0);

		/**
		 * @brief Fade the overal LED brightness.
		 * @param out Fade out/in indicator.
		 * @param ms Step delay.
		 * @param stepsize Value step size.
		 */
		void fadeBrightness(bool out, int ms = 30, double stepsize  = 50.0);

		/**
		 * @brief Set the HSV value.
		 * @param h Hue value.
		 * @param s Staturation value.
		 * @param v 'Value' value.
		 */
		void setHSV(int h, double s, double v);

		/**
		 * @brief Rotate through the HSV spectrum.
		 * @param ms Step delay.
		 */
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
