/*
 * PWM module implementation.
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
	PwmTimer::PwmTimer() : PwmTimer(-1)
	{
	}

	PwmTimer::PwmTimer(int freq) : _channels(3), _freq(freq)
	{
	}

	void PwmTimer::setFrequency(const int& freq)
	{
		this->_freq = freq;

		for(auto channel : this->_channels) {
			channel->setFrequency(freq);
		}
	}

	const int& PwmTimer::frequency() const
	{
		return this->_freq;
	}

	const PwmChannel& PwmTimer::operator [](const size_t& idx) const
	{
		return *this->_channels[idx];
	}

	PwmChannel& PwmTimer::operator[](const size_t& idx)
	{
		return *this->_channels[idx];
	}

	PwmTimer::operator bool() const
	{
		return this->_freq > 0 && this->_channels.length() > 0;
	}

	void PwmTimer::addChannel(PwmChannel& channel)
	{
		this->_channels.add(&channel);
	}

	void PwmTimer::update()
	{
		if(this->_channels.length() == 0)
			return;

		for(auto channel : this->_channels) {
			channel->update(this->_freq);
		}
	}
}
