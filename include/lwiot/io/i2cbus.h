/*
 * I2C serial bus wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file i2cbus.h

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/stl/vector.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/io/i2chal.h>
#include <lwiot/io/i2calgorithm.h>

namespace lwiot
{
	/**
	 * @brief I2C bus peripheral abstraction.
	 * @ingroup io
	 */
	class I2CBus {
	public:
		/**
		 * @brief Construct an I2C bus.
		 */
		explicit I2CBus();

		/**
		 * @brief Copy an I2C bus.
		 * @param bus
		 */
		I2CBus(const I2CBus& bus);

		/**
		 * @brief Construct an I2C bus.
		 * @param algo Algorithm to use.
		 */
		explicit I2CBus(I2CAlgorithm *algo);

		/**
		 * @brief I2C bus copy assignment operator.
		 * @param bus Bus to copy.
		 * @return A reference to \p *this.
		 */
		I2CBus& operator=(const I2CBus& bus);

		/**
		 * @brief Initiate a data transfer.
		 * @param msg I2C message.
		 * @return Success indicator.
		 */
		bool transfer(I2CMessage& msg);

		/**
		 * @brief Initiate a data transfer.
		 * @param msgs I2C message.
		 * @return Success indicator.
		 */
		bool transfer(stl::Vector<I2CMessage>& msgs);

		/**
		 * @brief Send data.
		 * @param sla Slave address.
		 * @param buffer Buffer to send.
		 * @return Success indicator.
		 */
		bool send(int8_t  sla, const ByteBuffer& buffer);

		/**
		 * @brief Send data.
		 * @param sla Slave address.
		 * @param buffer Buffer to send.
		 * @param size Number of bytes to send.
		 * @return Success indicator.
		 */
		bool send(int8_t sla, const void *buffer, size_t size);

		/**
		 * @brief Set the bus frequency.
		 * @param freq Frequency to set.
		 */
		void setFrequency(const uint32_t& freq);

		/**
		 * @brief Set the I2C algorithm.
		 * @param algo Algo to set.
		 */
		void setAlgorithm(I2CAlgorithm* algo);

		/**
		 * @brief I2C algorithm getter.
		 * @return The current algorithm.
		 */
		I2CAlgorithm *algorithm() const;

		/**
		 * @brief Get the current timeout.
		 * @return The timeout.
		 */
		inline int timeout() const
		{
			return this->_timeout;
		}

		/**
		 * @brief Set the current timeout.
		 * @param tmo Timeout value.
		 */
		inline void setTimeout(int tmo)
		{
			this->_timeout = tmo;
		}

	private:
		SharedPointer<I2CAlgorithm> _algo;
		SharedPointer<Lock> _lock;
		int _timeout;
	};
}
