/*
 * Hardware I2C algorithm implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/scopedlock.h>
#include <lwiot/log.h>
#include <lwiot/function.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/io/hardwarei2calgorithm.h>
#include <lwiot/io/i2chal.h>

#include <lwiot/stl/move.h>

namespace lwiot
{
	HardwareI2CAlgorithm::HardwareI2CAlgorithm(const GpioPin &sclpin, const GpioPin &sdapin, uint32_t frequency) :
			I2CAlgorithm(I2CAlgorithm::DefaultRetryDelay, frequency),
			_scl(sclpin), _sda(sdapin), log("hw-i2c"), _lock(new Lock(false))
	{
		this->_scl.setOpenDrain();
		this->_sda.setOpenDrain();

		this->_scl << true;
		this->_sda << true;

		this->test();
		i2c_setup(sclpin, sdapin, frequency);
	}

	HardwareI2CAlgorithm::HardwareI2CAlgorithm(const GpioPin &scl, const GpioPin &sda) :
		HardwareI2CAlgorithm(scl, sda, DefaultSclFrequency)
	{
	}

	HardwareI2CAlgorithm::HardwareI2CAlgorithm(int scl, int sda, uint32_t frequency) :
		HardwareI2CAlgorithm(GpioPin(scl), GpioPin(sda), frequency)
	{
	}

	HardwareI2CAlgorithm::HardwareI2CAlgorithm(HardwareI2CAlgorithm &&other) noexcept :
		I2CAlgorithm(I2CAlgorithm::DefaultRetryDelay, other.frequency())
	{
		this->copy(other);
	}

	HardwareI2CAlgorithm::~HardwareI2CAlgorithm()
	{
		ScopedLock lock(this->_lock);

		i2c_disable();
		this->_scl.input();
		this->_sda.input();
	}

	HardwareI2CAlgorithm& HardwareI2CAlgorithm::operator=(HardwareI2CAlgorithm&& rhs) noexcept
	{
		this->move(rhs);
		return *this;
	}

	void HardwareI2CAlgorithm::move(lwiot::I2CAlgorithm &other)
	{
		HardwareI2CAlgorithm& tocopy = static_cast<HardwareI2CAlgorithm&>(other);

		tocopy._lock->lock();
		I2CAlgorithm::copy(other);

		this->_sda = stl::move(tocopy._sda);
		this->_scl = stl::move(tocopy._scl);
		this->_lock = stl::move(tocopy._lock);
		this->log = stl::move(tocopy.log);
		this->_lock->unlock();
	}

	void HardwareI2CAlgorithm::copy(lwiot::I2CAlgorithm &other)
	{
		HardwareI2CAlgorithm& tocopy = static_cast<HardwareI2CAlgorithm&>(other);
		ScopedLock lock(tocopy._lock);

		I2CAlgorithm::copy(other);
		this->_scl = tocopy._scl;
		this->_sda = tocopy._sda;
		this->_lock = tocopy._lock;
		this->log = tocopy.log;
	}

	void HardwareI2CAlgorithm::setFrequency(const uint32_t &freq)
	{
		ScopedLock(this->_lock);

		I2CAlgorithm::setFrequency(freq);
		i2c_set_frequency(freq);
	}

	ssize_t HardwareI2CAlgorithm::transfer(I2CMessage& msg)
	{
		uint8_t *data;
		uint16_t addr;
		bool success;
		ssize_t rv;
		ScopedLock lock(this->_lock);

		if(unlikely(!this->isBusy())) {
			print_dbg("I2C bus busy\n");
			return -ETRYAGAIN;
		}

		data = msg.data();
		addr = msg.address();

		addr <<= 1;
		addr |= msg.isRead();

		i2c_start(addr, false);

		if(msg.isRead()) {
			auto count = msg.count();

			if(count > 1)
				i2c_read_buffer(data, count - 1, true);

			i2c_read_byte(&data[count - 1], false);
			msg.setIndex(count);
		} else {
			i2c_write_buffer(data, msg.count(), true);
		}

		i2c_stop();

		success = this->transfer();
		rv = msg.count();

		if(unlikely(!success)) {
			this->log << "I2C hardware error!" << Logger::newline;
			rv = -EINVALID;
		}

		i2c_reset();
		return rv;
	}

	ssize_t HardwareI2CAlgorithm::transfer(stl::Vector<lwiot::I2CMessage*>& msgs)
	{
		bool success;
		ssize_t total = 0L;
		bool started = false;
		ScopedLock lock(this->_lock);

		if(unlikely(!this->isBusy())) {
			print_dbg("I2C bus busy\n");
			return -ETRYAGAIN;
		}

		for(auto ptr : msgs) {
			I2CMessage& msg = *ptr;
			auto address = msg.address();
			auto data = msg.data();

			address = address << 1U | msg.isRead();
			i2c_start(address, started);
			started = true;

			if(msg.isRead()) {
				auto count = msg.count();

				if(count > 1)
					i2c_read_buffer(data, count - 1, true);

				i2c_read_byte(&data[count - 1], false);
				msg.setIndex(count);
			} else {
				i2c_write_buffer(data, msg.count(), true);
			}

			total += msg.count();

			if(msg.repstart())
				continue;

			i2c_stop();
		}

		success = this->transfer();
		i2c_reset();

		if(unlikely(!success)) {
			this->log << "I2C hardware error!" << Logger::newline;
			total = -EINVALID;
		}

		return total;
	}

#define MAX_RETRIES 3

	bool HardwareI2CAlgorithm::transfer() const
	{
		int err;
		int retries = 0;

		do {
			err = i2c_write_buffers();

			if(unlikely(err != -EOK)) {
				lwiot_sleep(this->delay());
				retries += 1;
			}
		} while(err < 0 && retries <= MAX_RETRIES);

		return err == -EOK;
	}

	bool HardwareI2CAlgorithm::busy()
	{
		ScopedLock lock(this->_lock);
		return this->isBusy();
	}

	bool HardwareI2CAlgorithm::isBusy() const
	{
		Function<bool()> is_busy = [this]() -> bool {
			bool scl, sda;

			this->_scl >> scl;
			this->_sda >> sda;
			return scl && sda;
		};

		if(is_busy()) {
			lwiot_udelay(MaxRiseTime);
			return is_busy();
		}

		return true;
	}

	bool HardwareI2CAlgorithm::test()
	{
		bool sclvalue, sdavalue;
		ScopedLock lock(this->_lock);

		sdavalue = this->_sda;
		sclvalue = this->_scl;

		auto reset = [this]() -> void {
			this->_scl << true;
			this->_sda << true;
		};

		if(!sclvalue || !sdavalue) {
			log << "Unable to test bus; bus is busy.." << Logger::newline;
			return false;
		}

		this->sdalow();
		this->_sda >> sdavalue;
		sclvalue = this->_scl;

		if(sdavalue) {
			log << "SDA stuck high!" << Logger::newline;
			reset();
			return false;
		}

		if(!sclvalue) {
			log << "SCL unexpected low" << Logger::newline;
			reset();
			return false;
		}

		this->sdahi();
		this->_sda >> sdavalue;
		this->_scl >> sclvalue;

		if(!sdavalue) {
			log << "SDA stuck low!" << Logger::newline;
			reset();
			return false;
		}

		if(!sclvalue) {
			log << "SCL unexpected low" << Logger::newline;
			reset();
			return false;
		}

		this->scllow();
		this->_sda >> sdavalue;
		this->_scl >> sclvalue;

		if(!sdavalue) {
			log << "SDA unexpected low" << Logger::newline;
			reset();
			return false;
		}

		if(sclvalue) {
			log << "SCL stuck high" << Logger::newline;
			reset();
			return false;
		}

		this->sclhi();
		this->_sda >> sdavalue;
		this->_scl >> sclvalue;

		if(!sdavalue) {
			log << "SDA unexpected low" << Logger::newline;
			reset();
			return false;
		}

		if(!sclvalue) {
			log << "SCL stuck low" << Logger::newline;
			reset();
			return false;
		}

		log << "I2C bus test OK!" << Logger::newline;
		return true;
	}

	void RAM_ATTR HardwareI2CAlgorithm::sdahi() const
	{
		this->_sda << true;
		lwiot_udelay(MaxRiseTime);
	}

	void RAM_ATTR HardwareI2CAlgorithm::sdalow() const
	{
		this->_sda << false;
		lwiot_udelay(MaxRiseTime);
	}

	void RAM_ATTR HardwareI2CAlgorithm::scllow() const
	{
		this->_scl << false;
		lwiot_udelay(MaxRiseTime);
	}

	int RAM_ATTR HardwareI2CAlgorithm::sclhi() const
	{
		time_t tmo;
		bool scl;

		this->_scl << true;
		lwiot_udelay(MaxRiseTime);

		tmo = lwiot_tick_ms() + HardwareI2CAlgorithm::Timeout;

		do {
			scl = this->_scl;

			/*
			 * Apply code path length reduction. Most calls to `sclhi()' wont get past this
			 * point since sane I2C slave do not constantly stretch the I2C clock. The
			 * check below will only fail if the I2C slave actively streches the clockline (i.e. pulls
			 * the line low).
			 */
			if(likely(scl))
				break;

			if(tmo < lwiot_tick_ms())
				return -ETMO;
		} while(!scl);

		return -EOK;
	}
}
