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
#include <lwiot/io/pwm.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>

#include <driver/mcpwm.h>

namespace lwiot { namespace esp32
{
	class PwmTimer;
	class PwmChannel : public lwiot::PwmChannel {
	public:
		explicit PwmChannel(int num, PwmTimer& timer);
		virtual ~PwmChannel() = default;

		void enable() override;
		void disable() override;
		void reload() override;
		virtual void setGpioPin(const GpioPin& pin) override;

	protected:
		virtual void update(int freq) override;

	private:
		PwmTimer& _timer;
		mcpwm_io_signals_t _iopin;

		/* Methods */
		mcpwm_operator_t toOperator() const;
	};

	class PwmTimer : public lwiot::PwmTimer {
	public:
		PwmTimer(int num, mcpwm_unit_t unit, int freq = -1);
		virtual ~PwmTimer();

		mcpwm_unit_t unit() const;

	private:
		mcpwm_unit_t _unit;
		int _num;

		friend class PwmChannel;
	};
}
}
