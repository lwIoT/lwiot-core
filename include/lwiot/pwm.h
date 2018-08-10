/*
 * PWM base class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/vector.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>

namespace lwiot
{
	class PwmTimer;

	class PwmChannel {
	public:
		explicit PwmChannel();
		explicit PwmChannel(int pin);
		explicit PwmChannel(const GpioPin& pin);
		virtual ~PwmChannel() = default;

		const double& duty() const;
		void setDutyCycle(const double& duty);

		virtual void setGpioPin(const GpioPin& pin);

		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual void reload() = 0;
		operator bool() const;

	protected:
		mutable unsigned int freq_cache;

		/* Methods */
		virtual void setFrequency(int frequency) const;
		virtual void update(int freq) = 0;
		const GpioPin& pin() const;
		friend class PwmTimer;

	private:
		/* Attributes */
		GpioPin _pin;
		double _duty;
	};

	class PwmTimer {
	public:
		explicit PwmTimer();
		explicit PwmTimer(int freq);
		virtual ~PwmTimer() = default;

		const int& frequency() const;
		virtual void setFrequency(const int &freq);
		virtual void update();

		PwmChannel& operator [](const size_t& index);
		const PwmChannel& operator [](const size_t& index) const;
		operator bool() const;

	protected:
		void addChannel(PwmChannel& channel);
		Vector<PwmChannel*> _channels;
		friend class PwmChannel;

	private:
		int _freq;
	};
}
