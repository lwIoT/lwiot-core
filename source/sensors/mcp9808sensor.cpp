/*
 * Driver for the MCP9808 temperature sensor.
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

#include <lwiot/io/i2cmessage.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/kernel/lock.h>

#include <lwiot/device/mcp9808sensor.h>

#define MCP9808_REG_CONFIG 0x01

#define MCP9808_REG_CONFIG_SHUTDOWN 0x0100
#define MCP9808_REG_CONFIG_CRITLOCKED 0x0080
#define MCP9808_REG_CONFIG_WINLOCKED 0x0040
#define MCP9808_REG_CONFIG_INTCLR 0x0020
#define MCP9808_REG_CONFIG_ALERTSTAT 0x0010
#define MCP9808_REG_CONFIG_ALERTCTRL 0x0008
#define MCP9808_REG_CONFIG_ALERTSEL 0x0004
#define MCP9808_REG_CONFIG_ALERTPOL 0x0002
#define MCP9808_REG_CONFIG_ALERTMODE 0x0001

#define MCP9808_REG_UPPER_TEMP 0x02
#define MCP9808_REG_LOWER_TEMP 0x03
#define MCP9808_REG_CRIT_TEMP 0x04
#define MCP9808_REG_AMBIENT_TEMP 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_DEVICE_ID 0x07
#define MCP9808_REG_RESOLUTION 0x08

namespace lwiot
{
	MCP9808Sensor::MCP9808Sensor() : _lock(false)
	{
	}

	bool MCP9808Sensor::begin(lwiot::I2CBus &bus, uint8_t addr)
	{
		UniqueLock<Lock> lock(this->_lock);

		this->_bus = bus;
		this->_addr = addr;

		auto manuf = this->read16(MCP9808_REG_MANUF_ID);
		auto devid = this->read16(MCP9808_REG_DEVICE_ID);

		if(manuf != 0x54)
			return false;

		if(devid != 0x400)
			return false;

		this->write16(MCP9808_REG_CONFIG, 0);
		return true;
	}

	void MCP9808Sensor::shutdown()
	{
		uint16_t conf;
		uint16_t conf_reg;

		conf_reg = this->read16(MCP9808_REG_CONFIG);
		conf = conf_reg | MCP9808_REG_CONFIG_SHUTDOWN;
		this->write16(MCP9808_REG_CONFIG, conf);
	}

	void MCP9808Sensor::wake()
	{
		uint16_t conf;
		uint16_t conf_reg;

		conf_reg = this->read16(MCP9808_REG_CONFIG);
		conf = conf_reg & ~MCP9808_REG_CONFIG_SHUTDOWN;
		this->write16(MCP9808_REG_CONFIG, conf);
	}

	void MCP9808Sensor::setResolution(int resolution)
	{
		uint8_t reso;

		reso = resolution & 0x3;
		this->write8(MCP9808_REG_RESOLUTION, reso);
	}

	float MCP9808Sensor::read()
	{
		float temp = NAN;
		uint16_t raw;

		raw = this->read16(MCP9808_REG_AMBIENT_TEMP);

		if(raw != 0xFFFF) {
			temp = raw & 0xFFF;
			temp /= 16.0f;

			if(raw & 0x1000)
				temp -= 256;
		}

		return temp;
	}

	uint8_t MCP9808Sensor::read8(uint8_t reg)
	{
		I2CMessage rx(1), tx(1);
		stl::Vector<I2CMessage> msgs;

		tx.setAddress(this->_addr, false, false);
		tx.write(reg);
		tx.setRepeatedStart(true);

		rx.setAddress(this->_addr, false, true);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus->transfer(msgs)) {
			print_dbg("Unable to read from MCP9808!\n");
			return false;
		}

		rx = stl::move(msgs.back());
		return rx[0];
	}

	void MCP9808Sensor::write8(uint8_t reg, uint8_t value)
	{
		I2CMessage tx(2);

		tx.setAddress(this->_addr, false, false);
		tx.write(reg);
		tx.write(value);
		tx.setRepeatedStart(false);

		if(!this->_bus->transfer(tx)) {
			print_dbg("Unable to write to MCP9808!\n");
		}
	}

	void MCP9808Sensor::write16(uint8_t reg, uint16_t value)
	{
		I2CMessage tx(3);

		tx.setAddress(this->_addr, false, false);
		tx.write(reg);
		tx.write(value);
		tx.setRepeatedStart(false);

		if(!this->_bus->transfer(tx)) {
			print_dbg("Unable to write to MCP9808!\n");
		}
	}

	uint16_t MCP9808Sensor::read16(uint8_t reg)
	{
		I2CMessage rx(2), tx(1);
		stl::Vector<I2CMessage> msgs;
		uint16_t value;

		tx.setAddress(this->_addr, false, false);
		tx.write(reg);
		tx.setRepeatedStart(true);

		rx.setAddress(this->_addr, false, true);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus->transfer(msgs)) {
			print_dbg("Unable to read from MCP9808!\n");
			return false;
		}

		rx = stl::move(msgs.back());
		value = rx[0];
		value <<= 8;
		value |= rx[1];

		return value;
	}
}
