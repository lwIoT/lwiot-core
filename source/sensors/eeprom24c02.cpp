/*
 * 24C02 EEPROM header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/i2cbus.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/eeprom24c02.h>
#include <lwiot/scopedlock.h>

namespace lwiot
{
	Eeprom24C02::Eeprom24C02(lwiot::I2CBus &bus) : _bus(bus), _lock(false)
	{
	}

	void Eeprom24C02::write(uint8_t addr, uint8_t byte)
	{
		uint8_t data[] = {addr, byte};
		ScopedLock lock(this->_lock);

		this->_bus.send(Eeprom24C02::SlaveAddress, data, sizeof(data));
	}

	uint8_t Eeprom24C02::read(uint8_t addr)
	{
		I2CMessage rx(1);
		I2CMessage tx(1);
		Vector<I2CMessage *> msgs;
		ScopedLock lock(this->_lock);

		tx.setRepeatedStart(true);
		tx.write(addr);
		tx.setAddress(Eeprom24C02::SlaveAddress, false, false);
		rx.setAddress(Eeprom24C02::SlaveAddress, false, true);

		msgs.pushback(&tx);
		msgs.pushback(&rx);

		if(!this->_bus.transfer(msgs) || rx.length() == 0UL) {
			print_dbg("Unable to read byte from 24C02! Msg size: %u\n", rx.available());
			return 0;
		}

		return rx.at(0);
	}

	ssize_t Eeprom24C02::write(uint8_t addr, const void *data, size_t length)
	{
		return -1;
	}

	ssize_t Eeprom24C02::read(uint8_t addr, void *data, size_t length)
	{
		return -1;
	}
}
