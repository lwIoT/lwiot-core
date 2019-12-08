/*
 * Async wrapper around the Xbee libraries.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file asyncxbee.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/scopedlock.h>
#include <lwiot/function.h>

#include <lwiot/stl/forward.h>
#include <lwiot/stl/move.h>

#include <lwiot/stream.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/lock.h>

#include <lwiot/network/xbee/xbee.h>
#include <lwiot/network/xbee/asyncxbee.h>
#include <lwiot/network/xbee/xbeeresponse.h>
#include <lwiot/network/xbee/xbeeaddress.h>
#include <lwiot/network/xbee/xbeerequest.h>

namespace lwiot
{
	/**
	 * @brief Asynchronous XBee driver.
	 */
	class AsyncXbee : public Thread {
	public:
		typedef Function<void(XBeeResponse&)> ResponseHandler; //!< Response handler.

		explicit AsyncXbee(); //!< Construct a new XBee driver.
		explicit AsyncXbee(XBee& xb); //!< Construct a new XBee driver.
		explicit AsyncXbee(Stream& stream); //!< Construct a new XBee driver.
		~AsyncXbee() override; //!< Destroy an XBee driver.

		/**
		 * @brief Start the XBee driver.
		 * @tparam T Handler functor type.
		 * @param handler Handler functor.
		 */
		template <typename T>
		void begin(T&& handler)
		{
			this->_handler = handler;
			this->init();
		}

		/**
		 * @brief Start the XBee driver.
		 * @param handler Handler functor.
		 */
		void begin(const ResponseHandler& handler);

		/**
		 * @brief Set the response handler.
		 * @param handler Response handler to set.
		 */
		void setHandler(const ResponseHandler& handler);

		/**
		 * @brief Set the underlying XBee driver.
		 * @param xb XBee driver to set.
		 * @see XBee
		 */
		void setDevice(XBee& xb);

		/**
		 * @brief Get the underlying XBee device.
		 * @return The underlying XBee device driver.
		 */
		XBee& getDevice();

		/**
		 * @brief Send a request.
		 * @param request Request to send.
		 * @return Success indicator.
		 */
		bool send(XBeeRequest& request) const;

		/**
		 * @brief Send a request.
		 * @tparam Func Callback type.
		 * @param rq Request to send.
		 * @param callback handler.
		 * @return Success indicator.
		 */
		template <typename Func>
		void send(XBeeRequest& rq, Func&& callback) const
		{
			auto success = this->send(rq);
			callback(success);
		}

		bool transmit(const stl::String& data, uint16_t addr) const; //!< Transmit data.
		bool transmit(const stl::String& data, uint64_t addr) const; //!< Transmit data.
		bool transmit(ZigbeeAddress addr, const ByteBuffer& buffer) const; //!< Transmit data.
		bool transmit(ZigbeeAddress addr, const ByteBuffer& buffer, uint16_t profile, uint16_t cluster) const; //!< Transmit data.

		/**
		 * @brief Set a handler (callback).
		 * @tparam Func Callback type.
		 * @param handler Handler to set.
		 * @see ResponseHandler
		 */
		template <typename Func>
		void setHandler(Func&& handler)
		{
			UniqueLock<Lock> lock(this->_lock);
			this->_handler = stl::forward<Func>(handler);
		}

		uint64_t getHardwareAddress() const; //!< Get the hardware address.
		uint16_t getNetworkAddress() const; //!< Get the network address.

		void setNetworkID(uint16_t netid) const; //!< Set the PAN ID.
		void setChannel(uint16_t channel) const; //!< Set the channel.
		void setZigbeePro(bool enabled) const; //!< Enable/disable ZigBee PRO.

		void enableEncryption(bool enabled) const; //!< Enable/disable encryption.

		/**
		 * @brief Configure encryption.
		 * @param opts Encryption options.
		 */
		void setEncryptionOptions(XBee::EncryptionOptions opts) const;

		/**
		 * @brief Set the link key.
		 * @param key Key to set.
		 * @note This key is used to encrypt the network key for joining devices.
		 */
		void setLinkKey(const ByteBuffer& key) const;

		/**
		 * @brief Set the network key.
		 * @param key Pre-shared network key.
		 */
		void setNetworkKey(const ByteBuffer& key) const;

		/**
		 * @brief Configure the sleep strategy.
		 * @param mode Sleep mode to set.
		 */
		void setSleepMode(XBee::SleepMode mode) const; //!< Configure the sleep mode.
		uint16_t getParentAddress() const; //!< Get the parent device address.
		uint8_t getMaxPayloadSize() const; //!< Get the maximum payload size.
		void setMaxHops(uint8_t max) const; //!< Set the maximum number of hops.
		void setNodeIdentifier(const stl::String& id) const; //!< Set the node ID.
		void enableCoordinator(bool enable) const; //!< Enable/disable coordinator mode.

		void writeToFlash() const; //!< Write settings to flash.

	protected:
		void run() override; //!< Run the async driver.
		void init(); //!< Start the driver.

	private:
		mutable Lock _lock;
		ResponseHandler _handler;
		bool _running;
		mutable XBee _xb;

		bool validateTxRequest() const;
	};
}
