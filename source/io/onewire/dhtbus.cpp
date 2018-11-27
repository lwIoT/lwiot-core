/*
 * DHT humidity 1-wire protocol bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/gpiopin.h>
#include <lwiot/log.h>
#include <lwiot/vector.h>
#include <lwiot/lock.h>
#include <lwiot/dhtbus.h>
#include <lwiot/scopedlock.h>

namespace lwiot
{
	DhtBus::DhtBus(const GpioPin& pin) : _pin(pin), _lock(false)
	{
		ScopedLock lock(this->_lock);

		this->_pin.mode(PinMode::INPUT_PULLUP);
		this->_pin << false;
	}

	DhtBus::~DhtBus()
	{
		ScopedLock lock(this->_lock);
		this->_pin.mode(PinMode::INPUT_NOPULLUP);
	}

	const GpioPin& DhtBus::pin() const
	{
		return this->_pin;
	}

	bool DhtBus::await(uint32_t tmo, bool expected, uint32_t& duration)
	{
		bool value;

		for(auto idx = 0U; idx < tmo; idx += DhtBus::Interval) {
			lwiot_udelay(DhtBus::Interval);
			this->_pin >> value;

			if(value == expected) {
				duration = idx;
				return true;
			}
		}

		return false;
	}

#define INIT_DELAY 250000U
#define START_DELAY 50000U

	bool DhtBus::_read(Vector<bool>& bits)
	{
		uint32_t low, high;

		this->_pin.mode(PinMode::OUTPUT);
		this->_pin.write(true);

		lwiot_udelay(START_DELAY);
		lwiot_udelay(START_DELAY);
		lwiot_udelay(START_DELAY);
		lwiot_udelay(START_DELAY);
		lwiot_udelay(START_DELAY);

		this->_pin.write(false);
		lwiot_udelay(10 * 1000);

		this->_pin.write(true);
		lwiot_udelay(40);

		this->_pin.mode(PinMode::INPUT_PULLUP);

		if(!this->await(DhtBus::PhaseOne, false, low)) {
			print_dbg("[DHT]: P1 fail\n");
			return false;
		}

		if(!this->await(DhtBus::PhaseTwo, true, low)) {
			/* TODO: log output */
			print_dbg("[DHT]: P2 fail\n");
			return false;
		}

		if(!this->await(DhtBus::PhaseThree, false, low)) {
			/* TODO: log output */
			print_dbg("[DHT]: P3 fail\n");
			return false;
		}

		low = high = 0UL;

		for(auto idx = 0; idx < DhtBus::Bits; idx++) {
			if(!this->await(DhtBus::DelayOne, true, low)) {
				print_dbg("[DHT]: D1 fail\n");
				return false;
			}

			if(!this->await(DhtBus::DelayTwo, false, high)) {
				print_dbg("[DHT]: D2 fail\n");
				return false;
			}

			bits.pushback(high > low);
		}

		return true;
	}

	bool DhtBus::read(Vector<bool>& bits)
	{
		bool retval;
		ScopedLock lock(this->_lock);

		enter_critical();
		retval = this->_read(bits);
		exit_critical();

		return retval;
	}
}
