/*
 * 24C02 EEPROM header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file eeprom24c02.h EEPROM driver.

#pragma once

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief EEPROM driver.
	 * @ingroup device
	 */
	class Eeprom24C02 {
	public:
		/**
		 * @brief Construct a new EEPROM object.
		 * @param bus I2C bus.
		 */
		explicit Eeprom24C02(I2CBus& bus);

		/**
		 * @brief EEPROM destructor.
		 */
		~Eeprom24C02() = default;

		/**
		 * @brief Read from EEPROM.
		 * @param addr Address to read from.
		 * @return Value at \p addr.
		 */
		uint8_t read(uint8_t addr);

		/**
		 * @brief Write a single byte to EEPROM.
		 * @param addr Address to write to.
		 * @param byte Value to write.
		 */
		void write(uint8_t addr, uint8_t byte);

		/**
		 * @brief Read from EEPROM.
		 * @param addr Address to read from.
		 * @param data Data buffer to read into.
		 * @param length Lengthof \p data.
		 * @return Number of bytes read.
		 */
		ssize_t read(uint8_t addr, void *data, size_t length);

		/**
		 * @brief Write data to \p addr.
		 * @param addr EEPROM address to write to.
		 * @param data Data to write.
		 * @param length Length of \p data.
		 * @return Number of bytes written.
		 */
		ssize_t write(uint8_t addr, const void *data, size_t length);

	private:
		I2CBus& _bus;
		Lock _lock;

		static constexpr int8_t PageSize = 8;
		static constexpr int8_t SlaveAddress = 0x50;
	};
}
