/*
 * DS3232 RTC by Maxim driver implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/realtimeclock.h>
#include <lwiot/util/datetime.h>
#include <lwiot/device/dsrealtimeclock.h>

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x01
#define RTC_HOURS 0x02
#define RTC_DAY 0x03
#define RTC_DATE 0x04
#define RTC_MONTH 0x05
#define RTC_YEAR 0x06
#define RTC_STATUS 0x0F

#define ALM1_SECONDS 0x07
#define ALM1_MINUTES 0x08
#define ALM1_HOURS 0x09
#define ALM1_DAYDATE 0x0A
#define ALM2_MINUTES 0x0B
#define ALM2_HOURS 0x0C
#define ALM2_DAYDATE 0x0D
#define RTC_CONTROL 0x0E
#define RTC_STATUS 0x0F
#define RTC_AGING 0x10
#define RTC_TEMP_MSB 0x11
#define RTC_TEMP_LSB 0x12
#define SRAM_START_ADDR 0x14
#define SRAM_SIZE 236


#define YEAR_OFFSET 2000

/* Alarm mask bits */
#define A1M1 7
#define A1M2 7
#define A1M3 7
#define A1M4 7
#define A2M2 7
#define A2M3 7
#define A2M4 7

/* Control register bits */
#define EOSC 7
#define BBSQW 6
#define CONV 5
#define RS2 4
#define RS1 3
#define INTCN 2
#define A2IE 1
#define A1IE 0

/* Status register bits */
#define OSF 7
#define BB32KHZ 6
#define CRATE1 5
#define CRATE0 4
#define EN32KHZ 3
#define BSY 2
#define A2F 1
#define A1F 0

#define DS1307_CH 7                // for DS1307 compatibility, Clock Halt bit in Seconds register
#define HR1224 6                   // Hours register 12 or 24 hour mode (24 hour mode==0)
#define CENTURY 7                  // Century bit in Month register
#define DYDT 6                     // Day/Date flag bit in alarm Day/Date registers

namespace lwiot
{
	DsRealTimeClock::DsRealTimeClock(const lwiot::I2CBus &bus) : DsRealTimeClock(DateTime(), bus)
	{
	}

	DsRealTimeClock::DsRealTimeClock(const lwiot::DateTime &now, const lwiot::I2CBus &bus) : _bus(bus)
	{
	}

	void DsRealTimeClock::mktime(struct tm &tm, const lwiot::I2CMessage &msg, bool century) const
	{
		unsigned add_century = 0;

		tm.tm_sec = msg[0] & ~_BV(DS1307_CH);
		tm.tm_min = msg[1];
		tm.tm_hour = msg[2] & ~_BV(HR1224);
		tm.tm_mday = msg[4];
		tm.tm_mon = msg[5] - 1;

		if(century)
			add_century = 100;

		tm.tm_year = msg[6] + YEAR_OFFSET - 1900; /* tm.tm_year is counted since 1900 */
		tm.tm_year += add_century;
	}

	DateTime DsRealTimeClock::now()
	{
		I2CMessage tx(1), rx(ReadLength);
		stl::Vector<I2CMessage> msgs;
		struct tm tm{};
		time_t stamp;

		tx.setAddress(SlaveAddress, false);
		tx.markAsReadOperation(false);
		tx.setRepeatedStart(true);
		tx.write(RTC_SECONDS);

		rx.setAddress(DsRealTimeClock::SlaveAddress, false);
		rx.markAsReadOperation(true);
		rx.setRepeatedStart(false);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus.transfer(msgs)) {
			return DateTime();
		}

		rx = stl::move(msgs[1]);

		auto century = rx[5] & _BV(CENTURY);
		rx[5] &= ~_BV(CENTURY);

		for(uint8_t &byte : rx) {
			byte = this->decode(byte);
		}

		this->mktime(tm, rx, century != 0);
		stamp = ::mktime(&tm);
		return DateTime(stamp);
	}

	void DsRealTimeClock::set(const DateTime &dt)
	{
		I2CMessage tx(8), status(1), rx(1);
		uint8_t stopbit;

		tx.setAddress(SlaveAddress, false);
		tx.markAsReadOperation(false);
		tx.setRepeatedStart(false);

		tx.write(RTC_SECONDS);
		tx.write(this->encode(dt.second()));
		tx.write(this->encode(dt.minute()));
		tx.write(this->encode(dt.hour()));
		tx.write(this->encode(dt.dayOfWeek() + 1));
		tx.write(this->encode(dt.day()));
		tx.write(this->encode(dt.month() + 1));

		auto yr = dt.year();
		yr -= YEAR_OFFSET;
		tx.write(this->encode(yr));

		if(!this->_bus.transfer(tx)) {
			print_dbg("Unable to set RTC!\n");
			return;
		}

		stopbit = this->read(RTC_STATUS);
		stopbit &= ~_BV(OSF);
		this->write(RTC_STATUS, stopbit);
	}

	void DsRealTimeClock::write(uint8_t addr, uint8_t data)
	{
		I2CMessage tx(2);

		tx.setAddress(SlaveAddress, false);
		tx.markAsReadOperation(false);
		tx.setRepeatedStart(false);
		tx.write(addr);
		tx.write(data);

		if(!this->_bus.transfer(tx)) {
			print_dbg("Unable to write to RTC!");
		}
	}

	uint8_t DsRealTimeClock::read(uint8_t addr)
	{
		I2CMessage tx(1), rx(1);
		stl::Vector<I2CMessage> msgs;

		tx.setAddress(SlaveAddress, false);
		tx.markAsReadOperation(false);
		tx.setRepeatedStart(true);
		tx.write(addr);

		rx.setAddress(SlaveAddress, false);
		rx.markAsReadOperation(true);
		rx.setRepeatedStart(false);

		msgs.pushback(stl::move(tx));
		msgs.pushback(stl::move(rx));

		if(!this->_bus.transfer(msgs)) {
			print_dbg("Unable to read RTC!");
			return 0;
		}

		rx = stl::move(msgs.back());

		return rx[0];
	}

	void DsRealTimeClock::setAlarm(lwiot::DsRealTimeClock::AlarmType id, int seconds, int minutes, int hours, int day)
	{
		uint8_t addr;

		seconds = this->encode(seconds);
		minutes = this->encode(minutes);
		hours = this->encode(hours);

		if(id & 0x01)
			seconds |= _BV(A1M1);

		if(id & 0x02)
			minutes |= _BV(A1M2);

		if(id & 0x04)
			hours |= _BV(A1M3);

		if(id & 0x10) {
			day = this->encode(day);
			day |= _BV(DYDT);
		}

		if(id & 0x08) {
			day = this->encode(day);
			day |= _BV(A1M4);
		}

		if((id & 0x80) == 0) {
			addr = ALM1_SECONDS;
			this->write(addr, seconds);

			addr += 1;
		} else {
			addr = ALM2_MINUTES;
		}

		this->write(addr++, minutes);
		this->write(addr++, hours);
		this->write(addr, day);
	}

	void DsRealTimeClock::enableAlarmInterrupt(lwiot::DsRealTimeClock::Alarm id, bool enabled)
	{
		uint8_t control;
		uint8_t mask;

		control = this->read(RTC_CONTROL);
		mask = _BV(A1IE) << (id - 1);

		if(enabled)
			control |= mask;
		else
			control &= ~mask;

		this->write(RTC_CONTROL, control);
	}

	void DsRealTimeClock::setAlarm(lwiot::DsRealTimeClock::AlarmType type, const lwiot::DateTime &dt)
	{
		auto seconds = dt.second();
		auto minutes = dt.minute();
		auto hours = dt.hour();
		uint8_t day;

		if(type == ALARM1_MATCH_DATE || type == ALARM2_MATCH_DATE)
			day = dt.day();
		else
			day = dt.dayOfWeek() + 1;

		this->setAlarm(type, seconds, minutes, hours, day);
	}

	bool DsRealTimeClock::alarm(lwiot::DsRealTimeClock::Alarm id)
	{
		uint8_t num = id - 1;
		uint8_t status, mask;
		bool rv = false;

		status = this->read(RTC_STATUS);
		mask = _BV(A1F) << num;

		print_dbg("Status: 0x%X\n", status);

		if(status & mask) {
			status &= ~mask;
			this->write(RTC_STATUS, status);
			rv = true;
		}

		return rv;
	}

	uint8_t __never_inline DsRealTimeClock::encode(uint8_t byte)
	{
		return byte + 6 * (byte / 10);
	}

	uint8_t __never_inline DsRealTimeClock::decode(uint8_t byte)
	{
		return byte - 6 * (byte >> 4);
	}
}
