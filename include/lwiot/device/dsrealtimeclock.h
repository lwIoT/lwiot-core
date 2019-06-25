/*
 * DS3232 / DS3231 RTC by Maxim driver definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/realtimeclock.h>
#include <lwiot/io/i2cbus.h>

namespace lwiot
{
	class DsRealTimeClock : public RealTimeClock {
	public:
		enum AlarmType {
			ALARM1_EVERY_SECOND = 0x0F,
			ALARM1_MATCH_SECONDS = 0x0E,
			ALARM1_MATCH_MINUTES = 0x0C,
			ALARM1_MATCH_HOURS = 0x08,
			ALARM1_MATCH_DATE = 0x00,
			ALARM1_MATCH_DAY = 0x10,
			ALARM2_EVERY_MINUTE = 0x8E,
			ALARM2_MATCH_MINUTES = 0x8C,
			ALARM2_MATCH_HOURS = 0x88,
			ALARM2_MATCH_DATE = 0x80,
			ALARM2_MATCH_DAY = 0x90,
		};

		enum Alarm {
			ALARM_ONE = 1,
			ALARM_TWO = 2
		};

		explicit DsRealTimeClock(const I2CBus& bus);
		explicit DsRealTimeClock(const DateTime& now, const I2CBus& bus);

		DateTime now() override;
		void set(const DateTime &dt) override;

		void setAlarm(AlarmType type,  const DateTime& dt);
		bool alarm(Alarm id);

	private:
		I2CBus _bus;
		static constexpr int ReadLength = 7;
		static constexpr int8_t SlaveAddress = 0x68;

		/* Methods */
		uint8_t encode(uint8_t byte);
		uint8_t decode(uint8_t byte);
		void mktime(struct tm& tm, const I2CMessage& msg, bool century) const;

		uint8_t read(uint8_t addr);
		void write(uint8_t addr, uint8_t data);
	};
}

#ifndef _BV
#define _BV(bit) (1U << (bit))
#endif
