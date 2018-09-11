/*
 * TSL2561 light sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/i2cbus.h>
#include <lwiot/lock.h>

namespace lwiot
{
	class Apds9301Sensor {
	public:
		explicit Apds9301Sensor(I2CBus& bus, uint8_t addr = SlaveAddress);
		~Apds9301Sensor() = default;

		bool on();
		bool off();
		bool begin();

		bool setGain(bool level);

		bool getLux(double& lux);
		bool getGain(bool& gain);
		uint8_t id();

		typedef enum {
			INT_TIME_13_7_MS, INT_TIME_101_MS, INT_TIME_402_MS
		} integration_time_t;
		integration_time_t getIntegrationTime();
		bool setIntegrationTime(integration_time_t time);

	private:
		I2CBus& _bus;
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
