/*
 * Bit-banged GPIO I2C algorithm implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/log.h>

namespace lwiot
{
	class GpioI2CAlgorithm : public I2CAlgorithm {
	public:
		explicit GpioI2CAlgorithm(int sda, int scl, uint32_t frequency = 100000);
		explicit GpioI2CAlgorithm(const GpioPin& sda, const GpioPin& scl, uint32_t frequency = 100000);
		~GpioI2CAlgorithm() override;

		ssize_t transfer(I2CMessage& msg) override;
		ssize_t transfer(stl::Vector<I2CMessage*>& msgs) override;
		void setFrequency(const uint32_t& freq) override;

		bool test();

	protected:
		virtual int calcDelay(const uint32_t& freq) const;

	private:
		mutable GpioPin _scl;
		mutable GpioPin _sda;

		int _udelay;
		int _halftime;
		Lock _lock;
		mutable Logger log;

		static constexpr int Timeout = 10;
		static constexpr uint8_t MsbPosition = 7U;
		static constexpr uint8_t LsbValue = 1U;

		/* Methods */
		bool begin(const I2CMessage& msg);
		int begin(uint8_t addr, int retries);

		bool busy() const;

		void repstart() const;
		void start() const;
		void stop() const;
		int acknack(bool ack) const;

		void sdahi() const;
		void sdalow() const;

		void scllow() const;
		int sclhi() const;

		int write(uint8_t byte);
		int write(const I2CMessage& msg);

		int read(uint8_t& byte);
		int read(I2CMessage& msg);
	};
}
