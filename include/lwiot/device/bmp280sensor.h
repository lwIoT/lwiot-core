/*
 * BMP280 barometric pressure sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/device/bmpsensor.h>

typedef int32_t BMP280_S32_t;
typedef uint32_t BMP280_U32_t;

typedef int64_t BMP280_S64_t;

namespace lwiot
{
	typedef enum {
		BMP280_REGISTER_DIG_T1              = 0x88,
		BMP280_REGISTER_DIG_T2              = 0x8A,
		BMP280_REGISTER_DIG_T3              = 0x8C,

		BMP280_REGISTER_DIG_P1              = 0x8E,
		BMP280_REGISTER_DIG_P2              = 0x90,
		BMP280_REGISTER_DIG_P3              = 0x92,
		BMP280_REGISTER_DIG_P4              = 0x94,
		BMP280_REGISTER_DIG_P5              = 0x96,
		BMP280_REGISTER_DIG_P6              = 0x98,
		BMP280_REGISTER_DIG_P7              = 0x9A,
		BMP280_REGISTER_DIG_P8              = 0x9C,
		BMP280_REGISTER_DIG_P9              = 0x9E,

		BMP280_REGISTER_CHIPID             = 0xD0,
		BMP280_REGISTER_VERSION            = 0xD1,
		BMP280_REGISTER_SOFTRESET          = 0xE0,

		BMP280_REGISTER_CAL26              = 0xE1,

		BMP280_REGISTER_CONTROL            = 0xF4,
		BMP280_REGISTER_CONFIG             = 0xF5,
		BMP280_REGISTER_PRESSUREDATA       = 0xF7,
		BMP280_REGISTER_TEMPDATA           = 0xFA,
	} bmp280_register_t;

	namespace bmp280
	{
		struct CallibrationData {
			uint16_t dig_T1;
			int16_t  dig_T2;
			int16_t  dig_T3;

			uint16_t dig_P1;
			int16_t  dig_P2;
			int16_t  dig_P3;
			int16_t  dig_P4;
			int16_t  dig_P5;
			int16_t  dig_P6;
			int16_t  dig_P7;
			int16_t  dig_P8;
			int16_t  dig_P9;
		};
	}

	class Bmp280Sensor : public BmpSensor {
	public:
		explicit Bmp280Sensor(I2CBus& bus);
		virtual ~Bmp280Sensor() = default;

		void begin();

		void read(int32_t & pressure, float& temperature);
		void read();

		const int32_t & pressure() const;
		const float& temperature() const;

	private:
		int32_t _pressure;
		float _temperature;
		bmp280::CallibrationData _data;

		mutable bool _calibrate;
		mutable int32_t _tfine;

		static constexpr int8_t SlaveAddress = 0x77;
		static constexpr uint8_t ChipId = 0x58;

		inline int32_t readAdcTemperature();
		inline int32_t readAdcPressure();

		/* Methods */
		int32_t computeTemperature(int32_t input) const;
		int32_t computePressure(int32_t input);
	};
}
