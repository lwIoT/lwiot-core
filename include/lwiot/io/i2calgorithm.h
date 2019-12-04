/*
 * I2C algorithm base class.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file i2calgorithm.h

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>

#include <lwiot/stl/vector.h>
#include <lwiot/io/i2cmessage.h>

#define MAX_RETRIES 3

namespace lwiot
{
	/**
	 * @brief I2C algorithm base class.
	 * @ingroup io
	 */
	class I2CAlgorithm {
	public:
		/**
		 * @brief Construct a new algorithm.
		 * @param delay Retry delay.
		 * @param frequency Bus frequency.
		 */
		explicit I2CAlgorithm(int delay, uint32_t frequency);


		/**
		 * @brief Construct a new algorithm.
		 */
		explicit I2CAlgorithm();
		/**
		 * @brief Construct a new algorithm.
		 * @param other Algorithm to move.
		 */
		I2CAlgorithm(I2CAlgorithm&& other) = default;
		/**
		 * @brief Construct a new algorithm.
		 * @param other Algorithm to copy.
		 */
		I2CAlgorithm(const I2CAlgorithm& other) = default;
		virtual ~I2CAlgorithm() = default; //!< Algorithm destructor.

		/**
		 * @brief Copy assignment operator.
		 * @param rhs Algorithm to copy.
		 * @return A reference to \p *this.
		 */
		I2CAlgorithm& operator=(const I2CAlgorithm& rhs) = default;

		/**
		 * @brief Move assignment operator.
		 * @param rhs Algorithm to move.
		 * @return A reference to \p *this.
		 */
		I2CAlgorithm& operator=(I2CAlgorithm&& rhs) = default;

		/**
		 * @brief Get the frequency.
		 * @return The bus frequency.
		 */
		const uint32_t& frequency() const;

		/**
		 * @brief Get the algorithm retry delay.
		 * @return The retry delay.
		 */
		int delay() const;

		/**
		 * @brief Set the bus frequency.
		 * @param freq Frequency to set.
		 */
		virtual void setFrequency(const uint32_t& freq);

		/**
		 * @brief Initiate a data transfer.
		 * @param msg I2C message.
		 * @return Number of bytes transfered.
		 */
		virtual ssize_t transfer(I2CMessage& msg) = 0;

		/**
		 * @brief Initiate a data transfer.
		 * @param msgs I2C messages.
		 * @return Number of bytes transfered.
		 */
		virtual ssize_t transfer(stl::Vector<I2CMessage>& msgs) = 0;

	protected:
		uint32_t _frequency; //!< Bus frequency.
		static constexpr int DefaultRetryDelay = 200; //!< Default retry delay.

		/**
		 * @brief Copy operation.
		 * @param other Object to copy.
		 */
		virtual void copy(const I2CAlgorithm& other);

	private:
		int _delay;

		static constexpr uint32_t DefaultFrequency = 100000UL;
	};
}
