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
		typedef Function<void(const XBeeResponse&)> ResponseHandler;

		explicit AsyncXbee();
		explicit AsyncXbee(XBee& xb);
		explicit AsyncXbee(Stream& stream);
		virtual ~AsyncXbee();

		void begin(ResponseHandler&& handler);
		void begin(const ResponseHandler& handler);

		void setHandler(const ResponseHandler& handler);
		void setDevice(XBee& xb);
		XBee& getDevice();

		template <typename Func>
		void setHandler(Func&& handler)
		{
			UniqueLock<Lock> lock(this->_lock);
			this->_handler = stl::forward<Func>(handler);
		}

		void setNetworkID(uint16_t netid);
		void setChannel(uint8_t channel);
		uint64_t getNetworkAddress();

	protected:
		void run() override;

	private:
		mutable Lock _lock;
		ResponseHandler _handler;
		bool _running;
		XBee _xb;

		/* Methods */
		ByteBuffer sendCommand(const uint8_t* cmd, const uint8_t* value = nullptr, size_t length = 0U);
	};
}
