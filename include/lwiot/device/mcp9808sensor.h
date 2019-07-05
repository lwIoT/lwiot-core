/*
 * Driver for the MCP9808 temperature sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/io/i2cmessage.h>
#include <lwiot/io/i2cbus.h>

#include <lwiot/kernel/lock.h>

#include <lwiot/kernel/uniquelock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	class MCP9808Sensor {
	public:
		explicit MCP9808Sensor();
		~MCP9808Sensor() = default;

		bool begin(I2CBus& bus, uint8_t addr = I2C_ADDRESS);

		void wake();
		void shutdown();
		float read();
		void setResolution(int resolution);

		static constexpr uint8_t I2C_ADDRESS = 0x18;

	private:
		stl::ReferenceWrapper<I2CBus> _bus;
		uint8_t _addr;
		Lock _lock;

	private:
		void write16(uint8_t reg, uint16_t value);
		void write8(uint8_t reg, uint8_t value);

		uint8_t read8(uint8_t reg);
		uint16_t read16(uint8_t reg);
	};
}
