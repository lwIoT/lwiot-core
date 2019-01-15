/*
 * CCS811 air quality sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/i2cbus.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/device/ccs811sensor.h>

#define CCS_HW_ID_CODE 0x81
#define CCS811_REF_RESISTOR 100000

namespace lwiot
{
	Ccs811Sensor::Ccs811Sensor(lwiot::I2CBus &bus) : _bus(bus)
	{
	}

	bool Ccs811Sensor::begin()
	{
		this->reset();
		lwiot_sleep(100);

		if(this->read8(CCS811_HW_ID) != CCS_HW_ID_CODE) {
			print_dbg("Unable to initialise CCS811 chip\n");
			return false;
		}

		this->write(CCS811_BOOTLOADER_APP_START, nullptr, 0);
		lwiot_sleep(100);

		if(this->error())
			return false;

		if(!this->_status.fw_mode)
			return false;

		this->setInterrupt(false);
		this->setDriveMode(CCS811_DRIVE_MODE_1SEC);
		return true;
	}

	uint16_t Ccs811Sensor::tvoc() const
	{
		return this->_tvoc;
	}

	uint16_t Ccs811Sensor::eco2() const
	{
		return this->_eco2;
	}

	bool Ccs811Sensor::write(uint8_t reg, uint8_t value)
	{
		uint8_t data[] = {reg, value};
		return this->_bus.send(Ccs811Sensor::SlaveAddress, &data, sizeof data);
	}

	bool Ccs811Sensor::write(uint8_t reg, const void *data, size_t length)
	{
		I2CMessage msg(length + 1);

		msg.setRepeatedStart(false);
		msg.setAddress(Ccs811Sensor::SlaveAddress, false, false);
		msg.write(reg);

		if(length && data)
			msg.write(static_cast<const uint8_t *>(data), length);

		return this->_bus.transfer(msg);
	}

	bool Ccs811Sensor::read(uint8_t reg, uint8_t *buf, size_t length)
	{
		I2CMessage wr(1), rd(length);
		stl::Vector<I2CMessage*> msgs;

		wr.setAddress(SlaveAddress, false, false);
		wr.setRepeatedStart(true);
		wr.write(reg);
		rd.setAddress(SlaveAddress, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(!this->_bus.transfer(msgs)) {
			print_dbg("Unable to read %u bytes to CCS811 sensor\n", length);
			return false;
		}

		auto idx = 0UL;
		for(auto byte : rd) {
			buf[idx++] = byte;
		}

		return true;
	}

	void Ccs811Sensor::setInterrupt(bool enabled)
	{
		if(enabled)
			this->_mode.interrupt_datardy = 1;
		else
			this->_mode.interrupt_datardy = 0;

		this->write(CCS811_MEAS_MODE, this->_mode.get());
	}

	bool Ccs811Sensor::available()
	{
		auto status = this->read8(CCS811_STATUS);

		this->_status.set(status);
		return this->_status.data_ready != 0;
	}

	void Ccs811Sensor::setTemperatureOffset(float offset)
	{
		this->_temp_offset = offset;
	}

	void Ccs811Sensor::setDriveMode(uint8_t mode)
	{
		this->_mode.drive_mode = mode;
		this->write(CCS811_MEAS_MODE, this->_mode.get());
	}

	void Ccs811Sensor::setEnvironimentalData(uint8_t rh, double temperature)
	{
		uint8_t hum_perc = rh << 1;
		float fractional = modf(temperature, &temperature);
		uint16_t temp_high = (((uint16_t)temperature + 25) << 9);
		uint16_t temp_low = ((uint16_t)(fractional / 0.001953125) & 0x1FF);
		uint16_t temp_conv = (temp_high | temp_low);

		uint8_t buf[] = { hum_perc, 0x0, (uint8_t)((temp_conv >> 8) & 0xFF),
		                  (uint8_t)(temp_conv & 0xFF)};
		this->write(CCS811_ENV_DATA, buf, sizeof(buf));
	}

	double Ccs811Sensor::calculateTemperature()
	{
		uint8_t buf[4];
		uint32_t vref, vntc;
		uint32_t rntc;
		double ntc_temp;

		this->read(CCS811_NTC, buf, 4);
		vref = ((uint32_t)buf[0] << 8) | buf[1];
		vntc = ((uint32_t)buf[2] << 8) | buf[3];

		rntc = vntc * CCS811_REF_RESISTOR / vref;
		ntc_temp = log((double)rntc / CCS811_REF_RESISTOR);
		ntc_temp /= 3380;
		ntc_temp += 1.0 / (25 + 273.15);
		ntc_temp = 1.0 / ntc_temp;
		ntc_temp -= 273.15;

		return ntc_temp - this->_temp_offset;
	}

	bool Ccs811Sensor::read()
	{
		uint8_t buf[8];

		if(!available())
			return false;

		this->read(CCS811_ALG_RESULT_DATA, buf, sizeof(buf));
		this->_eco2 = (static_cast<uint16_t>(buf[0]) << 8) | (static_cast<uint16_t>(buf[1]));
		this->_tvoc = (static_cast<uint16_t>(buf[2]) << 8) | (static_cast<uint16_t>(buf[3]));

		if(this->_status.error)
			return false;

		return true;
	}

	uint8_t Ccs811Sensor::read8(uint8_t reg)
	{
		I2CMessage wr(1), rd(1);
		stl::Vector<I2CMessage*> msgs;

		wr.setAddress(SlaveAddress, false, false);
		wr.setRepeatedStart(true);
		wr.write(reg);
		rd.setAddress(SlaveAddress, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(!this->_bus.transfer(msgs)) {
			print_dbg("Unable to read byte from CCS811 sensor\n");
			return 0;
		}

		return rd.at(0);
	}

	uint8_t Ccs811Sensor::error()
	{
		auto rv = this->read8(CCS811_STATUS);

		this->_status.set(rv);
		return this->_status.error;
	}

	void Ccs811Sensor::setThresholds(uint16_t low, uint16_t medium, uint8_t hysteresis)
	{
		uint8_t data[] = {(uint8_t)((low >> 8) & 0xF), (uint8_t)(low & 0xF),
		                  (uint8_t)((medium >> 8) & 0xF), (uint8_t)(medium & 0xF), hysteresis};
		this->write(CCS811_THRESHOLDS, data, 5);
	}

	void Ccs811Sensor::reset()
	{
		uint8_t data[4] = {0x11, 0xE5, 0x72, 0x8A};
		this->write(CCS811_SW_RESET, data, sizeof(data));
	}

	namespace ccs811
	{
		uint8_t MeasurementMode::get()
		{
			return (this->interrupt_threshold << 2) | (this->interrupt_datardy << 3) | (this->drive_mode << 4);
		}

		void Status::set(uint8_t data)
		{
			this->error = data & 0x1;
			this->data_ready = (data >> 3) & 0x1;
			this->app_valid = (data >> 4) & 0x1;
			this->fw_mode= (data >> 7) & 0x1;
		}

		void ErrorId::set(uint8_t data)
		{
			this->write_reg_invalid = data & 0x1;
			this->read_reg_invalid = (data & 0x2) >> 1;
			this->measurement_mode_invalid = (data & 0x4) >> 2;
			this->max_resistance = (data & 0x8) >> 3;
			this->heater_fault = (data & 0x10) >> 4;
			this->heater_supply = (data & 0x20) >> 5;
		}
	}
}
