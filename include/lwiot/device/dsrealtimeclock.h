/*
 * DS3232 / DS3231 RTC by Maxim driver definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file dsrealtimeclock.h RTC clock.

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/realtimeclock.h>
#include <lwiot/io/i2cbus.h>

namespace lwiot
{
	/**
	 * @brief DS real time clock driver.
	 * @ingroup device
	 */
	class DsRealTimeClock : public RealTimeClock {
	public:

		/**
		 * @brief Alarm type.
		 */
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

		/**
		 * @brief Alarm number.
		 */
		enum Alarm {
			ALARM_ONE = 1,
			ALARM_TWO = 2
		};

		/**
		 * @brief Construct a new real time clock.
		 * @param bus I2C bus.
		 */
		explicit DsRealTimeClock(const I2CBus& bus);

		/**
		 * @brief Construct a new real time clock.
		 * @param now Current time stamp.
		 * @param bus I2C bus.
		 */
		explicit DsRealTimeClock(const DateTime& now, const I2CBus& bus);

		/**
		 * @brief Current time value.
		 * @return The current time value.
		 */
		DateTime now() override;

		/**
		 * @brief Set the time value.
		 * @param dt Time value to set.
		 */
		void set(const DateTime &dt) override;

		/**
		 * @brief Set an alarm.
		 * @param type Alarm type to set.
		 * @param seconds Second value.
		 * @param minutes Minute value.
		 * @param hours Hour value.
		 * @param day Day value.
		 */
		void setAlarm(AlarmType type, int seconds = 0, int minutes = 0, int hours = 0, int day = 1);

		/**
		 * @brief Set an alarm.
		 * @param type Alarm type.
		 * @param dt Date/time to set the alarm for.
		 */
		void setAlarm(AlarmType type, const DateTime& dt);

		/**
		 * @brief Reset the alarm.
		 * @param id Alarm to reset.
		 * @return A success indicator.
		 */
		bool alarm(Alarm id);

		/**
		 * @brief Enable the alarm interrupt.
		 * @param id Alarm to enable the ID for.
		 * @param enabled Enable/disable flag.
		 */
		void enableAlarmInterrupt(Alarm id, bool enabled);

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
