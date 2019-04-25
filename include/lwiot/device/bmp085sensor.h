/*
 * BMP085 driver header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/device/bmpsensor.h>

namespace lwiot
{
	class Bmp085Sensor : public BmpSensor {
	public:
		explicit Bmp085Sensor(I2CBus& bus);
		virtual ~Bmp085Sensor() = default;

		void begin();

		void read(int32_t & pressure, float& temperature);
		void read();

		int32_t pressure() const;
		float temperature() const;

	private:
		int32_t _pressure;
		float _temperature;

		mutable bool _calibrate;
		uint8_t _oversampling;

		int16_t ac1, ac2, ac3;
		uint16_t ac4, ac5, ac6;
		int16_t b1, b2;
		int16_t mb, mc, md;

		static constexpr int8_t SlaveAddress = 0x77;

		/* Methods */
		uint16_t readTemperature();
		uint32_t readPressure();
		float calculateTemperature();
		int32_t calculatePressure();
		int32_t computeB5(int32_t ut);
	};
}
