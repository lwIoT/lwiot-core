/*
 * One wire protocol bus definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/error.h>
#include <lwiot/log.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/io/onewirebus.h>
#include <lwiot/scopedlock.h>

namespace lwiot
{
	OneWireBus::OneWireBus(const GpioPin& pin) : _pin(pin), _lock(false), _log("1-wire", stdout)
	{
		ScopedLock lock(this->_lock);

		this->_pin.mode(PinMode::OUTPUT_OPEN_DRAIN);
		this->_pin << true;
	}

	OneWireBus::~OneWireBus()
	{
		ScopedLock lock(this->_lock);
		this->_pin.mode(PinMode::INPUT);
	}

	const GpioPin& OneWireBus::pin() const
	{
		return this->_pin;
	}

	bool OneWireBus::reset()
	{
		ScopedLock lock(this->_lock);

		this->_pin.mode(PinMode::OUTPUT_OPEN_DRAIN);
		this->_pin.write(true);

		if(!this->wait(250))
			return false;

		this->_pin << false;
		lwiot_udelay(480);

		enter_critical();
		this->_pin << true;
		lwiot_udelay(70);
		bool retval = !this->_pin;
		exit_critical();

		if(this->wait(410))
			return false;

		return retval;
	}

	bool OneWireBus::wait(unsigned long us)
	{
		bool state;

		for(auto idx = 0UL; idx < ((us + 4) / 5); idx++) {
			if(this->_pin)
				break;

			lwiot_udelay(5);
		}

		this->_pin >> state;
		lwiot_udelay(1);

		return state;
	}

	bool OneWireBus::write(uint8_t value)
	{
		bool mask;

		for(int pos = 0; pos < OneWireBus::msb()+1; pos++) {
			mask = (value >> pos) & OneWireBus::lsbval();
			if(!this->write(mask))
				return false;
		}

		return true;
	}

	bool OneWireBus::write(bool value)
	{
		if(!this->wait(10))
			return false;

		if(value) {
			enter_critical();
			this->_pin << false;
			lwiot_udelay(10);
			this->_pin << true;
			exit_critical();
			lwiot_udelay(55);
		} else {
			enter_critical();
			this->_pin << false;
			lwiot_udelay(65);
			this->_pin << true;
			exit_critical();
		}

		lwiot_udelay(1);
		return true;
	}

	bool OneWireBus::read(uint8_t& output)
	{
		uint8_t byte(0);
		bool bit;

		for(int i = 0; i < OneWireBus::msb()+1; i++ ) {
			if(this->read(bit) != -EOK)
				return false;

			byte |= bit << i;
		}

		output = byte;
		return true;
	}

	int OneWireBus::read(bool& output)
	{
		bool value;

		if(!this->wait(10))
			return -EINVALID;

		enter_critical();
		this->_pin << false;
		lwiot_udelay(2);

		this->_pin << true;
		lwiot_udelay(11);

		value = this->_pin;
		exit_critical();
		lwiot_udelay(48);

		output = value;
		return -EOK;
	}

	bool OneWireBus::write(const uint8_t *data, size_t length)
	{
		ScopedLock lock(this->_lock);

		for(auto idx = 0U; idx < length; idx++) {
			if(!this->write(data[idx]))
				return false;
		}

		return true;
	}

	bool OneWireBus::write(BufferedStream& io)
	{
		ScopedLock lock(this->_lock);

		while(io.available() > 0UL) {
			auto data = io.read();
			this->write(data);
		}

		return true;
	}

	bool OneWireBus::read(uint8_t *data, size_t length)
	{
		ScopedLock lock(this->_lock);

		for(auto idx = 0U; idx < length; idx++) {
			if(!this->read(data[idx]))
				return false;
		}

		return true;
	}

	bool OneWireBus::read(BufferedStream& io, size_t length)
	{
		uint8_t data;
		ScopedLock lock(this->_lock);

		for(auto idx = 0U; idx < length; idx++) {
			if(!this->read(data))
				return false;

			io << data;
		}

		return true;
	}

#define SELECT_ROM 0x55U

	bool OneWireBus::select(const onewire_addr_t& addr)
	{
		onewire_addr_t address = addr;
		ScopedLock lock(this->_lock);

		if(!this->write((uint8_t)SELECT_ROM))
			return true;

		for(auto i = 0U; i < sizeof(addr); i++) {
			if(!this->write(static_cast<uint8_t>(address & 0xFF)))
				return false;

			address >>= 8U;	
		}
		return true;
	}
}
