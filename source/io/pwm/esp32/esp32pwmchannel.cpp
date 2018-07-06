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

#include <soc/rtc.h>
#include <soc/mcpwm_reg.h>
#include <soc/mcpwm_struct.h>

#include <driver/mcpwm.h>

static mcpwm_io_signals_t pwm_channel_map[] = {
	MCPWM0A, MCPWM0B,
	MCPWM1A, MCPWM1B,
	MCPWM2A, MCPWM2B
};

static mcpwm_config_t pwm_config[] = {
	{0}, {0}, {0}, /* First unit */
	{0}, {0}, {0} /* Second unit */
};

namespace lwiot
{
	Esp32PwmChannel::Esp32PwmChannel(int num, Esp32PwmTimer& timer) :
		PwmChannel(-1), _timer(timer)
	{
		if((unsigned)num >= sizeof(pwm_channel_map))
			return;

		this->_iopin = pwm_channel_map[num];
	}

	void Esp32PwmChannel::update(int freq)
	{
		auto num = static_cast<mcpwm_timer_t>(this->_timer._num);
		auto _operator = this->toOperator();

		mcpwm_set_frequency(this->_timer.unit(), num, freq);
		mcpwm_set_duty(this->_timer.unit(), num, _operator, this->duty());
	}

	mcpwm_operator_t Esp32PwmChannel::toOperator() const
	{
		mcpwm_operator_t _operator;

		switch(this->_iopin) {
		default:
		case MCPWM0A:
		case MCPWM1A:
		case MCPWM2A:
			_operator = MCPWM_OPR_A;
			break;

		case MCPWM0B:
		case MCPWM1B:
		case MCPWM2B:
			_operator = MCPWM_OPR_B;
			break;
		}

		return _operator;
	}

	void Esp32PwmChannel::enable()
	{
		auto num = static_cast<mcpwm_timer_t>(this->_timer._num);
		mcpwm_set_duty(this->_timer.unit(), num, this->toOperator(), this->duty());
	}

	void Esp32PwmChannel::reload()
	{
		auto num = static_cast<mcpwm_timer_t>(this->_timer._num);

		mcpwm_set_frequency(this->_timer.unit(), num, this->freq_cache);
		mcpwm_set_duty(this->_timer.unit(), num, this->toOperator(), this->duty());
	}

	void Esp32PwmChannel::disable()
	{
		auto num = static_cast<mcpwm_timer_t>(this->_timer._num);
		mcpwm_set_duty(this->_timer.unit(), num, this->toOperator(), this->duty());
	}

	void Esp32PwmChannel::setGpioPin(const GpioPin& pin)
	{
		int idx = (int)this->_timer.unit() + this->_timer._num;

		auto config = &pwm_config[idx];

		PwmChannel::setGpioPin(pin);
		config->cmpr_a = config->cmpr_b = 0.0f;
		config->frequency = 50;
		config->counter_mode = MCPWM_UP_COUNTER;
		config->duty_mode = MCPWM_DUTY_MODE_0;

		mcpwm_gpio_init(this->_timer.unit(), this->_iopin, pin.pin());
		mcpwm_init(this->_timer.unit(), static_cast<mcpwm_timer_t>(this->_timer._num), config);
	}
}
