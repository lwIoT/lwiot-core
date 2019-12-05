/*
 * SHT31 sensor driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sht31sensor.h SHT31 sensor driver

#pragma once

#include <lwiot.h>

#include <lwiot/stl/referencewrapper.h>
#include <lwiot/io/i2cbus.h>

#define SHT31_DEFAULT_ADDR         0x44
#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

namespace lwiot
{
	/**
	 * @brief SHT31 sensor driver.
	 * @ingroup device
	 */
	class Sht31Sensor {
	public:
		/**
		 * @brief SHT31 sensor.
		 */
		explicit Sht31Sensor();

		/**
		 * @brief SHT31 sensor.
		 * @param io I2C bus.
		 */
		explicit Sht31Sensor(I2CBus& io);
		~Sht31Sensor() = default;

		/**
		 * @brief Start the SHT31 driver.
		 * @return A success indicator.
		 */
		bool begin();

		/**
		 * @brief Get the latest humidity sample value.
		 * @return The humidity value.
		 */
		double humidity();

		/**
		 * @brief Get the latest temperature sample value.
		 * @return The temperature value.
		 */
		double temperature();

		/**
		 * @brief Reset the sensor.
		 */
		void reset();

		/**
		 * @brief Enable/disable the heater.
		 * @param enable Heater status.
		 */
		void setHeaterStatus(bool enable);

		/**
		 * @brief Sample the SH31.
		 */
		void measure();

		/**
		 * @brief Set the I2C bus object.
		 * @param io I2C bus.
		 */
		void setBus(I2CBus& io);

	private:
		stl::ReferenceWrapper<I2CBus> _bus;

		/* Methods */
		void writeCommaned(uint16_t cmd);

		struct ShtResult {
			double temperature;
			double humidity;
		} _result;
	};
}
