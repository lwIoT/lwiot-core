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
#include <lwiot/io/gpiopin.h>

namespace lwiot
{
	class AsyncXbee;

	class XBee {
	public:
		enum SleepMode {
			NoSleep = 0,
			PinHibernate = 1,
			CyclicSleep = 4,
			CyclicSleepPinWakeUp = 5
		};

		enum EncryptionOptions {
			None = 0,
			TrustCenter = 2,
			UseTrustCenter = 8
		};

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
		void send(XBeeRequest &request) const;
		uint8_t getNextFrameId();
		void setSerial(Stream &serial);
		void setSleepPin(const GpioPin& pin);

		void apply();
		uint64_t getHardwareAddress();
		void setNetworkID(uint16_t netid);
		void setChannel(uint16_t channel);
		void setZigbeePro(bool enabled);
		uint16_t getNetworkAddress();
		uint16_t getParentAddress();

		void enableEncryption(bool enabled);
		void setEncryptionOptions(EncryptionOptions opts);
		void setLinkKey(const ByteBuffer& key);
		void setNetworkKey(const ByteBuffer& key);
		void setNodeIdentifier(const stl::String& id);
		void setMaxHops(uint8_t max);
		void enableCoordinator(bool enable);

		void writeToFlash();

		uint8_t getMaxPayloadSize() const;

		void setSleepMode(SleepMode sleepmode);
		void sleep();
		void wakeUp();

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
		uint8_t _max_payload;

		stl::ReferenceWrapper<Stream> _serial;
		GpioPin _sleep_pin;

		bool available();
		uint8_t read();
		void write(uint8_t val) const;
		void sendByte(uint8_t b, bool escape) const;
		void resetResponse();
		ByteBuffer sendCommand(const uint8_t* cmd, int tmo, const uint8_t* value = nullptr, size_t length = 0U);
		void fetchMaxPayloadSize();
	};
}
