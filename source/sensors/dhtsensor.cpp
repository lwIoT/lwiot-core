/*
 * DHT sensor implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/dhtbus.h>
#include <lwiot/dhtsensor.h>

#ifndef BIT
#define BIT(x) (1UL << x)
#endif

namespace lwiot
{
	DhtSensor::DhtSensor(const GpioPin& pin, dht_type_t type) : _io(pin), _type(type)
	{
	}

	const dht_type_t& DhtSensor::type() const
	{
		return this->_type;
	}

	int16_t DhtSensor::convert(uint8_t msb, uint8_t lsb)
	{
		int16_t data;

		if(this->type() == dht_type_t::DHT22) {
			data = msb & 0x7F;
			data <<= 8U;
			data |= lsb;

			if(msb & BIT(7))
				data = 0-data; /* Faster than `* -1' */
		} else {
			data = msb * 10;
		}

		return data;
	}

	bool DhtSensor::read(int16_t& humidity, int16_t& temperature)
	{
		Vector<bool> bits(DhtBus::Bits);
		uint8_t data[DhtBus::Bits / BITS_PER_BYTE];
		int idx;

		memset((void*)data, 0, sizeof(data));

		if(!this->_io.read(bits))
			return false;

		idx = 0;
		for(auto value : bits) {
			data[idx / BITS_PER_BYTE] <<= 1U;
			data[idx / BITS_PER_BYTE] |= static_cast<uint8_t>(value);
			idx++;
		}

		if(data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
			print_dbg("DHT22 checksum failed! [%u : %u : %u : %u : %u]\n",
				data[0], data[1], data[2], data[3], data[4]);
		}

		humidity = this->convert(data[0], data[1]);
		temperature = this->convert(data[2], data[3]);

		return true;
	}

	bool DhtSensor::read(float& humidity, float& temperature)
	{
		int16_t h, t;

		if(!this->read(h, t))
			return false;

		humidity = static_cast<float>(h) / 10;
		temperature = static_cast<float>(t) / 10;
		return true;
	}
}
