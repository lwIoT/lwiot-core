/*
 * DHT sensor definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file DHT sensor.

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/dhtbus.h>

namespace lwiot
{
	/**
	 * @brief DHT device type.
	 * @ingroup device
	 */
	typedef enum {
		DHT11,
		DHT22
	} dht_type_t;

	/**
	 * @brief DHT sensor driver.
	 * @ingroup device
	 */
	class DhtSensor {
	public:
		/**
		 * @brief Construct a new DHT device driver object.
		 * @param pin Data pin.
		 * @param type DHT driver type.
		 */
		explicit DhtSensor(const GpioPin& pin, dht_type_t type = DHT22);
		~DhtSensor() = default;

		/**
		 * @brief Read from the DHT bus.
		 * @param humidity Humidity value.
		 * @param temperature Temperature value.
		 * @return Success indicator.
		 */
		bool read(float& humidity, float& temperature);

		/**
		 * @brief Get the device type.
		 * @return The DHT device type this driver is configured for.
		 */
		const dht_type_t& type() const;

	private:
		DhtBus _io;
		dht_type_t _type;

		/* Methods */
		int16_t convert(uint8_t msb, uint8_t lsb);
		bool read(int16_t& humid, int16_t& temperature);
	};
}
