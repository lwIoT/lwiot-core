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

		explicit HardwareI2CAlgorithm(HardwareI2CAlgorithm&& other) noexcept;
		explicit HardwareI2CAlgorithm(const HardwareI2CAlgorithm&) = delete;

		virtual ~HardwareI2CAlgorithm();

		HardwareI2CAlgorithm& operator=(HardwareI2CAlgorithm&& rhs) noexcept;
		HardwareI2CAlgorithm& operator=(const HardwareI2CAlgorithm&) = delete;

		ssize_t transfer(I2CMessage &msg) override;
		ssize_t transfer(stl::Vector<I2CMessage *> &msgs) override;
		void setFrequency(const uint32_t& freq) override;

		bool test();
		bool busy();

	protected:
		static constexpr uint32_t DefaultSclFrequency = 100000;
		static constexpr int Timeout = 10;

		bool transfer() const;
		bool isBusy() const;
		void copy(I2CAlgorithm& other) override;

	private:
		static constexpr int MaxRiseTime = 20;

		mutable GpioPin _scl;
		mutable GpioPin _sda;
		mutable Logger log;
		Lock _lock;

		void sdahi() const;
		void sdalow() const;

		void scllow() const;
		int sclhi() const;
	};
}
