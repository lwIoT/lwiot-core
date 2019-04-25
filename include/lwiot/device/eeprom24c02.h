/*
 * 24C02 EEPROM header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	class Eeprom24C02 {
	public:
		explicit Eeprom24C02(I2CBus& bus);
		~Eeprom24C02() = default;

		uint8_t read(uint8_t addr);
		void write(uint8_t addr, uint8_t byte);

		ssize_t read(uint8_t addr, void *data, size_t length);
		ssize_t write(uint8_t addr, const void *data, size_t length);

	private:
		I2CBus& _bus;
		Lock _lock;

		static constexpr int8_t PageSize = 4;
		static constexpr int8_t SlaveAddress = 0x50;
	};
}
