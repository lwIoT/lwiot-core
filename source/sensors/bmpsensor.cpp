/*
 * BMP base class implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/i2cbus.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/device/bmpsensor.h>

namespace lwiot
{
	BmpSensor::BmpSensor(uint8_t addr, lwiot::I2CBus &bus) : _addr(addr), _bus(bus)
	{
	}

	void BmpSensor::write(uint8_t reg, uint8_t value)
	{
		const uint8_t data[] = {reg, value};
		this->_bus.send(this->_addr, data, sizeof(data));
	}

	uint8_t BmpSensor::read8(uint8_t reg)
	{
		uint8_t data;
		return this->read(reg, &data, 1) ? data : 0;
	}

	uint16_t BmpSensor::read16(uint8_t reg)
	{
		uint8_t raw[2] = {0,0};
		uint16_t rv;

		if(!this->read(reg, raw, 2))
			return 0;

		rv = raw[0];
		rv <<= BITS_PER_BYTE;
		rv |= raw[1];
		return rv;
	}

	uint32_t BmpSensor::read24(uint8_t reg)
	{
		uint8_t raw[3] = {0,0,0};
		uint32_t rv;

		if(!this->read(reg, raw, 3))
			return 0;

		rv = raw[0];
		rv <<= BITS_PER_BYTE;
		rv |= raw[1];
		rv <<= BITS_PER_BYTE;
		rv |= raw[2];

		return rv;
	}

	int16_t BmpSensor::readS16(uint8_t reg)
	{
		return (int16_t)this->read16(reg);
	}

	uint16_t BmpSensor::read16_LE(uint8_t reg)
	{
		auto value = this->read16(reg);
		return (value >> BITS_PER_BYTE) | (value << BITS_PER_BYTE);
	}

	int16_t BmpSensor::readS16_LE(uint8_t reg)
	{
		return static_cast<int16_t>(this->read16_LE(reg));
	}

	bool BmpSensor::read(uint8_t reg, uint8_t *rv, size_t num)
	{
		I2CMessage wr(1), rd(num);
		stl::Vector<I2CMessage*> msgs;

		wr.write(reg);
		wr.setAddress(this->_addr, false, false);
		wr.setRepeatedStart(true);
		rd.setAddress(this->_addr, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(!this->_bus.transfer(msgs)) {
			print_dbg("Unable to write %u bytes to BMP sensor\n", num);
			return false;
		}

		size_t idx = 0;
		for(auto byte : rd) {
			rv[idx++] = byte;
		}

		return true;
	}
}
