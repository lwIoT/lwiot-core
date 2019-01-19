/*
 * Bit-banged GPIO I2C algorithm implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/io/gpioi2calgorithm.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/log.h>
#include <lwiot/scopedlock.h>
#include <lwiot/function.h>
#include <lwiot/util/count.h>
#include <lwiot/io/watchdog.h>

#define BUSY_WAIT 10
#define READ_FLAG 0x1U

namespace lwiot
{
	GpioI2CAlgorithm::GpioI2CAlgorithm(int sda, int scl, uint32_t frequency) : GpioI2CAlgorithm(GpioPin(sda), GpioPin(scl), frequency)
	{
	}

	GpioI2CAlgorithm::GpioI2CAlgorithm(const lwiot::GpioPin &sda, const lwiot::GpioPin &scl, uint32_t frequency) :
		I2CAlgorithm(I2CAlgorithm::DefaultRetryDelay, frequency), _scl(scl), _sda(sda), _udelay(0), _lock(false)
	{
		this->_scl.setOpenDrain();
		this->_sda.setOpenDrain();

		this->_scl << true;
		this->_sda << true;

		this->_udelay = this->calcDelay(frequency);
		this->_halftime = (this->_udelay + 1) / 2;
	}

	GpioI2CAlgorithm::~GpioI2CAlgorithm()
	{
		this->_scl.mode(PinMode::INPUT_NOPULLUP);
		this->_sda.mode(PinMode::INPUT_NOPULLUP);
	}

	void GpioI2CAlgorithm::setFrequency(const uint32_t &freq)
	{
		ScopedLock lock(this->_lock);

		I2CAlgorithm::setFrequency(freq);
		this->_udelay = this->calcDelay(freq);
		this->_halftime = (this->_udelay + 1) / 2;
	}

	int GpioI2CAlgorithm::calcDelay(const uint32_t &frequency) const
	{
		double freq, tval;

		if(frequency == 0U)
			return -EINVALID;

		freq = static_cast<double>(frequency);
		tval = 1.0 / (2.0 * freq);
		tval -= 0.000002;
		tval *= 1e6;

		return static_cast<int>(tval);
	}

	void RAM_ATTR GpioI2CAlgorithm::repstart() const
	{
		this->sdahi();
		this->sclhi();
		this->_sda << false;

		lwiot_udelay(this->_udelay);
		this->scllow();
	}

	void RAM_ATTR GpioI2CAlgorithm::start() const
	{
		this->_sda << false;
		lwiot_udelay(this->_udelay);
		this->scllow();
	}

	void RAM_ATTR GpioI2CAlgorithm::stop() const
	{
		this->sdalow();
		this->sclhi();
		this->_sda(true);
		lwiot_udelay(this->_udelay);
	}

	void RAM_ATTR GpioI2CAlgorithm::sdahi() const
	{
		this->_sda << true;
		lwiot_udelay(this->_halftime);
	}

	void RAM_ATTR GpioI2CAlgorithm::sdalow() const
	{
		this->_sda << false;
		lwiot_udelay(this->_halftime);
	}

	void RAM_ATTR GpioI2CAlgorithm::scllow() const
	{
		this->_scl << false;
		lwiot_udelay(this->_halftime);
	}

	int RAM_ATTR GpioI2CAlgorithm::sclhi() const
	{
		time_t tmo;
		bool scl;

		this->_scl << true;
		lwiot_udelay(this->_udelay);
		tmo = lwiot_tick_ms() + GpioI2CAlgorithm::Timeout;

		do {
			this->_scl >> scl;

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

	bool GpioI2CAlgorithm::busy() const
	{
		bool sda, scl;

		scl = this->_scl;
		sda = this->_sda;

		if(!scl || !sda) {
			lwiot_udelay(BUSY_WAIT);
			scl = this->_scl;
			sda = this->_sda;
		}

		return !(scl && sda);
	}

	int RAM_ATTR GpioI2CAlgorithm::acknack(bool ack) const
	{
		if(ack)
			this->_sda(false);

		lwiot_udelay(this->_halftime);

		if(this->sclhi() < 0) {
			log << "ACK / NACK timeout" << Logger::newline;
			return -ETMO;
		}

		this->scllow();
		return -EOK;
	}

	int RAM_ATTR GpioI2CAlgorithm::begin(uint8_t addr, int retries)
	{
		int rv = -EINVALID;

		for(auto idx = 0; idx < retries; idx++) {
			rv = this->write(addr);

			if(rv > 0 || idx == retries)
				break;

			this->stop();
			lwiot_udelay(this->_udelay);
			this->start();
		}

		if(rv < 0)
			this->log << "unable to connect to chip on address " << addr << Logger::newline;

		return rv;
	}

	int RAM_ATTR GpioI2CAlgorithm::write(uint8_t byte)
	{
		Function<int(*)(int)> writer;
		int rv;
		bool ack;

		writer = [&](const ssize_t& idx) -> int {
			bool readback;
			bool bit;

			bit = (byte >> idx) & GpioI2CAlgorithm::LsbValue;
			this->_sda << bit;
			lwiot_udelay(this->_halftime);

			/*
			 * Treat timeouts and loss of arbitration as errors. Technically, according to
			 * the I2C specification, neither are errors. The official procedure after
			 * losing arbitration is to watch the bus for the winning master to issue its stop,
			 * and then retry the transmission (from the first message). Application level code
			 * should (and cannot) care about this.
			 */
			if(unlikely(this->sclhi() < 0)) {
				this->log << "write bit timeout at bit: " << idx << Logger::newline;
				return -ETMO;
			}

			this->_sda >> readback;
			if(unlikely(bit && !readback)) {
				this->log << "bus arbitration failed at bit: " << idx << Logger::newline;
				return -ETRYAGAIN;
			}

			this->scllow();
			return -EOK;
		};

		rv = count(GpioI2CAlgorithm::MsbPosition, 0, writer);
		if(unlikely(rv < 0))
			return rv;

		this->sdahi();

		if(unlikely(this->sclhi() < 0)) {
			this->log << "timeout while ACKing!" << Logger::newline;
			return -ETMO;
		}

		ack = !this->_sda;
		this->scllow();

		/* SCL is low and SDA in an undefined state at this point */
		return ack;
	}

	int RAM_ATTR GpioI2CAlgorithm::read(uint8_t& byte)
	{
		uint8_t value;
		Function<int(*)(int)> reader;
		int rv;

		value = 0;
		this->sdahi();

		reader = [&](const ssize_t& bit) -> int {
			bool bitvalue;
			int delay;

			if(unlikely(this->sclhi() < 0)) {
				this->log << "read timeout at bit: " << (GpioI2CAlgorithm::MsbPosition - bit) << Logger::newline;
				byte = 0;
				return -ETMO;
			}

			this->_sda >> bitvalue;
			value <<= 1;

			if(bitvalue)
				value |= GpioI2CAlgorithm::LsbValue;

			if(unlikely(bit == GpioI2CAlgorithm::MsbPosition))
				delay = this->_udelay / 2;
			else
				delay = this->_udelay;

			this->_scl << false;
			lwiot_udelay(delay);

			return -EOK;
		};

		rv = count(0, GpioI2CAlgorithm::MsbPosition + 1, reader);
		if(rv < 0) {
			byte = 0;
			return rv;
		}

		byte = value;
		return -EOK;
	}

	bool RAM_ATTR GpioI2CAlgorithm::begin(const I2CMessage& msg)
	{
		uint8_t addr, lower;
		uint16_t fulladdr;
		int rv, retries;

		retries = MAX_RETRIES;

		if(msg.is10Bit()) {
			fulladdr = msg.address();

			/* 0xF0 prefix + first two bits of address
			   The RW-bit is set to 0 for now
			*/
			addr = 0xF0 | ((fulladdr >> 7) & 0x6);
			rv = this->begin(addr, retries);

			if(rv < 0) {
				log << "Timeout at extended address!" << Logger::newline;
				return false;
			}

			/* Write the lower 8 bits of the address byte */
			lower = static_cast<uint8_t>(msg.address() & 0xFF);
			rv = this->write(lower);

			if(rv < 0) {
				log << "Terminated at second address code!" << Logger::newline;
				return false;
			}

			if(msg.isRead()) {
				this->repstart();
				addr |= READ_FLAG;
				rv = this->begin(addr, retries);

				if(rv < 0) {
					log << "Terminated at repeated start address!" << Logger::newline;
					return false;
				}
			}
		} else {
			addr = static_cast<uint8_t>(msg.address());
			addr <<= 1;

			if(msg.isRead())
				addr |= READ_FLAG;

			rv = this->begin(addr, retries);

			if(rv < 0)
				return false;
		}

		return true;
	}

	ssize_t GpioI2CAlgorithm::transfer(stl::Vector<I2CMessage*>& msgs)
	{
		ssize_t total;
		int rv;
		bool first;
		ScopedLock lock(this->_lock);

		if(this->busy())
			return -ETRYAGAIN;

		total = 0L;
		first = true;
		rv = -EOK;

		enter_critical();
		this->start();

		for(auto& _msg : msgs) {
			auto& msg = *_msg;
			if(!first) {
				if(msg.repstart()) {
					this->repstart();
				} else {
					this->stop();
					this->start();
				}
			}

			if(!this->begin(msg)) {
				log << "Device address NACKed!" << Logger::newline;
				rv = -EINVALID;
				break;
			}

			if(msg.isRead()) {
				rv = this->read(msg);

				if(rv < static_cast<ssize_t>(msg.count())) {
					rv = -EINVALID;
					log << "Unable to read wanted amount!" << Logger::newline;
					break;
				}

				total += rv;
			} else {
				rv = this->write(msg);

				if(rv < static_cast<ssize_t>(msg.count())) {
					rv = -EINVALID;
					log << "Unable to write wanted bytes!" << Logger::newline;
					break;
				}

				total += rv;
			}

			wdt.reset();
			first = false;
		}

		this->stop();
		exit_critical();

		return rv >= 0 ? total : rv;
	}

	ssize_t GpioI2CAlgorithm::transfer(I2CMessage& msg)
	{
		ssize_t total = msg.count();
		ssize_t rv;
		ScopedLock lock(this->_lock);

		if(this->busy())
			return -ETRYAGAIN;

		enter_critical();
		this->start();

		if(!this->begin(msg)) {
			log << "Device address NACKed!" << Logger::newline;
			this->stop();
			exit_critical();
			return -EINVALID;
		}

		if(msg.isRead()) {
			rv = this->read(msg);

			if(rv < total) {
				log << "Unable to read wanted amount!" << Logger::newline;
				this->stop();
				exit_critical();
				return -EINVALID;
			}
		} else {
			rv = this->write(msg);

			if(rv < total) {
				log << "Unable to write wanted bytes!" << Logger::newline;
				this->stop();
				exit_critical();
				return -EINVALID;
			}
		}

		this->stop();
		exit_critical();

		return rv;
	}

	int GpioI2CAlgorithm::read(lwiot::I2CMessage &msg)
	{
		uint8_t inval;
		size_t length;
		int total;
		int rv;

		length = msg.count();
		total = 0L;

		for(auto idx = 0U; idx < length; idx++) {
			rv = this->read(inval);

			if(rv < 0)
				break;

			total++;
			msg[idx] = inval;

			rv = this->acknack(idx+1 != length);
			if(rv < 0)
				return rv;
		}

		msg.setIndex(length);
		return total;
	}

	int GpioI2CAlgorithm::write(const I2CMessage& msg)
	{
		size_t count;
		int rv;
		int written;

		count = msg.count();
		written = 0;

		for(auto idx = 0UL; idx < count; idx++) {
			const auto& byte = msg[idx];
			rv = this->write(byte);

			if(rv > 0) {
				written ++;
			} else if(rv == 0) {
				log << "write-byte: NACK received!" << Logger::newline;
				return -EINVALID;
			} else {
				log << "write-byte ACK timeout!" << Logger::newline;
				return rv;
			}
		}

		return written;
	}

	bool GpioI2CAlgorithm::test()
	{
		bool scl, sda;
		ScopedLock lock(this->_lock);

		sda = this->_sda;
		scl = this->_scl;

		auto reset = [this]() -> void {
			this->_scl << true;
			this->_sda << true;
		};

		if(!scl || !sda) {
			log << "Unable to test bus; bus is busy.." << Logger::newline;
			return false;
		}

		this->sdalow();
		this->_sda >> sda;
		this->_scl >> scl;

		if(sda) {
			log << "SDA stuck high!" << Logger::newline;
			reset();
			return false;
		}

		if(!scl) {
			log << "SCL unexpected low" << Logger::newline;
			reset();
			return false;
		}

		this->sdahi();
		sda = this->_sda;
		scl = this->_scl;

		if(!sda) {
			log << "SDA stuck low!" << Logger::newline;
			reset();
			return false;
		}

		if(!scl) {
			log << "SCL unexpected low" << Logger::newline;
			reset();
			return false;
		}

		this->scllow();
		this->_sda >> sda;
		this->_scl >> scl;

		if(!sda) {
			log << "SDA unexpected low" << Logger::newline;
			reset();
			return false;
		}

		if(scl) {
			log << "SCL stuck high" << Logger::newline;
			reset();
			return false;
		}

		this->sclhi();
		sda = this->_sda;
		scl = this->_scl;

		if(!sda) {
			log << "SDA unexpected low" << Logger::newline;
			reset();
			return false;
		}

		if(!scl) {
			log << "SCL stuck low" << Logger::newline;
			reset();
			return false;
		}

		log << "I2C bus test OK!" << Logger::newline;
		return true;
	}
}
