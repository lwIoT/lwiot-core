/*
 * XBee device implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeeaddress.h>
#include <lwiot/network/xbee/xbeeresponse.h>
#include <lwiot/network/xbee/xbeerequest.h>

#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	class AsyncXbee;

	class XBee {
	public:
		explicit XBee();
		virtual ~XBee() = default;
		explicit XBee(const XBee& xb);
		explicit XBee(const XBee&& xb) noexcept ;

		XBee& operator=(const XBee& xb);
		XBee& operator=(const XBee&& xb) noexcept ;

		void readPacket();
		bool readPacket(int timeout);
		void readPacketUntilAvailable();
		void begin(Stream &serial);
		void getResponse(XBeeResponse &response);
		XBeeResponse& getResponse();
		void send(XBeeRequest &request);
		uint8_t getNextFrameId();
		void setSerial(Stream &serial);

		void apply();
		uint64_t getNetworkAddress();
		void setNetworkID(uint16_t netid);
		void setChannel(uint16_t channel);
		void setZigbeePro(bool enabled);

	protected:
		virtual void copy(const XBee& rhs);

	private:
		friend class AsyncXbee;

		XBeeResponse _response;
		bool _escape;
		uint8_t _pos;
		uint8_t b;
		uint8_t _checksumTotal;
		uint8_t _nextFrameId;
		uint8_t _responseFrameData[MAX_FRAME_DATA_SIZE];

		stl::ReferenceWrapper<Stream> _serial;

		bool available();
		uint8_t read();
		void write(uint8_t val);
		void sendByte(uint8_t b, bool escape);
		void resetResponse();
		ByteBuffer sendCommand(const uint8_t* cmd, int tmo, const uint8_t* value = nullptr, size_t length = 0U);
	};
}
