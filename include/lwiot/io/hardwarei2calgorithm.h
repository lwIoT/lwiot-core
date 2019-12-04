/*
 * Hardware I2C algorithm definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file hardwarei2calgorithm.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief Hardware I2C algorithm base class.
	 * @ingroup io
	 */
	class HardwareI2CAlgorithm : public I2CAlgorithm {
	public:
		/**
		 * @brief Construct a new hardware I2C algorithm object.
		 * @param scl SCL pin.
		 * @param sda SDA pin.
		 * @param frequency Frequency to operate at.
		 */
		explicit HardwareI2CAlgorithm(int scl, int sda, uint32_t frequency = DefaultSclFrequency);

		/**
		 * @brief Construct a new hardware I2C algorithm object.
		 * @param scl SCL pin.
		 * @param sda SDA pin.
		 * @param frequency Frequency to operate at.
		 */
		explicit HardwareI2CAlgorithm(const GpioPin& scl, const GpioPin& sda, uint32_t frequency);

		/**
		 * @brief Construct a new hardware I2C algorithm object.
		 * @param scl SCL pin.
		 * @param sda SDA pin.
		 */
		explicit HardwareI2CAlgorithm(const GpioPin& scl, const GpioPin& sda);

		/**
		 * @brief Move constructor.
		 * @param other Algorithm to move into \p *this.
		 */
		HardwareI2CAlgorithm(HardwareI2CAlgorithm&& other) noexcept;

		/**
		 * @brief Copy constructor.
		 * @param other Algorithm to copy into \p *this.
		 */
		HardwareI2CAlgorithm(const HardwareI2CAlgorithm& other) noexcept;

		~HardwareI2CAlgorithm() override; //!< Algorithm destructor.

		/**
		 * @brief Move assignment operator.
		 * @param rhs Algorithm to move into \p *this.
		 */
		HardwareI2CAlgorithm& operator=(HardwareI2CAlgorithm&& rhs) noexcept;

		/**
		 * @brief Copy assignment operator.
		 * @param other Algorithm to copy into \p *this.
		 */
		HardwareI2CAlgorithm& operator=(const HardwareI2CAlgorithm& other) noexcept;

		/**
		 * @brief Initiate a data transfer.
		 * @param msg I2C message.
		 * @return Number of bytes transfered.
		 */
		ssize_t transfer(I2CMessage &msg) override;

		/**
		 * @brief Initiate a data transfer.
		 * @param msgs I2C messages.
		 * @return Number of bytes transfered.
		 */
		ssize_t transfer(stl::Vector<I2CMessage> &msgs) override;

		/**
		 * @brief Set the bus frequency.
		 * @param freq Frequency to set.
		 */
		void setFrequency(const uint32_t& freq) override;

		/**
		 * @brief Test the I2C bus.
		 * @return Success indicator.
		 */
		bool test();

		/**
		 * @brief Test if the I2C bus is busy.
		 * @return Busy indicator.
		 */
		bool busy();

	protected:
		static constexpr uint32_t DefaultSclFrequency = 100000; //!< Default frequency.
		static constexpr int Timeout = 10; //!< Default timeout.

		/**
		 * @brief Flush the bus.
		 * @return Success indicator.
		 */
		bool transfer() const;

		/**
		 * @brief Test if the I2C bus is busy.
		 * @return Busy indicator.
		 */
		bool isBusy() const;

		/**
		 * @brief Move operation.
		 * @param other Object to move.
		 */
		virtual void move(I2CAlgorithm& other);

		/**
		 * @brief Copy operation.
		 * @param other Object to copy.
		 */
		void copy(const I2CAlgorithm& other) override;

		/**
		 * @brief Send a start condition.
		 * @param sla Slave address.
		 * @param repeated Repeated start.
		 */
		virtual void start(uint16_t sla, bool repeated) = 0;

		/**
		 * @brief Send a stop condition.
		 */
		virtual void stop() = 0;

		/**
		 * @brief Reset the bus.
		 */
		virtual void reset() = 0;

		/**
		 * @brief Write a single byte.
		 * @param byte Data to write.
		 * @param ack Acknowledge indicator.
		 */
		virtual void write(const uint8_t *byte, bool ack) = 0;

		/**
		 * @brief Write data.
		 * @param bytes Data to write.
		 * @param length Length of \p data.
		 * @param ack Acknowledge indicator.
		 */
		virtual void write(const uint8_t *bytes, size_t length, bool ack) = 0;

		/**
		 * @brief Read data from the bus.
		 * @param byte Input buffer.
		 * @param ack Acknowledge indicator.
		 */
		virtual void read(uint8_t* byte, bool ack) = 0;

		/**
		 * @brief Read data from the bus.
		 * @param bytes Input buffer.
		 * @param length Length of \p bytes.
		 * @param ack Acknowledge indicator.
		 */
		virtual void read(uint8_t* bytes, size_t length, bool ack) = 0;

		/**
		 * @brief Flush the bus.
		 * @return Error code.
		 */
		virtual int flush() const = 0;

	private:
		static constexpr int MaxRiseTime = 20;

		mutable GpioPin _scl;
		mutable GpioPin _sda;
		mutable Logger log;
		SharedPointer<Lock> _lock;

		void sdahi() const;
		void sdalow() const;

		void scllow() const;
		int sclhi() const;
	};
}
