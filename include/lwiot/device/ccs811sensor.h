/*
 * CCS811 air quality sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/io/i2cbus.h>

namespace lwiot
{
	typedef enum {
		CCS811_BOOTLOADER_APP_ERASE = 0xF1,
		CCS811_BOOTLOADER_APP_DATA = 0xF2,
		CCS811_BOOTLOADER_APP_VERIFY = 0xF3,
		CCS811_BOOTLOADER_APP_START = 0xF4
	} ccs811_bootloader_register_t;

	typedef enum {
		CCS811_STATUS = 0x00,
		CCS811_MEAS_MODE = 0x01,
		CCS811_ALG_RESULT_DATA = 0x02,
		CCS811_RAW_DATA = 0x03,
		CCS811_ENV_DATA = 0x05,
		CCS811_NTC = 0x06,
		CCS811_THRESHOLDS = 0x10,
		CCS811_BASELINE = 0x11,
		CCS811_HW_ID = 0x20,
		CCS811_HW_VERSION = 0x21,
		CCS811_FW_BOOT_VERSION = 0x23,
		CCS811_FW_APP_VERSION = 0x24,
		CCS811_ERROR_ID = 0xE0,
		CCS811_SW_RESET = 0xFF,
	} ccs811_register_t;

	typedef enum {
		CCS811_DRIVE_MODE_IDLE = 0x00,
		CCS811_DRIVE_MODE_1SEC = 0x01,
		CCS811_DRIVE_MODE_10SEC = 0x02,
		CCS811_DRIVE_MODE_60SEC = 0x03,
		CCS811_DRIVE_MODE_250MS = 0x04,
	} ccs811_drive_mode_t;

	namespace ccs811
	{
		struct Status {
		public:
			uint8_t error : 1;
			uint8_t data_ready : 1;
			uint8_t app_valid : 1;
			uint8_t fw_mode;

			void set(uint8_t data);
		};

		struct MeasurementMode {
		public:
			uint8_t interrupt_threshold : 1;
			uint8_t interrupt_datardy : 1;
			uint8_t drive_mode : 3;

			uint8_t get();
		};

		struct ErrorId {
			uint8_t write_reg_invalid : 1;
			uint8_t read_reg_invalid : 1;
			uint8_t measurement_mode_invalid : 1;
			uint8_t max_resistance : 1;
			uint8_t heater_fault : 1;
			uint8_t heater_supply : 1;

			void set(uint8_t data);
		};
	}

	class Ccs811Sensor {
	public:
		explicit Ccs811Sensor(I2CBus& bus);
		virtual ~Ccs811Sensor() = default;

		bool begin();

		void setEnvironimentalData(uint8_t rh, double temperature);
		void setDriveMode(uint8_t mode);
		void setInterrupt(bool enabled);
		void setTemperatureOffset(float offset);
		void setThresholds(uint16_t low, uint16_t medium, uint8_t hysteresis = 50);

		uint16_t tvoc() const;
		uint16_t eco2() const;
		bool available();
		uint8_t error();

		double calculateTemperature();
		bool read();


	private:
		I2CBus _bus;
		float _temp_offset;
		uint16_t _tvoc;
		uint16_t _eco2;

		ccs811::Status _status;
		ccs811::MeasurementMode _mode;
		ccs811::ErrorId _error;

		static constexpr int8_t SlaveAddress = 0x5B;

		/* Methods */
		bool write(uint8_t reg, uint8_t value);
		bool write(uint8_t reg, const void *data, size_t length);
		bool read(uint8_t reg, uint8_t *buf, size_t length);
		uint8_t read8(uint8_t reg);

		void reset();
	};
}
