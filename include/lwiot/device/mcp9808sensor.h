/*
 * Driver for the MCP9808 temperature sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file mcp9808.h MCP driver.

#pragma once

#include <lwiot.h>

#include <lwiot/io/i2cmessage.h>
#include <lwiot/io/i2cbus.h>

#include <lwiot/kernel/lock.h>

#include <lwiot/kernel/uniquelock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @file MCP9808 driver.
	 * @ingroup device
	 */
	class MCP9808Sensor {
	public:
		/**
		 * @brief Construct a new driver object.
		 */
		explicit MCP9808Sensor();

		/**
		 * @brief MCP9808 driver destructor.
		 */
		~MCP9808Sensor() = default;

		/**
		 * @brief Start the MCP9808 driver.
		 * @param bus I2C bus.
		 * @param addr I2C slave address.
		 * @return Success indicator.
		 */
		bool begin(I2CBus& bus, uint8_t addr = I2C_ADDRESS);

		/**
		 * @brief Send a wake up to the sensor.
		 */
		void wake();

		/**
		 * @brief Stop the sensor.
		 */
		void shutdown();

		/**
		 * @brief Read the temperature from the sensor.
		 * @return The current temperature.
		 */
		float read();

		/**
		 * @brief Set the sensor resolution.
		 * @param resolution Resolution value.
		 */
		void setResolution(int resolution);

		/**
		 * @brief I2C slave address.
		 */
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
