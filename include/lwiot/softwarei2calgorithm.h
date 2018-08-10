/*
 * I2C software (i.e. bit-banged) implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/lock.h>
#include <lwiot/log.h>

namespace lwiot
{
	class SoftwareI2CAlgorithm : public I2CAlgorithm {
	public:
		explicit SoftwareI2CAlgorithm(int sda, int scl, uint32_t frequency = 100000);
		virtual ~SoftwareI2CAlgorithm();

		virtual ssize_t transfer(I2CMessage& msg) override;
		virtual ssize_t transfer(Vector<I2CMessage*>& msgs) override;

		virtual void setFrequency(const uint32_t& freq) override;

		bool test();

	protected:
		virtual int calcDelay(const uint32_t& freq) const;

	private:
		mutable GpioPin _sda;
		mutable GpioPin _scl;
		int _udelay;
		Lock _lock;
		mutable Logger log;
		int _halftime;

		static constexpr int Timeout = 10;
		static constexpr uint8_t MsbPosition = 7U;
		static constexpr uint8_t LsbValue = 1U;

		/* Methods */
		bool begin(const I2CMessage& msg);
		int begin(uint8_t addr, int retries);

		void repstart() const;
		void start() const;
		void stop() const;
		int acknack(bool ack) const;

		void sdahi() const;
		void sdalow() const;

		void scllow() const;
		int sclhi() const;

		int write(uint8_t byte);
		int write(I2CMessage& msg);

		int read(uint8_t& byte);
		int read(I2CMessage& msg);
	};
}
