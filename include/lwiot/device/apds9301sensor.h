/*
 * TSL2561 light sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file apds9301sensor.h

#pragma once

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief APDS 9301 driver.
	 * @ingroup device
	 */
	class Apds9301Sensor {
	public:
		/**
		 * @brief Construct a new object.
		 * @param bus I2C bus object.
		 * @param addr Slave address.
		 */
		explicit Apds9301Sensor(I2CBus& bus, uint8_t addr = SlaveAddress);
		~Apds9301Sensor() = default;

		/**
		 * @brief Enable the sensor.
		 * @return Success indicator.
		 */
		bool on();

		/**
		 * @brief Disable the sensor.
		 * @return Success indicator.
		 */
		bool off();

		/**
		 * @brief Start the device driver.
		 * @return A success indicator.
		 */
		bool begin();

		/**
		 * @brief Set the gain level.
		 * @param level Gain level.
		 * @return A success indicator.
		 */
		bool setGain(bool level);

		/**
		 * @brief Get the lux value.
		 * @param lux Output value.
		 * @return Success indicator.
		 */
		bool getLux(double& lux);

		/**
		 * @brief Get the gain level.
		 * @param gain The gain level.
		 * @return A success indicator.
		 */
		bool getGain(bool& gain);

		/**
		 * @brief Get the device ID.
		 * @return The device ID.
		 */
		uint8_t id();

		/**
		 * @brief Integration time setting.
		 */
		typedef enum {
			INT_TIME_13_7_MS, // 13.7ms
			INT_TIME_101_MS,  // 101ms
			INT_TIME_402_MS   // 402ms
		} integration_time_t;

		/**
		 * @brief Get the integration time.
		 * @return The integration time.
		 */
		integration_time_t getIntegrationTime();

		/**
		 * @brief Set the integration time.
		 * @param time The integration time to set.
		 * @return A success indicator.
		 */
		bool setIntegrationTime(integration_time_t time);

	private:
		I2CBus _bus;
		uint8_t _addr;

		static constexpr int SlaveAddress = 0x39;

		/* Methods */
		bool getData(uint16_t &x, uint16_t &y);

		bool read(uint8_t addr, uint8_t& byte);
		bool read(uint8_t addr, uint16_t& word);

		bool write(uint8_t addr, uint8_t byte);
		bool write(uint8_t addr, uint16_t word);
	};
}
