/*
 * BMP280 barometric pressure sensor.
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
#include <lwiot/bmpsensor.h>
#include <lwiot/bmp280sensor.h>

namespace lwiot
{
	Bmp280Sensor::Bmp280Sensor(lwiot::I2CBus &bus) : BmpSensor(Bmp280Sensor::SlaveAddress, bus),
		 _pressure(0), _temperature(0.0), _calibrate(true)
	{
	}

	void Bmp280Sensor::begin()
	{
		this->_data.dig_T1 = this->read16_LE(BMP280_REGISTER_DIG_T1);
		this->_data.dig_T2 = this->readS16_LE(BMP280_REGISTER_DIG_T2);
		this->_data.dig_T3 = this->readS16_LE(BMP280_REGISTER_DIG_T3);


		this->_data.dig_P1 = this->read16_LE(BMP280_REGISTER_DIG_P1);
		this->_data.dig_P2 = this->readS16_LE(BMP280_REGISTER_DIG_P2);
		this->_data.dig_P3 = this->readS16_LE(BMP280_REGISTER_DIG_P3);
		this->_data.dig_P4 = this->readS16_LE(BMP280_REGISTER_DIG_P4);
		this->_data.dig_P5 = this->readS16_LE(BMP280_REGISTER_DIG_P5);
		this->_data.dig_P6 = this->readS16_LE(BMP280_REGISTER_DIG_P6);
		this->_data.dig_P7 = this->readS16_LE(BMP280_REGISTER_DIG_P7);
		this->_data.dig_P8 = this->readS16_LE(BMP280_REGISTER_DIG_P8);
		this->_data.dig_P9 = this->readS16_LE(BMP280_REGISTER_DIG_P9);

		this->write(BMP280_REGISTER_CONTROL, 0x3F);

		this->_calibrate = false;
	}

	int32_t Bmp280Sensor::readAdcPressure()
	{
		return this->read24(BMP280_REGISTER_PRESSUREDATA);
	}

	int32_t Bmp280Sensor::readAdcTemperature()
	{
		return this->read24(BMP280_REGISTER_TEMPDATA);
	}

	const int32_t &Bmp280Sensor::pressure() const
	{
		return this->_pressure;
	}

	const float &Bmp280Sensor::temperature() const
	{
		return this->_temperature;
	}

	void Bmp280Sensor::read()
	{
		if(this->_calibrate)
			return;

		auto adcT = this->readAdcTemperature();
		lwiot_sleep(20);

		this->_temperature = this->computeTemperature(adcT) / 100.0;
		auto adcP = 0;
		auto pvalue = this->computePressure(adcP);
		this->_pressure = pvalue;
	}

	void Bmp280Sensor::read(int32_t &pressure, float &temperature)
	{
		this->read();

		pressure = this->_pressure;
		temperature = this->_temperature;
	}

	float Bmp280Sensor::computePressure(int32_t input)
	{
		int64_t var1, var2, p;

		int32_t adc_P = this->read24(BMP280_REGISTER_PRESSUREDATA);
		adc_P >>= 4;

		var1 = ((int64_t)this->_tfine) - 128000;
		var2 = var1 * var1 * (int64_t)this->_data.dig_P6;
		var2 = var2 + ((var1*(int64_t)this->_data.dig_P5)<<17);
		var2 = var2 + (((int64_t)this->_data.dig_P4)<<35);
		var1 = ((var1 * var1 * (int64_t)this->_data.dig_P3)>>8) +
		       ((var1 * (int64_t)this->_data.dig_P2)<<12);
		var1 = (((((int64_t)1)<<47)+var1))*((int64_t)this->_data.dig_P1)>>33;

		if (var1 == 0) {
			return 0;  // avoid exception caused by division by zero
		}
		p = 1048576 - adc_P;
		p = (((p<<31) - var2)*3125) / var1;
		var1 = (((int64_t)this->_data.dig_P9) * (p>>13) * (p>>13)) >> 25;
		var2 = (((int64_t)this->_data.dig_P8) * p) >> 19;

		p = ((p + var1 + var2) >> 8) + (((int64_t)this->_data.dig_P7)<<4);
		return (float)p/256;
	}

	int32_t Bmp280Sensor::computeTemperature(int32_t adc_T) const
	{
		int32_t var1, var2;

		adc_T >>= 4;

		var1  = ((((adc_T>>3) - ((int32_t)this->_data.dig_T1 <<1))) *
		         ((int32_t)this->_data.dig_T2)) >> 11;

		var2  = (((((adc_T>>4) - ((int32_t)this->_data.dig_T1)) *
		           ((adc_T>>4) - ((int32_t)this->_data.dig_T1))) >> 12) *
		         ((int32_t)this->_data.dig_T3)) >> 14;

		this->_tfine = var1 + var2;
		return (this->_tfine * 5 + 128) >> 8;
	}
}
