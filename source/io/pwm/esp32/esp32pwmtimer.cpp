/*
 * ESP32 PWM implementation.
 * 
 * @author MIchel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/pwm.h>
#include <lwiot/vector.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

#include <lwiot/esp32/esp32pwm.h>

namespace lwiot
{
	Esp32PwmTimer::Esp32PwmTimer(int num, mcpwm_unit_t unit, int freq) :
		PwmTimer(freq), _unit(unit), _num(num)
	{
		int channel;

		switch(num) {
		case 0:
			channel = 0;
			break;

		case 1:
			channel = 2;
			break;

		case 2:
			channel = 4;
			break;

		default:
			return;
		}

		this->_channels.add(new Esp32PwmChannel(channel, *this));
		this->_channels.add(new Esp32PwmChannel(channel + 1, *this));
	}

	Esp32PwmTimer::~Esp32PwmTimer()
	{
		for(auto channel : this->_channels) {
			delete channel;
		}
	}

	mcpwm_unit_t Esp32PwmTimer::unit() const
	{
		return this->_unit;
	}
}
