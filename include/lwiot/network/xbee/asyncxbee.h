/*
 * Async wrapper around the Xbee libraries.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
	class AsyncXbee : public Thread {
	public:
		typedef Function<void(XBeeResponse&)> ResponseHandler;

		explicit AsyncXbee();
		explicit AsyncXbee(XBee& xb);
		explicit AsyncXbee(Stream& stream);
		virtual ~AsyncXbee();

		void begin(ResponseHandler&& handler);
		void begin(const ResponseHandler& handler);

		void setHandler(const ResponseHandler& handler);
		void setDevice(XBee& xb);
		XBee& getDevice();

		bool send(XBeeRequest& request) const;

		template <typename Func>
		void send(XBeeRequest& rq, Func&& callback) const
		{
			auto success = this->send(rq);
			callback(success);
		}

		bool transmit(const stl::String& data, uint16_t addr) const;
		bool transmit(const stl::String& data, uint64_t addr) const;

		template <typename Func>
		void setHandler(Func&& handler)
		{
			UniqueLock<Lock> lock(this->_lock);
			this->_handler = stl::forward<Func>(handler);
		}

		uint64_t getHardwareAddress() const;
		uint16_t getNetworkAddress() const;

		void setNetworkID(uint16_t netid) const;
		void setChannel(uint16_t channel) const;
		void setZigbeePro(bool enabled) const;

		void enableEncryption(bool enabled) const;
		void setEncryptionOptions(XBee::EncryptionOptions opts) const;
		void setLinkKey(const ByteBuffer& key) const;
		void setNetworkKey(const ByteBuffer& key) const;

		void setSleepMode(XBee::SleepMode mode) const;

	protected:
		void run() override;
		void init();

	private:
		mutable Lock _lock;
		ResponseHandler _handler;
		bool _running;
		mutable XBee _xb;

		bool validateTxRequest() const;
	};
}
