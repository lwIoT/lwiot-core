/*
 * SGP30 sensor module.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/io/i2cbus.h>
#include <lwiot/stl/referencewrapper.h>

#define SGP30_CRC8_POLYNOMIAL 0x31
#define SGP30_CRC8_INIT 0xFF

namespace lwiot
{
	class Sgp30Sensor {
	public:
		constexpr Sgp30Sensor() : _bus(), _tvoc(0), _co2(0)
		{ }

		explicit Sgp30Sensor(I2CBus& bus);

		void begin();
		bool setHumidity(uint32_t abs);
		bool measure();

		constexpr uint16_t tvoc() const
		{
			return this->_tvoc;
		}

		constexpr uint16_t co2() const
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
