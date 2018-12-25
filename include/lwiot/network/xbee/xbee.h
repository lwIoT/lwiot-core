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

namespace lwiot
{
	class XBee {
	public:
		explicit XBee();

		void readPacket();
		bool readPacket(int timeout);
		void readPacketUntilAvailable();
		void begin(Stream &serial);
		void getResponse(XBeeResponse &response);
		XBeeResponse& getResponse();
		void send(XBeeRequest &request);
		uint8_t getNextFrameId();
		void setSerial(Stream &serial);

	private:
		XBeeResponse _response;
		bool _escape;
		uint8_t _pos;
		uint8_t b;
		uint8_t _checksumTotal;
		uint8_t _nextFrameId;
		uint8_t _responseFrameData[MAX_FRAME_DATA_SIZE];

		Stream* _serial;

		bool available();
		uint8_t read();
		void flush();
		void write(uint8_t val);
		void sendByte(uint8_t b, bool escape);
		void resetResponse();
	};
}
