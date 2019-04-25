/*
 * DHT sensor definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/dhtbus.h>

namespace lwiot
{
	typedef enum {
		DHT11,
		DHT22
	} dht_type_t;

	class DhtSensor {
	public:
		explicit DhtSensor(const GpioPin& pin, dht_type_t type = DHT22);
		virtual ~DhtSensor() = default;

		bool read(float& humidity, float& temperature);
		const dht_type_t& type() const;

	private:
		DhtBus _io;
		dht_type_t _type;

		/* Methods */
		int16_t convert(uint8_t msb, uint8_t lsb);
		bool read(int16_t& humid, int16_t& temperature);
	};
}
