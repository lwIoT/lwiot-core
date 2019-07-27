/*
 * Hardware I2C algorithm definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	class HardwareI2CAlgorithm : public I2CAlgorithm {
	public:
		explicit HardwareI2CAlgorithm(int scl, int sda, uint32_t frequency = DefaultSclFrequency);
		explicit HardwareI2CAlgorithm(const GpioPin& scl, const GpioPin& sda, uint32_t frequency);
		explicit HardwareI2CAlgorithm(const GpioPin& scl, const GpioPin& sda);

		HardwareI2CAlgorithm(HardwareI2CAlgorithm&& other) noexcept;
		HardwareI2CAlgorithm(const HardwareI2CAlgorithm& other) noexcept;

		~HardwareI2CAlgorithm() override;

		HardwareI2CAlgorithm& operator=(HardwareI2CAlgorithm&& rhs) noexcept;
		HardwareI2CAlgorithm& operator=(const HardwareI2CAlgorithm& other) noexcept;

		ssize_t transfer(I2CMessage &msg) override;
		ssize_t transfer(stl::Vector<I2CMessage> &msgs) override;
		void setFrequency(const uint32_t& freq) override;

		bool test();
		bool busy();

	protected:
		static constexpr uint32_t DefaultSclFrequency = 100000;
		static constexpr int Timeout = 10;

		bool transfer() const;
		bool isBusy() const;
		virtual void move(I2CAlgorithm& other);
		void copy(const I2CAlgorithm& other) override;

		virtual void start(uint16_t sla, bool repeated) = 0;
		virtual void stop() = 0;
		virtual void reset() = 0;

		virtual void write(const uint8_t *byte, bool ack) = 0;
		virtual void write(const uint8_t *bytes, size_t length, bool ack) = 0;
		virtual void read(uint8_t* byte, bool ack) = 0;
		virtual void read(uint8_t* bytes, size_t length, bool ack) = 0;
		virtual int flush() const = 0;

	private:
		static constexpr int MaxRiseTime = 20;

		mutable GpioPin _scl;
		mutable GpioPin _sda;
		mutable Logger log;
		SharedPointer<Lock> _lock;

		void sdahi() const;
		void sdalow() const;

		void scllow() const;
		int sclhi() const;
	};
}
