/*
 * PWM led brightness driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/leddriver.h>

namespace lwiot
{
	LedDriver::LedDriver(double brightness) : _channel(), _brightness(brightness)
	{
		if(brightness < 0)
			this->_brightness = 0.0;
	}

	LedDriver::LedDriver(lwiot::PwmChannel &channel, double brightness) : _channel(channel), _brightness(brightness)
	{
		if(brightness < 0.0)
			this->_brightness = 0;

		channel.setDutyCycle(this->_brightness);
		this->_channel->reload();
	}

	void LedDriver::fade(double target, int ms, int step_size)
	{
		double diff = target - this->_brightness;
		double step;
		bool neg = false;

		/* Reduce brightness if diff < 0 */
		if(diff < 0.0) {
			diff *= -1.0;
			neg = true;
		}

		step = diff / step_size;

		while(diff >= 0.0 && target != this->_brightness) {
			if(neg)
				this->_brightness -= step;
			else
				this->_brightness += step;

			this->_channel->setDutyCycle(this->_brightness);
			this->_channel->reload();
			lwiot_sleep(ms);

			if(diff == 0.0)
				break;

			if(diff > 0.0) {
				diff -= step;
			} else {
				step = diff;
				diff = 0;
			}
		}
	}

	void LedDriver::setChannel(lwiot::PwmChannel &channel)
	{
		this->_channel = channel;
		channel.setDutyCycle(this->_brightness);
		this->_channel->reload();
	}

	void LedDriver::setBrightness(double brightness)
	{
		if(brightness < 0.0)
			brightness = 0.0;

		this->_brightness = brightness;
		this->_channel->setDutyCycle(brightness);
		this->_channel->reload();
	}
}
