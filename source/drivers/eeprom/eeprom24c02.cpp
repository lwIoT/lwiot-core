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
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/device/eeprom24c02.h>
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
		stl::Vector<I2CMessage> msgs;
		ScopedLock lock(this->_lock);

		tx.setRepeatedStart(true);
		tx.write(addr);
		tx.setAddress(Eeprom24C02::SlaveAddress, false, false);
		rx.setAddress(Eeprom24C02::SlaveAddress, false, true);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus.transfer(msgs) || rx.length() == 0UL) {
			print_dbg("Unable to read byte from 24C02! Msg size: %u\n", rx.available());
			return 0;
		}

		rx = stl::move(msgs.back());
		return rx.at(0);
	}

	ssize_t Eeprom24C02::write(uint8_t addr, const void *data, size_t length)
	{
		size_t idx;
		uint8_t  address, current;
		const uint8_t *ptr;

		current = length < Eeprom24C02::PageSize ? static_cast<uint8_t>(length) : Eeprom24C02::PageSize;
		length -= current;
		address = addr;
		ptr = static_cast<const uint8_t *>(data);
		idx = 0UL;

		do {
			I2CMessage msg(current + 1);

			msg.write(address);
			for(auto i = 0UL; i < current; i++) {
				msg.write(ptr[idx++]);
			}

			msg.setAddress(Eeprom24C02::SlaveAddress, false, false);
			if(!this->_bus.transfer(msg)) {
				print_dbg("Unable to write bytes to 24C02 chip!\n");
				return -EINVALID;
			}

			address += current;
			current = length < Eeprom24C02::PageSize ? static_cast<uint8_t>(length) : Eeprom24C02::PageSize;
			length -= current;
			lwiot_sleep(10);
		} while(current > 0);

		return length;
	}

	ssize_t Eeprom24C02::read(uint8_t addr, void *data, size_t length)
	{
		I2CMessage tx(1), rx(length);
		stl::Vector<I2CMessage> msgs;
		ScopedLock lock(this->_lock);

		tx.setRepeatedStart(true);
		tx.setAddress(Eeprom24C02::SlaveAddress, false, false);
		tx.write(addr);
		rx.setAddress(Eeprom24C02::SlaveAddress, false, true);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus.transfer(msgs) || rx.length() != length) {
			print_dbg("Failed to read from 24C02. Received length: %lu\n", rx.length());
			return -EINVALID;
		}

		rx = stl::move(msgs.back());
		memcpy(data, rx.data(), rx.length());
		return rx.length();
	}
}
