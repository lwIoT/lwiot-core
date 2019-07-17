/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/rgbleddriver.h>
#include <lwiot/io/leddriver.h>

namespace lwiot
{
	RgbLedDriver::RgbLedDriver() : _red(), _green(), _blue()
	{
	}

	RgbLedDriver::RgbLedDriver(lwiot::PwmChannel &r, lwiot::PwmChannel &g, lwiot::PwmChannel &b) :
		_red(r), _green(g), _blue(b)
	{
	}

	void RgbLedDriver::setLedDrivers(const lwiot::LedDriver &r, const lwiot::LedDriver &g, const lwiot::LedDriver &b)
	{
		this->_red = r;
		this->_green = g;
		this->_blue = b;
	}

	void RgbLedDriver::set(uint8_t r, uint8_t g, uint8_t b)
	{
		double r_percent;
		double g_percent;
		double b_percent;

		r_percent = (static_cast<double>(r) / UINT8_MAX) * 100.0;
		g_percent = (static_cast<double>(g) / UINT8_MAX) * 100.0;
		b_percent = (static_cast<double>(b) / UINT8_MAX) * 100.0;

		this->_red.setBrightness(r_percent);
		this->_green.setBrightness(g_percent);
		this->_blue.setBrightness(b_percent);
	}

	void RgbLedDriver::fade(uint8_t r, uint8_t g, uint8_t b, int ms, double stepsize)
	{
		double r_percent, rdiff;
		double g_percent, gdiff;
		double b_percent, bdiff;
		double rstep, gstep, bstep;

		r_percent = (static_cast<double>(r) / UINT8_MAX) * 100.0;
		g_percent = (static_cast<double>(g) / UINT8_MAX) * 100.0;
		b_percent = (static_cast<double>(b) / UINT8_MAX) * 100.0;

		rdiff = r_percent - this->_red.brightness();
		gdiff = g_percent - this->_green.brightness();
		bdiff = b_percent - this->_blue.brightness();

		rstep = rdiff / stepsize;
		gstep = gdiff / stepsize;
		bstep = bdiff / stepsize;

		while(rdiff != 0.0 || gdiff != 0.0 || rdiff != 0.0) {
			if(rdiff != 0.0)
				this->_red.setBrightness(this->_red.brightness() + rstep);

			if(gdiff != 0.0)
				this->_green.setBrightness(this->_green.brightness() + gstep);

			if(bdiff != 0.0)
				this->_blue.setBrightness(this->_blue.brightness() + bstep);

			this->updateDiffValue(rdiff, rstep);
			this->updateDiffValue(gdiff, gstep);
			this->updateDiffValue(bdiff, bstep);
			lwiot_sleep(30);
		}
	}

	void RgbLedDriver::updateDiffValue(double &diff, double &step)
	{
		if(diff < 0.0) {
			if((diff + step) >= 0.0) {
				step = diff;
				diff = 0.0;
			} else {
				diff += step;
			}
		} else {
			if((diff - step) <= 0.0) {
				step = diff;
				diff = 0.0;
			} else {
				diff -= step;
			}
		}
	}
}
