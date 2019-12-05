/*
 * CCS811 air quality sensor.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file ccs811sensor.h

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

	/**
	 * @brief CCS811 drive mode.
	 * @ingroup device
	 */
	typedef enum {
		CCS811_DRIVE_MODE_IDLE = 0x00, //!< Disable the sensor.
		CCS811_DRIVE_MODE_1SEC = 0x01, //!< Sample every second.
		CCS811_DRIVE_MODE_10SEC = 0x02, //!< Sample every 10 seconds.
		CCS811_DRIVE_MODE_60SEC = 0x03, //!< Sample every 60 seconds.
		CCS811_DRIVE_MODE_250MS = 0x04, //!< Sample every 250ms.
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

	/**
	 * @brief CCS811 driver.
	 * @ingroup device
	 */
	class Ccs811Sensor {
	public:
		/**
		 * @brief Construct a new CCS811 driver object.
		 * @param bus I2C bus.
		 */
		explicit Ccs811Sensor(I2CBus& bus);
		~Ccs811Sensor() = default;

		/**
		 * @brief Construct a new
		 * @return
		 */
		bool begin();

		/**
		 * @brief Set environment values.
		 * @param rh Relative humidity.
		 * @param temperature Temperature.
		 */
		void setEnvironimentalData(uint8_t rh, double temperature);

		/**
		 * @brief Set the drive mode.
		 * @param mode Drive mode to set.
		 */
		void setDriveMode(uint8_t mode);

		/**
		 * @brief Set the interrupt state.
		 * @param enabled Enable/disable flag.
		 */
		void setInterrupt(bool enabled);

		/**
		 * @brief Set the temperature offset.
		 * @param offset Offset to set.
		 */
		void setTemperatureOffset(float offset);

		/**
		 * @brief Set the threshold value.
		 * @param low Low end.
		 * @param medium Medium threshold.
		 * @param hysteresis Hysteresis value.
		 */
		void setThresholds(uint16_t low, uint16_t medium, uint8_t hysteresis = 50);

		/**
		 * @brief Get the latest TVOC reading.
		 * @return The latest TVOC readng.
		 */
		uint16_t tvoc() const;

		/**
		 * @brief Get the latest CO2 equivalent value.
		 * @return The latest samplevalue.
		 */
		uint16_t eco2() const;

		/**
		 * @brief Check if a new sample is available.
		 * @return Availability indicator.
		 */
		bool available();

		/**
		 * @brief Get the last error code.
		 * @return Error code value.
		 */
		uint8_t error();

		/**
		 * @brief Calculate the temperature value.
		 * @return The current temperature.
		 */
		double calculateTemperature();

		/**
		 * @brief Read a new sample.
		 * @return Success indicator.
		 */
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
