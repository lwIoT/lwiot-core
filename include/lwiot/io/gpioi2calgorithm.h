/*
 * Bit-banged GPIO I2C algorithm implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file gpioi2calgorithm.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/log.h>

namespace lwiot
{
	/**
	 * @brief GPIO (bit-banged) I2C algorithm.
	 * @ingroup io
	 */
	class GpioI2CAlgorithm : public I2CAlgorithm {
	public:
		/**
		 * @brief Construct a new algorithm object.
		 * @param sda SDA pin.
		 * @param scl SCL pin.
		 * @param frequency Frequency to operate at.
		 */
		explicit GpioI2CAlgorithm(int sda, int scl, uint32_t frequency = 100000);
		/**
		 * @brief Construct a new algorithm object.
		 * @param sda SDA pin.
		 * @param scl SCL pin.
		 * @param frequency Frequency to operate at.
		 */
		explicit GpioI2CAlgorithm(const GpioPin& sda, const GpioPin& scl, uint32_t frequency = 100000);
		~GpioI2CAlgorithm() override; //!< Destroy a GPIO I2C algorithm.

		/**
		 * @brief Initiate a data transfer.
		 * @param msg I2C message.
		 * @return Number of bytes transfered.
		 */
		ssize_t transfer(I2CMessage& msg) override;

		/**
		 * @brief Initiate a data transfer.
		 * @param msgs I2C messages.
		 * @return Number of bytes transfered.
		 */
		ssize_t transfer(stl::Vector<I2CMessage>& msgs) override;

		/**
		 * @brief Set the bus frequency.
		 * @param freq Frequency to set.
		 */
		void setFrequency(const uint32_t& freq) override;

		/**
		 * @brief Test the I2C bus.
		 * @return
		 */
		bool test();

	protected:
		/**
		 * @brief Calculate the I2C  delay.
		 * @param freq Frequency to calculate the delay for.
		 * @return The required delay to archive \p freq.
		 */
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
