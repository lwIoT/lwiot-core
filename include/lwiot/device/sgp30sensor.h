/*
 * SGP30 sensor module.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sgp30sensor.h SGP30 sensor.

#pragma once

#include <lwiot/io/i2cbus.h>
#include <lwiot/stl/referencewrapper.h>

#define SGP30_CRC8_POLYNOMIAL 0x31
#define SGP30_CRC8_INIT 0xFF

namespace lwiot
{
	/**
	 * @brief SGP30 sensor driver.
	 * @ingroup device
	 */
	class Sgp30Sensor {
	public:
		/**
		 * @brief Construct a SGP30 driver object.
		 */
		explicit Sgp30Sensor() : _bus(), _tvoc(0), _co2(0)
		{ }

		/**
		 * @brief Construct a SGP30 sensor object.
		 * @param bus I2C bus.
		 */
		explicit Sgp30Sensor(I2CBus& bus);

		/**
		 * @brief Start the driver.
		 */
		void begin();

		/**
		 * @brief Set the humidity value.
		 * @param abs Absolute humidity value.
		 * @return A success indicator.
		 */
		bool setHumidity(uint32_t abs);

		/**
		 * @brief Sample the SGP30 sensor.
		 * @return A success indicator.
		 */
		bool measure();

		/**
		 * @brief Get the last TVOC sample value.
		 * @return The last TVOC value.
		 */
		inline uint16_t tvoc() const
		{
			return this->_tvoc;
		}

		/**
		 * @brief Get the last eCO2 sample value.
		 * @return The last eCO2 value.
		 */
		inline uint16_t co2() const
		{
			return this->_co2;
		}

	private:
		stl::ReferenceWrapper<I2CBus> _bus;

		uint16_t _tvoc;
		uint16_t _co2;

		static constexpr int SGP30_I2CADDR_DEFAULT = 0x58;
		static constexpr int SGP30_FEATURESET = 0x0020;
		static constexpr int SGP30_WORD_LEN = 2;

		bool init();
		bool read(uint8_t cmd[], int len, uint16_t ms, uint16_t *result = nullptr, int readlen = 0);
	};
}
