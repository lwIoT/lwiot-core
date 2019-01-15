/*
 * BMP085 barometric pressure sensor driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/i2cbus.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/device/bmpsensor.h>
#include <lwiot/device/bmp085sensor.h>

#define BMP_CHECK_REG 0xD0
#define BMP_CHECK_VALUE 0x55

#define BMP_A1_REG 0xAA
#define BMP_A2_REG 0xAC
#define BMP_A3_REG 0xAE
#define BMP_A4_REG 0xB0
#define BMP_A5_REG 0xB2
#define BMP_A6_REG 0xB4
#define BMP_B1_REG 0xB6
#define BMP_B2_REG 0xB8
#define BMP_MB_REG 0xBA
#define BMP_MC_REG 0xBC
#define BMP_MD_REG 0xBE

#define BMP_LOWPOWER    0
#define BMP_STANDARD    1
#define BMP_HIRES       2
#define BMP_ULTRA_HIRES 3

#define BMP_CONTROL 0xF4
#define BMP_READ_TEMPERATURE_CMD 0x2E
#define BMP_READ_PRESSURE_CMD 0x34
#define BMP_READ_TEMPDATA 0xF6
#define BMP_READ_PRESDATA 0xF6

namespace lwiot
{
	static inline void bmp_delay(int ms)
	{
		lwiot_sleep(ms);
	}

	Bmp085Sensor::Bmp085Sensor(lwiot::I2CBus &bus) : BmpSensor(Bmp085Sensor::SlaveAddress, bus),
		_pressure(0.0f), _temperature(0.0f), _calibrate(true), _oversampling(BMP_ULTRA_HIRES)
	{
	}

	void Bmp085Sensor::begin()
	{
		uint8_t check;

		check = this->read8(BMP_CHECK_REG);
		if(check != BMP_CHECK_VALUE) {
			print_dbg("Invalid value on BMP085 check register\n");
			return;
		}

		this->ac1 = this->readS16(BMP_A1_REG);
		this->ac2 = this->readS16(BMP_A2_REG);
		this->ac3 = this->readS16(BMP_A3_REG);
		this->ac4 = this->read16(BMP_A4_REG);
		this->ac5 = this->read16(BMP_A5_REG);
		this->ac6 = this->read16(BMP_A6_REG);

		this->b1 = this->readS16(BMP_B1_REG);
		this->b2 = this->readS16(BMP_B2_REG);

		this->mb = this->readS16(BMP_MB_REG);
		this->mc = this->readS16(BMP_MC_REG);
		this->md = this->readS16(BMP_MD_REG);
		this->_calibrate = false;
	}

	float Bmp085Sensor::temperature() const
	{
		if(this->_calibrate)
			return 0.0f;

		return this->_temperature;
	}

	int32_t Bmp085Sensor::pressure() const
	{
		if(this->_calibrate)
			return 0;

		return this->_pressure;
	}

#define TEMPERATURE_DELAY 5
	uint16_t Bmp085Sensor::readTemperature()
	{
		this->write(BMP_CONTROL, BMP_READ_TEMPERATURE_CMD);
		bmp_delay(TEMPERATURE_DELAY);
		return this->read16(BMP_READ_TEMPDATA);
	}

	uint32_t Bmp085Sensor::readPressure()
	{
		int msdelay;
		uint32_t raw;
		uint16_t raw16;
		uint8_t raw8;

		this->write(BMP_CONTROL, BMP_READ_PRESSURE_CMD + (this->_oversampling << 6));

		switch(this->_oversampling) {
		case BMP_LOWPOWER:
			msdelay = 5;
			break;

		case BMP_STANDARD:
			msdelay = 8;
			break;

		case BMP_HIRES:
			msdelay = 14;
			break;

		case BMP_ULTRA_HIRES:
			msdelay = 26;
			break;
		default:
			return 0;
		}

		bmp_delay(msdelay);
		raw16 = this->read16(BMP_READ_PRESDATA);
		raw8 = this->read8(BMP_READ_PRESDATA + sizeof(raw16));

		raw = raw16;
		raw <<= BITS_PER_BYTE;
		raw |= raw8;
		raw >>= (8 - this->_oversampling);

		return raw;
	}

	int32_t Bmp085Sensor::computeB5(int32_t ut)
	{
		int32_t x1, x2;

		x1 = (ut - static_cast<int32_t>(this->ac6)) * static_cast<int32_t>(this->ac5) >> 15;
		x2 = (static_cast<int32_t>(this->mc) << 11) / (x1 + static_cast<int32_t>(this->md));
		return x1 + x2;
	}

	float Bmp085Sensor::calculateTemperature()
	{
		float temp;
		auto ut = this->readTemperature();
		auto b5 = this->computeB5(ut);

		temp = (b5 + 8) >> 4;
		return temp / 10.0f;
	}

	int32_t Bmp085Sensor::calculatePressure()
	{
		int32_t up, ut, b5, b6, x1, x2, x3, p, b3;
		uint32_t b4, b7;

		ut = this->readTemperature();
		up = this->readPressure();
		b5 = this->computeB5(ut);

		b6 = b5 - 4000;
		x1 = ((int32_t)this->b2 * ((b6 * b6) >> 12)) >> 11;
		x2 = ((int32_t)this->ac2 * b6) >> 11;
		x3 = x1 + x2;
		b3 = ((( (int32_t)this->ac1 * 4 + x3) << this->_oversampling) + 2) / 4;

		x1 = ((int32_t)this->ac3 * b6) >> 13;
		x2 = ((int32_t)this->b1 * ((b6 * b6) >> 12)) >> 16;
		x3 = ((x1 + x2) + 2) >> 2;
		b4 = ((uint32_t)this->ac4 * (uint32_t) (x3 + 32768)) >> 15;
		b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> this->_oversampling);
		p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

		x1 = (p >> 8) * (p >> 8);
		x1 = (x1 * 3038) >> 16;
		x2 = (-7357 * p) >> 16;
		p = p + ((x1 + x2 + 3791) >> 4);

		return p;
	}

	void Bmp085Sensor::read()
	{
		if(this->_calibrate)
			return;

		this->_temperature = this->calculateTemperature();
		this->_pressure = this->calculatePressure();
	}

	void Bmp085Sensor::read(int32_t &pressure, float &temperature)
	{
		if(this->_calibrate)
			return;

		this->read();
		pressure = this->_pressure;
		temperature = this->_temperature;
	}
}
