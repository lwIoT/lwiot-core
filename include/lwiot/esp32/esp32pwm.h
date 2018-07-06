/*
 * ESP32 PWM implementation.
 * 
 * @author MIchel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/pwm.h>
#include <lwiot/vector.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

#include <driver/mcpwm.h>

namespace lwiot
{
	class Esp32PwmTimer;
	class Esp32PwmChannel : public PwmChannel {
	public:
		explicit Esp32PwmChannel(int num, Esp32PwmTimer& timer);
		virtual ~Esp32PwmChannel() = default;

		void enable() override;
		void disable() override;
		void reload() override;
		virtual void setGpioPin(const GpioPin& pin) override;

	protected:
		virtual void update(int freq) override;

	private:
		Esp32PwmTimer& _timer;
		mcpwm_io_signals_t _iopin;

		/* Methods */
		mcpwm_operator_t toOperator() const;
	};

	class Esp32PwmTimer : public PwmTimer {
	public:
		Esp32PwmTimer(int num, mcpwm_unit_t unit, int freq = -1);
		virtual ~Esp32PwmTimer();

		mcpwm_unit_t unit() const;

	private:
		mcpwm_unit_t _unit;
		int _num;

		friend class Esp32PwmChannel;
	};
}
