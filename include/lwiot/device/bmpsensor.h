/*
 * BMP base header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file bmpsensor.h BMP sensor base.

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/io/i2cbus.h>

namespace lwiot
{
	/**
	 * @brief BMP sensor base class.
	 * @ingroup device
	 */
	class BmpSensor {
	public:
		/**
		 * @brief Construct a new BMP sensor driver object.
		 * @param addr I2C slave address.
		 * @param bus  I2C bus.
		 */
		explicit BmpSensor(uint8_t addr, I2CBus& bus);
		virtual ~BmpSensor() = default;

	protected:
		/**
		 * @brief Read 24 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 */
		virtual uint32_t read24(uint8_t reg);

		/**
		 * @brief Read 16 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 */
		virtual uint16_t read16(uint8_t reg);

		/**
		 * @brief Read 8 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 */
		virtual uint8_t read8(uint8_t reg);

		/**
		 * @brief Read 16 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 *
		 * This member function will parse the output value as a signed integer.
		 */
		virtual int16_t readS16(uint8_t reg);

		/**
		 * @brief Read 16 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 * @brief The output is little endian.
		 */
		uint16_t read16_LE(uint8_t reg);

		/**
		 * @brief Read 16 bits from the I2C bus.
		 * @param reg Register to read from.
		 * @return Value read.
		 * @brief The output is little endian.
		 *
		 * This member function will parse the output value as a signed integer.
		 */
		int16_t readS16_LE(uint8_t reg);

		/**
		 * @brief Write a byte value to a given register.
		 * @param reg Register to write to.
		 * @param value Value to write.
		 */
		virtual void write(uint8_t reg, uint8_t value);

	private:
		uint8_t _addr;
		I2CBus _bus;

		bool read(uint8_t reg, uint8_t *rv, size_t num);
	};
}
