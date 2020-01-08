/*
 * PWM channel implementation.
 * 
 * @author Michel Megens
 * @email dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/pwm.h>


namespace lwiot
{
	PwmChannel::PwmChannel() : freq_cache(), _pin(-1), _duty()
	{}

	PwmChannel::PwmChannel(int pin) : freq_cache(), _pin(pin), _duty()
	{ }

	PwmChannel::PwmChannel(const GpioPin& pin) : freq_cache(), _pin(pin), _duty()
	{ }

	const double& PwmChannel::duty() const
	{
		return this->_duty;
	}

	void PwmChannel::setDutyCycle(const double& duty)
	{
		this->_duty = duty;
	}

	void PwmChannel::setGpioPin(const GpioPin& pin)
	{
		this->_pin = pin;
	}

	const GpioPin& PwmChannel::pin() const
	{
		return this->_pin;
	}

	PwmChannel::operator bool() const
	{
		return this->_pin.pin() >= 0;
	}

	void PwmChannel::setFrequency(int freq) const
	{
		this->freq_cache =  freq;
	}
}
