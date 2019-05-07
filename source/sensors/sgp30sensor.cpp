/*
 * SGP30 sensor module.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>

#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>

#include <lwiot/device/sgp30sensor.h>

static uint8_t crc(uint8_t *data, uint8_t datalen)
{
	uint8_t crc = SGP30_CRC8_INIT;

	for (uint8_t i = 0; i < datalen; i++) {
		crc ^= data[i];

		for (uint8_t b = 0; b < 8; b++) {
			if (crc & 0x80)
				crc = (crc << 1) ^ SGP30_CRC8_POLYNOMIAL;
			else
				crc <<= 1;
		}
	}

	return crc;
}

namespace lwiot
{
	Sgp30Sensor::Sgp30Sensor(lwiot::I2CBus &bus) : _bus(bus), _tvoc(0), _co2(0)
	{
	}

	void Sgp30Sensor::begin()
	{
		uint8_t cmd[2];
		uint16_t features;

		cmd[0] = 0x20;
		cmd[1] = 0x2F;

		if(!this->read(cmd, sizeof(cmd), 10, &features, 1)) {
			print_dbg("Unable to get features!\n");
			return;
		}

		if(features != SGP30_FEATURESET) {
			print_dbg("Invalid SGP30 feature set!\n");
			return;
		}

		if(!this->init()) {
			print_dbg("SGP30 init failed!\n");
		}
	}

	bool Sgp30Sensor::setHumidity(uint32_t abs)
	{
		uint16_t ah_scaled = (uint16_t)(((uint64_t)abs * 256 * 16777) >> 24);
		uint8_t command[5];

		if(abs > 256000)
			return false;

		command[0] = 0x20;
		command[1] = 0x61;
		command[2] = ah_scaled >> 8;
		command[3] = ah_scaled & 0xFF;
		command[4] = crc(command + 2, 2);

		return this->read(command, sizeof(command), 10);
	}

	/* COPYRIGHT Adafruit industries */
	bool Sgp30Sensor::read(uint8_t *cmd, int len, uint16_t ms, uint16_t *result, int readlen)
	{
		I2CMessage tx(len);

		tx.setAddress(SGP30_I2CADDR_DEFAULT, false);
		tx.setRepeatedStart(false);
		tx.markAsReadOperation(false);

		for(auto idx = 0; idx < len; idx++)
			tx.write(cmd[idx]);

		if(!this->_bus->transfer(tx)) {
			print_dbg("Unable to write SGP30 command!\n");
			return false;
		}

		lwiot_sleep(ms);

		if(readlen == 0)
			return true;

		auto rxlen = readlen * (SGP30_WORD_LEN + 1);
		I2CMessage rx(rxlen);

		rx.setAddress(SGP30_I2CADDR_DEFAULT, false);
		rx.markAsReadOperation(true);
		rx.setRepeatedStart(false);

		if(!this->_bus->transfer(rx)) {
			print_dbg("Unable to read SGP30 command result!\n");
			return false;
		}

		for(uint8_t idx = 0; idx < readlen; idx++) {
			auto data = rx.data();
			uint8_t crcvalue = crc(data + idx * 3, 2);

			if(crcvalue != data[idx * 3 + 2])
				return false;

			result[idx] = rx[idx * 3];
			result[idx] <<= 8;
			result[idx] |= rx[idx * 3 + 1];
		}

		return true;
	}

	bool Sgp30Sensor::init()
	{
		uint8_t cmd[2];

		cmd[0] = 0x20;
		cmd[1] = 0x3;

		return this->read(cmd, sizeof(cmd), 10);
	}

	bool Sgp30Sensor::measure()
	{
		uint8_t cmd[2];
		uint16_t reply[2];

		cmd[0] = 0x20;
		cmd[1] = 0x08;

		if(!this->read(cmd, sizeof(cmd), 12, reply, 2))
			return false;

		this->_co2 = reply[0];
		this->_tvoc = reply[1];
		return true;
	}
}
