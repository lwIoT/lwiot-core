/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>
#include <math.h>

#include <lwiot/system.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/rgbleddriver.h>
#include <lwiot/io/leddriver.h>
#include <lwiot/io/watchdog.h>

#define FIX_DIF(_d_) if(_d_ < 0.0) _d_ *= -1.0

namespace lwiot
{
	RgbLedDriver::RgbLedDriver(lwiot::PwmChannel &r, lwiot::PwmChannel &g, lwiot::PwmChannel &b) :
			_red(r), _green(g), _blue(b), _r(0), _g(0), _b(0)
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

	void RgbLedDriver::fade(bool out, int ms, double stepsize)
	{
		double r,g,b;

		if(out) {
			this->_r = this->_red.brightness();
			this->_g = this->_green.brightness();
			this->_b = this->_blue.brightness();

			r = g = b = 0.0;
		} else {
			r = this->_r;
			g = this->_g;
			b = this->_b;
		}

		this->fade(r, g, b, ms, stepsize);
	}

	void RgbLedDriver::fade(double r_percent, double g_percent, double b_percent, int ms, double stepsize)
	{
		double rdiff;
		double gdiff;
		double bdiff;
		double rstep, gstep, bstep;

		rdiff = r_percent - this->_red.brightness();
		gdiff = g_percent - this->_green.brightness();
		bdiff = b_percent - this->_blue.brightness();

		rstep = rdiff / stepsize;
		gstep = gdiff / stepsize;
		bstep = bdiff / stepsize;

		FIX_DIF(rdiff);
		FIX_DIF(gdiff);
		FIX_DIF(bdiff);

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

	void RgbLedDriver::fade(uint8_t r, uint8_t g, uint8_t b, int ms, double stepsize)
	{
		double r_percent;
		double g_percent;
		double b_percent;

		r_percent = (static_cast<double>(r) / UINT8_MAX) * 100.0;
		g_percent = (static_cast<double>(g) / UINT8_MAX) * 100.0;
		b_percent = (static_cast<double>(b) / UINT8_MAX) * 100.0;

		this->fade(r_percent, g_percent, b_percent, ms, stepsize);
	}

	void RgbLedDriver::updateDiffValue(double &diff, double &step)
	{
		auto tmp = (step < 0) ? step : step * -1.0;

		if((diff + tmp) <= 0.0) {
			if(step < 0)
				step = diff * -1.0;
			else
				step = diff;

			diff = 0.0;
		} else {
			diff += tmp;
		}
	}

	/*
	 * This implementation is based on:
	 *
	 *  -> https://github.com/BretStateham/RGBLED/blob/master/RGBLED.cpp
	 *  -> http://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c/
	 */
	void RgbLedDriver::setHSV(int h, double s, double v)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		auto i = static_cast<int>(::floor(h / 60.0));

		double f = h / 60.0 - i;
		double pv = v * (1 - s);
		double qv = v * (1 - s * f);
		double tv = v * (1 - s * (1 - f));

		switch(i) {
		case 0:
			r = v;
			g = tv;
			b = pv;
			break;

		case 1:
			r = qv;
			g = v;
			b = pv;
			break;

		case 2:
			r = pv;
			g = v;
			b = tv;
			break;
		case 3:
			r = pv;
			g = qv;
			b = v;
			break;

		case 4:
			r = tv;
			g = pv;
			b = v;
			break;

		case 5:
			r = v;
			g = pv;
			b = qv;
			break;

		default:
			print_dbg("Invalid HSV value!\n");
			return;
		}

		r *= 255;
		g *= 255;
		b *= 255;

		clamp(r, 0, 255);
		clamp(g, 0, 255);
		clamp(b, 0, 255);

		auto red = static_cast<uint8_t> (r);
		auto green = static_cast<uint8_t> (g);
		auto blue = static_cast<uint8_t> (b);

		this->set(red, green, blue);
	}

	void RgbLedDriver::rotate(int ms)
	{
		for(int idx = 0; idx < 360; idx++) {
			this->setHSV(idx, 1, 1);
			wdt.reset();
			System::delay(ms);
		}
	}
}
