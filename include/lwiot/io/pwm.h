/*
 * PWM base class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file pwm.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>

namespace lwiot
{
	class PwmTimer;

	/**
	 * @brief PWM channel.
	 * @ingroup io
	 */
	class PwmChannel {
	public:
		/**
		 * @brief Construct a PWM channel.
		 */
		explicit PwmChannel();

		/**
		 * @brief Construct a PWM channel.
		 * @param pin Output pin.
		 */
		explicit PwmChannel(int pin);

		/**
		 * @brief Construct a PWM channel.
		 * @param pin Output pint.
		 */
		explicit PwmChannel(const GpioPin& pin);
		virtual ~PwmChannel() = default; //!< PWM channel destructor.

		/**
		 * @brief Get the duty cycle.
		 * @return The duty cycle.
		 */
		const double& duty() const;

		/**
		 * @brief Set the duty cycle.
		 * @param duty Duty cycle to set.
		 */
		void setDutyCycle(const double& duty);

		/**
		 * @brief Set the output pin.
		 * @param pin The output pin to set.
		 */
		virtual void setGpioPin(const GpioPin& pin);

		virtual void enable() = 0; //!< Enable the channel.
		virtual void disable() = 0; //!< Disable the channel.
		virtual void reload() = 0; //!< Reload the channel.

		/**
		 * @brief Check the validity of the channel.
		 * @return Success indicator.
		 */
		operator bool() const;

	protected:
		mutable unsigned int freq_cache; //!< Cached frequency value.

		/* Methods */

		/**
		 * @brief Set the frequency.
		 * @param frequency Frequency value.
		 */
		virtual void setFrequency(int frequency) const;

		/**
		 * @brief Update the PWM channel.
		 * @param freq Frequency to set.
		 */
		virtual void update(int freq) = 0;

		/**
		 * @brief Get the output pin.
		 * @return The PWM output pin.
		 */
		const GpioPin& pin() const;
		friend class PwmTimer;

	private:
		/* Attributes */
		GpioPin _pin;
		double _duty;
	};

	/**
	 * @brief PWM timer object.
	 * @ingroup io
	 */
	class PwmTimer {
	public:
		/**
		 * @brief Construct a new PWM timer.
		 */
		explicit PwmTimer();

		/**
		 * @brief Construct a new PWM timer.
		 * @param freq Timer frequency.
		 */
		explicit PwmTimer(int freq);
		virtual ~PwmTimer() = default;

		/**
		 * @brief Get the timer frequency.
		 * @return The timer frequency.
		 */
		const int& frequency() const;

		/**
		 * @brief Set the timer frequency.
		 * @param freq The timer frequency.
		 */
		virtual void setFrequency(const int &freq);

		/**
		 * @brief Update the PWM timer.
		 */
		virtual void update();

		/**
		 * @brief Channel accessor.
		 * @param index Channel to access.
		 * @return A reference to the channel at \p index.
		 */
		PwmChannel& operator [](const size_t& index);

		/**
		 * @brief Channel accessor.
		 * @param index Channel to access.
		 * @return A reference to the channel at \p index.
		 */
		const PwmChannel& operator [](const size_t& index) const;

		/**
		 * @brief Boolean conversion operator.
		 * @return Validity indicator.
		 */
		operator bool() const;

	protected:
		/**
		 * @brief Add a PWM channel.
		 * @param channel Channel to add.
		 */
		void addChannel(PwmChannel& channel);

		stl::Vector<PwmChannel*> _channels; //!< Available PWM channels.
		friend class PwmChannel;

	private:
		int _freq;
	};
}
