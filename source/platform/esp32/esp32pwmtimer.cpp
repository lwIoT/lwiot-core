/*
 * ESP32 PWM implementation.
 * 
 * @author MIchel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/io/pwm.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>

#include <lwiot/esp32/esp32pwm.h>

namespace lwiot { namespace esp32
{
	PwmTimer::PwmTimer(int num, mcpwm_unit_t unit, int freq) :
		lwiot::PwmTimer(freq), _unit(unit), _num(num)
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

		this->_channels.add(new PwmChannel(channel, *this));
		this->_channels.add(new PwmChannel(channel + 1, *this));
	}

	PwmTimer::~PwmTimer()
	{
		for(auto channel : this->_channels) {
			delete channel;
		}
	}

	mcpwm_unit_t PwmTimer::unit() const
	{
		return this->_unit;
	}
}
}
