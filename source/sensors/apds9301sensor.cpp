/*
 * TSL2561 light sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/log.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/i2cbus.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/device/apds9301sensor.h>

#define APDS_DEBUG 1
#ifdef APDS_DEBUG
#define apds_dbg(...) print_dbg(__VA_ARGS__)
#else
#define apds_dbg(s, ...)
#endif

#define CONTROL_REG        0x80
#define TIMING_REG         0x81
#define THRESHLOWLOW_REG   0x82
#define THRESHLOWHI_REG    0x83
#define THRESHHILOW_REG    0x84
#define THRESHHIHI_REG     0x85
#define INTERRUPT_REG      0x86
#define ID_REG             0x8A
#define DATA0LOW_REG       0x8C
#define DATA0HI_REG        0x8D
#define DATA1LOW_REG       0x8E
#define DATA1HI_REG 0x8F

#define CMD_REG 0x20

namespace lwiot
{
	Apds9301Sensor::Apds9301Sensor(I2CBus &bus, uint8_t addr) : _bus(bus), _addr(addr)
	{
	}

	bool Apds9301Sensor::begin()
	{
		auto a = this->on();
		auto b = this->setGain(false);
		auto c = this->setIntegrationTime(Apds9301Sensor::INT_TIME_101_MS);

		return a && b && c;
	}

	bool Apds9301Sensor::getData(uint16_t &x, uint16_t &y)
	{
		x = y = 0U;

		auto x_result = this->read(DATA0LOW_REG, x);
		auto y_result = this->read(DATA1LOW_REG, y);
		return x_result && y_result;
	}

	bool Apds9301Sensor::on()
	{
		return this->write(CONTROL_REG, (uint8_t) 0x3);
	}

	bool Apds9301Sensor::off()
	{
		return this->write(CONTROL_REG, (uint8_t) 0x0);
	}

	bool Apds9301Sensor::setGain(bool level)
	{
		uint8_t reg;

		if(!this->read(TIMING_REG, reg))
			return false;

		if(level)
			reg |= 0x10;
		else
			reg &= 0x10;

		return this->write(TIMING_REG, reg);
	}

	bool Apds9301Sensor::setIntegrationTime(lwiot::Apds9301Sensor::integration_time_t time)
	{
		uint8_t reg;

		if(!this->read(TIMING_REG, reg))
			return false;

		reg &= 3;
		switch(time) {
		case INT_TIME_402_MS:
			reg |= 2;
			break;

		case INT_TIME_101_MS:
			reg |= 1;
			break;

		default:
		case INT_TIME_13_7_MS:
			break;
		}

		return this->write(TIMING_REG, reg);
	}

	bool Apds9301Sensor::getLux(double &lux)
	{
		uint16_t x, y;
		bool gain;
		double ratio, ch0, ch1;

		if(!this->getData(x, y)) {
			apds_dbg("Unable to get data..\n");
			return false;
		}

		ch0 = x;
		ch1 = y;
		ratio = ch1 / ch0;

		switch(this->getIntegrationTime()) {
		case INT_TIME_13_7_MS:
			ch0 *= 1 / 0.034;
			ch1 *= 1 / 0.034;
			break;
		case INT_TIME_101_MS:
			ch0 *= 1 / 0.252;
			ch1 *= 1 / 0.252;
			break;

		case INT_TIME_402_MS:
			ch0 *= 1;
			ch1 *= 1;
			break;
		}

		gain = false;
		if(!this->getGain(gain)) {
			apds_dbg("Unable to get gain..\n");
			return false;
		}

		if(!gain) {
			ch0 *= 16;
			ch1 *= 16;
		}

		if(ratio <= 0.5) {
			lux = (0.0304 * ch0) - ((0.062 * ch0) * (pow((ch1 / ch0), 1.4)));
		} else if(ratio <= 0.61) {
			lux = (0.0224 * ch0) - (0.031 * ch1);
		} else if(ratio <= 0.8) {
			lux = (0.0128 * ch0) - (0.0153 * ch1);
		} else if(ratio <= 1.3) {
			lux = (0.00146 * ch0) - (0.00112 * ch1);
		}

		return true;
	}

	bool Apds9301Sensor::getGain(bool& gain)
	{
		uint8_t value;

		if(!this->read(TIMING_REG, value))
			return false;

		value &= 0x10;
		gain = value != 0;
		return true;
	}

	Apds9301Sensor::integration_time_t Apds9301Sensor::getIntegrationTime()
	{
		uint8_t reg;

		this->read(TIMING_REG, reg);
		reg &= 3;

		switch(reg) {
		case 0:
			return INT_TIME_13_7_MS;
		case 1:
			return INT_TIME_101_MS;
		default:
			return INT_TIME_402_MS;
		}
	}

	uint8_t Apds9301Sensor::id()
	{
		uint8_t _id;

		_id = 0;
		this->read(ID_REG, _id);

		return _id;
	}

	bool Apds9301Sensor::write(uint8_t addr, uint8_t byte)
	{
		uint8_t data[] = {
				addr, byte
		};

		return this->_bus.send(this->_addr, data, sizeof data);
	}

	bool Apds9301Sensor::write(uint8_t addr, uint16_t word)
	{
		uint8_t address = CMD_REG | addr;
		uint8_t data[] = {
				address, (uint8_t) (word & 0xFF), (uint8_t) ((word >> BITS_PER_BYTE) & 0xFF)
		};

		return this->_bus.send(this->_addr, data, sizeof data);
	}

	bool Apds9301Sensor::read(uint8_t addr, uint16_t &word)
	{
		uint16_t data = 0;
		lwiot::I2CMessage wr(1), rd(2);
		lwiot::stl::Vector<lwiot::I2CMessage *> msgs;

		wr.write(CMD_REG | addr);
		wr.setAddress(this->_addr, false, false);
		wr.setRepeatedStart(true);
		rd.setRepeatedStart(false);
		rd.setAddress(this->_addr, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);
		if(!this->_bus.transfer(msgs))
			return false;

		data = rd[0] | (rd[1] << BITS_PER_BYTE);
		print_dbg("Data: %u\n", data);

		word = data;
		return true;
	}

	bool Apds9301Sensor::read(uint8_t addr, uint8_t &byte)
	{
		lwiot::I2CMessage wr(1), rd(1);
		lwiot::stl::Vector<lwiot::I2CMessage *> msgs;

		wr.write(addr);
		wr.setAddress(this->_addr, false, false);
		wr.setRepeatedStart(true);
		rd.setRepeatedStart(false);
		rd.setAddress(this->_addr, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);
		if(!this->_bus.transfer(msgs))
			return false;

		byte = rd[0];
		return true;
	}
}
