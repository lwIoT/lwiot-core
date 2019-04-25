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
		explicit DsRealTimeClock(const I2CBus& bus);
		explicit DsRealTimeClock(const DateTime& now, const I2CBus& bus);

		DateTime now() override;
		void set(const DateTime &dt) override;

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
