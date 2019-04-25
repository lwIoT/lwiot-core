/*
 * BMP base header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/io/i2cbus.h>

namespace lwiot
{
	class BmpSensor {
	public:
		explicit BmpSensor(uint8_t addr, I2CBus& bus);
		virtual ~BmpSensor() = default;

	protected:
		virtual uint32_t read24(uint8_t reg);
		virtual uint16_t read16(uint8_t reg);
		virtual uint8_t read8(uint8_t reg);

		virtual int16_t readS16(uint8_t reg);

		uint16_t read16_LE(uint8_t reg);
		int16_t readS16_LE(uint8_t reg);

		virtual void write(uint8_t reg, uint8_t value);

	private:
		uint8_t _addr;
		I2CBus _bus;

		bool read(uint8_t reg, uint8_t *rv, size_t num);
	};
}
