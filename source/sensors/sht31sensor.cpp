/*
 * SHT31 sensor driver implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/move.h>

#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/device/sht31sensor.h>

static uint8_t crc8(const uint8_t *data, int len)
{
	const uint8_t POLYNOMIAL(0x31);
	uint8_t crc(0xFF);

	for (int j = len; j; --j) {
		crc ^= *data++;

		for (int i = 8; i; --i) {
			crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
		}
	}

	return crc;
}

namespace lwiot
{
	Sht31Sensor::Sht31Sensor()
	{
	}

	Sht31Sensor::Sht31Sensor(lwiot::I2CBus &io) : _bus(io)
	{
	}

	void Sht31Sensor::writeCommaned(uint16_t cmd)
	{
		lwiot::I2CMessage tx(2);

		tx.setAddress(SHT31_DEFAULT_ADDR, false);
		tx.setRepeatedStart(false);
		tx.markAsReadOperation(false);

		tx.write(cmd >> 8);
		tx.write(static_cast<uint8_t>(cmd & 0xFF));

		if(!this->_bus->transfer(tx)) {
			print_dbg("Unable to write SHT31 command!\n");
		}
	}

	double Sht31Sensor::temperature()
	{
		return this->_result.temperature;
	}

	double Sht31Sensor::humidity()
	{
		return this->_result.humidity;
	}

	void Sht31Sensor::setHeaterStatus(bool enable)
	{
		if(enable)
			this->writeCommaned(SHT31_HEATEREN);
		else
			this->writeCommaned(SHT31_HEATERDIS);
	}

	void Sht31Sensor::reset()
	{
		this->writeCommaned(SHT31_SOFTRESET);
		lwiot_sleep(10);
	}

	void Sht31Sensor::measure()
	{
		I2CMessage rx(6);
		uint16_t st, srh;

		this->writeCommaned(SHT31_MEAS_HIGHREP);
		lwiot_sleep(20);

		rx.setAddress(SHT31_DEFAULT_ADDR, false);
		rx.markAsReadOperation(true);
		rx.setRepeatedStart(false);

		if(!this->_bus->transfer(rx)) {
			print_dbg("Unable to measure SHT31");
		}

		/* Algorithm taken from Sensiron datasheet */
		st = rx[0];
		st <<= 8;
		st |= rx[1];

		if(rx[2] != crc8(rx.data(), 2)) {
			print_dbg("SHT31 CRC8 1 failed!\n");
			return;
		}

		srh = rx[3];
		srh <<= 8;
		srh |= rx[4];

		if(rx[5] != crc8(rx.data() + 3, 2)) {
			print_dbg("SHT31 CRC8 2 failed!\n");
			return;
		}

		double stemp = st;
		double shum = srh;

		stemp *= 175;
		stemp /= 0xffff;
		stemp = -45 + stemp;
		this->_result.temperature = stemp;

		//  Serial.print("SRH = "); Serial.println(SRH);
		shum *= 100;
		shum /= 0xFFFF;

		this->_result.humidity = shum;
	}

	void Sht31Sensor::setBus(lwiot::I2CBus &io)
	{
		this->_bus = io;
	}
}
