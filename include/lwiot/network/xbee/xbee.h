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
#include <lwiot/network/zigbeeaddress.h>

#include <lwiot/stl/array.h>
#include <lwiot/stl/referencewrapper.h>

#include <lwiot/io/gpiopin.h>

namespace lwiot
{
	class AsyncXbee;

	/**
	 * @brief XBee device driver.
	 * @ingroup xb
	 */
	class XBee {
	public:
		/**
		 * @brief Sleep mode.
		 */
		enum SleepMode {
			NoSleep = 0,
			PinHibernate = 1,
			CyclicSleep = 4,
			CyclicSleepPinWakeUp = 5
		};

		/**
		 * @brief Encryption options.
		 */
		enum EncryptionOptions {
			None = 0,
			TrustCenter = 2,
			UseTrustCenter = 8
		};

		explicit XBee(); //!< Construct a new XBee device driver.
		virtual ~XBee() = default; //!< Destroy a XBee device driver.
		explicit XBee(const XBee& xb); //!< Construct a new XBee device driver.
		explicit XBee(XBee&& xb) noexcept ; //!< Construct a new XBee device driver.

		XBee& operator=(const XBee& xb); //!< Copy assignment operator.
		XBee& operator=(XBee&& xb) noexcept ; //!< Move assignment operator.

		void readPacket(); //!< Read a single packet.

		/**
		 * @brief Attempt to read a packet for \p timeout miliseconds.
		 * @see readPacket
		 * @param timeout Period to attempt to read a packet.
		 * @return True or false based on whether a new packet was received.
		 */
		bool readPacket(int timeout);
		void readPacketUntilAvailable(); //!< Block unil a new packet has been received.

		/**
		 * @brief Start the XBee device driver.
		 * @param serial Serial connection.
		 */
		void begin(Stream &serial);

		/**
		 * @brief Get the XBee response.
		 * @param response The XBee response.
		 */
		void getResponse(XBeeResponse &response);

		/**
		 * @brief Get the XBee response.
		 * @return The XBee response.
		 */
		XBeeResponse& getResponse();

		void send(XBeeRequest &request) const; //!< Send data.
		void send(ZigbeeAddress addr, const ByteBuffer& buffer) const; //!< Send data.
		void send(ZigbeeAddress addr, const ByteBuffer& buffer, uint16_t profile, uint16_t cluster) const; //!< Send data.
		uint8_t getNextFrameId(); //!< Generate a new frame ID.

		/**
		 * @brief Set the serial driver.
		 * @param serial Serial stream.
		 */
		void setSerial(Stream &serial);

		/**
		 * @brief Set the sleep request pin.
		 * @param pin Sleep request pin to set.
		 */
		void setSleepPin(const GpioPin& pin);

		void apply(); //!< Apply new settings.

		uint64_t getHardwareAddress(); //!< Get the hardware address.
		void setNetworkID(uint16_t netid); //!< Set the PAN ID.
		void setChannel(uint16_t channel); //!< Set the channel.
		void setZigbeePro(bool enabled); //!< Enable/disable ZigBee PRO.
		uint16_t getNetworkAddress(); //!< Get the network address.
		uint16_t getParentAddress(); //!< Get the parent device address.

		void enableEncryption(bool enabled); //!< Enable/disable encryption.

		/**
		 * @brief Configure encryption.
		 * @param opts Encryption options.
		 */
		void setEncryptionOptions(EncryptionOptions opts);

		/**
		 * @brief Set the link key.
		 * @param key Key to set.
		 * @note This key is used to encrypt the network key for joining devices.
		 */
		void setLinkKey(const ByteBuffer& key);

		/**
		 * @brief Set the network key.
		 * @param key Pre-shared network key.
		 */
		void setNetworkKey(const ByteBuffer& key);
		void setNodeIdentifier(const stl::String& id); //!< Set the node ID.
		void setMaxHops(uint8_t max); //!< Set the maximum number of hops.
		void enableCoordinator(bool enable); //!< Enable/disable coordinator mode.
		void writeToFlash(); //!< Write settings to flash.
		uint8_t getMaxPayloadSize() const; //!< Get the maximum payload size.
		void setSleepMode(SleepMode sleepmode); //!< Configure the sleep mode.
		void sleep(); //!< Sleep the XBee device.
		void wakeUp(); //!< Wake the XBee device.

	protected:
		virtual void copy(const XBee& rhs); //!< Copy an XBee device.

	private:
		friend class AsyncXbee;

		XBeeResponse _response;
		bool _escape;
		uint8_t _pos;
		uint8_t b;
		uint8_t _checksumTotal;
		uint8_t _nextFrameId;
//		uint8_t _responseFrameData[MAX_FRAME_DATA_SIZE];
		stl::Array<uint8_t, MAX_FRAME_DATA_SIZE> _responseFrameData;
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
