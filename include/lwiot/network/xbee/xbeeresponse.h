/*
 * XBee response (RX) base class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeeaddress.h>

namespace lwiot
{
	class XBeeResponse {
	public:
		explicit XBeeResponse();

		uint8_t getApiId();

		void setApiId(uint8_t apiId);

		uint8_t getMsbLength();

		void setMsbLength(uint8_t msbLength);

		uint8_t getLsbLength();

		void setLsbLength(uint8_t lsbLength);

		uint8_t getChecksum();

		void setChecksum(uint8_t checksum);

		uint8_t getFrameDataLength();

		void setFrameData(uint8_t *frameDataPtr);

		uint8_t *getFrameData();

		void setFrameLength(uint8_t frameLength);

		uint16_t getPacketLength();

		void reset();

		void init();

#ifdef SERIES_2

		void getZBTxStatusResponse(XBeeResponse &response);

		void getZBRxResponse(XBeeResponse &response);

		void getZBExplicitRxResponse(XBeeResponse &response);

		void getZBRxIoSampleResponse(XBeeResponse &response);

#endif

#ifdef SERIES_1

		void getTxStatusResponse(XBeeResponse &response);

		void getRx16Response(XBeeResponse &response);

		void getRx64Response(XBeeResponse &response);

		void getRx16IoSampleResponse(XBeeResponse &response);

		void getRx64IoSampleResponse(XBeeResponse &response);

#endif

		void getAtCommandResponse(XBeeResponse &responses);

		void getRemoteAtCommandResponse(XBeeResponse &response);

		void getModemStatusResponse(XBeeResponse &response);

		bool isAvailable();

		void setAvailable(bool complete);

		bool isError();

		uint8_t getErrorCode();

		void setErrorCode(uint8_t errorCode);

	protected:
		uint8_t *_frameDataPtr;

	private:
		uint8_t _apiId;
		uint8_t _msbLength;
		uint8_t _lsbLength;
		uint8_t _checksum;
		uint8_t _frameLength;
		bool _complete;
		uint8_t _errorCode;

		void setCommon(XBeeResponse &target);
	};

	class FrameIdResponse : public XBeeResponse {
	public:
		FrameIdResponse();

		uint8_t getFrameId();

	private:
		uint8_t _frameId;
	};

/**
 * Common functionality for both Series 1 and 2 data RX data packets
 */
	class RxDataResponse : public XBeeResponse {
	public:
		RxDataResponse();

		/**
		 * Returns the specified index of the payload.  The index may be 0 to getDataLength() - 1
		 * This method is deprecated; use uint8_t* getData()
		 */
		uint8_t getData(int index);

		/**
		 * Returns the payload array.  This may be accessed from index 0 to getDataLength() - 1
		 */
		uint8_t *getData();

		/**
		 * Returns the length of the payload
		 */
		virtual uint8_t getDataLength() = 0;

		/**
		 * Returns the position in the frame data where the data begins
		 */
		virtual uint8_t getDataOffset() = 0;
	};

// getResponse to return the proper subclass:
// we maintain a pointer to each type of response, when a response is parsed, it is allocated only if NULL
// can we allocate an object in a function?

#ifdef SERIES_2

/**
 * Represents a Series 2 TX status packet
 */
	class ZBTxStatusResponse : public FrameIdResponse {
	public:
		ZBTxStatusResponse();

		uint16_t getRemoteAddress();

		uint8_t getTxRetryCount();

		uint8_t getDeliveryStatus();

		uint8_t getDiscoveryStatus();

		bool isSuccess();

		static const uint8_t API_ID = ZB_TX_STATUS_RESPONSE;
	};

/**
 * Represents a Series 2 RX packet
 */
	class ZBRxResponse : public RxDataResponse {
	public:
		ZBRxResponse();

		XBeeAddress64 &getRemoteAddress64();

		uint16_t getRemoteAddress16();

		virtual uint8_t getOption();

		uint8_t getDataLength() override;

		// frame position where data starts
		uint8_t getDataOffset() override;

		static const uint8_t API_ID = ZB_RX_RESPONSE;
	private:
		XBeeAddress64 _remoteAddress64;
	};

/**
 * Represents a Series 2 Explicit RX packet
 *
 * Note: The receive these responses, set AO=1. With the default AO=0,
 * you will receive ZBRxResponses, not knowing exact details.
 */
	class ZBExplicitRxResponse : public ZBRxResponse {
	public:
		ZBExplicitRxResponse();

		uint8_t getSrcEndpoint();

		uint8_t getDstEndpoint();

		uint16_t getClusterId();

		uint16_t getProfileId();

		uint8_t getOption() override;

		uint8_t getDataLength() override;

		uint8_t getDataOffset() override;

		static const uint8_t API_ID = ZB_EXPLICIT_RX_RESPONSE;
	};

/**
 * Represents a Series 2 RX I/O Sample packet
 */
	class ZBRxIoSampleResponse : public ZBRxResponse {
	public:
		ZBRxIoSampleResponse();

		bool containsAnalog();

		bool containsDigital();

		/**
		 * Returns true if the pin is enabled
		 */
		bool isAnalogEnabled(uint8_t pin);

		/**
		 * Returns true if the pin is enabled
		 */
		bool isDigitalEnabled(uint8_t pin);

		/**
		 * Returns the 10-bit analog reading of the specified pin.
		 * Valid pins include ADC:xxx.
		 */
		uint16_t getAnalog(uint8_t pin);

		/**
		 * Returns true if the specified pin is high/on.
		 * Valid pins include DIO:xxx.
		 */
		bool isDigitalOn(uint8_t pin);

		uint8_t getDigitalMaskMsb();

		uint8_t getDigitalMaskLsb();

		uint8_t getAnalogMask();

		static const uint8_t API_ID = ZB_IO_SAMPLE_RESPONSE;
	};

#endif

#ifdef SERIES_1

/**
 * Represents a Series 1 TX Status packet
 */
	class TxStatusResponse : public FrameIdResponse {
	public:
		TxStatusResponse();

		uint8_t getStatus();

		bool isSuccess();

		static const uint8_t API_ID = TX_STATUS_RESPONSE;
	};

/**
 * Represents a Series 1 RX packet
 */
	class RxResponse : public RxDataResponse {
	public:
		RxResponse();

		// remember rssi is negative but this is unsigned byte so it's up to you to convert
		uint8_t getRssi();

		uint8_t getOption();

		bool isAddressBroadcast();

		bool isPanBroadcast();

		uint8_t getDataLength() override;

		uint8_t getDataOffset() override;

		virtual uint8_t getRssiOffset() = 0;
	};

/**
 * Represents a Series 1 16-bit address RX packet
 */
	class Rx16Response : public RxResponse {
	public:
		Rx16Response();

		uint8_t getRssiOffset() override;

		uint16_t getRemoteAddress16();

		static const uint8_t API_ID = RX_16_RESPONSE;
	protected:
		uint16_t _remoteAddress;
	};

/**
 * Represents a Series 1 64-bit address RX packet
 */
	class Rx64Response : public RxResponse {
	public:
		Rx64Response();

		uint8_t getRssiOffset() override;

		XBeeAddress64 &getRemoteAddress64();

		static const uint8_t API_ID = RX_64_RESPONSE;
	private:
		XBeeAddress64 _remoteAddress;
	};

/**
 * Represents a Series 1 RX I/O Sample packet
 */
	class RxIoSampleBaseResponse : public RxResponse {
	public:
		RxIoSampleBaseResponse();

		/**
		 * Returns the number of samples in this packet
		 */
		uint8_t getSampleSize();

		bool containsAnalog();

		bool containsDigital();

		/**
		 * Returns true if the specified analog pin is enabled
		 */
		bool isAnalogEnabled(uint8_t pin);

		/**
		 * Returns true if the specified digital pin is enabled
		 */
		bool isDigitalEnabled(uint8_t pin);

		/**
		 * Returns the 10-bit analog reading of the specified pin.
		 * Valid pins include ADC:0-5.  Sample index starts at 0
		 */
		uint16_t getAnalog(uint8_t pin, uint8_t sample);

		/**
		 * Returns true if the specified pin is high/on.
		 * Valid pins include DIO:0-8.  Sample index starts at 0
		 */
		bool isDigitalOn(uint8_t pin, uint8_t sample);

		uint8_t getSampleOffset();

		/**
		 * Gets the offset of the start of the given sample.
		 */
		uint8_t getSampleStart(uint8_t sample);

	private:
	};

	class Rx16IoSampleResponse : public RxIoSampleBaseResponse {
	public:
		Rx16IoSampleResponse();

		uint16_t getRemoteAddress16();

		uint8_t getRssiOffset() override;

		static const uint8_t API_ID = RX_16_IO_RESPONSE;
	};

	class Rx64IoSampleResponse : public RxIoSampleBaseResponse {
	public:
		Rx64IoSampleResponse();

		XBeeAddress64 &getRemoteAddress64();

		uint8_t getRssiOffset() override;

		static const uint8_t API_ID = RX_64_IO_RESPONSE;
	private:
		XBeeAddress64 _remoteAddress;
	};

#endif

/**
 * Represents a Modem Status RX packet
 */
	class ModemStatusResponse : public XBeeResponse {
	public:
		ModemStatusResponse();
		uint8_t getStatus();

		static const uint8_t API_ID = MODEM_STATUS_RESPONSE;
	};

	class AtCommandResponse : public FrameIdResponse {
	public:
		AtCommandResponse();

		virtual uint8_t *getCommand();
		virtual uint8_t getStatus();
		virtual uint8_t *getValue();
		virtual uint8_t getValueLength();
		virtual bool isOk();

		static const uint8_t API_ID = AT_COMMAND_RESPONSE;
	};

/**
 * Represents a Remote AT Command RX packet
 */
	class RemoteAtCommandResponse : public AtCommandResponse {
	public:
		RemoteAtCommandResponse();

		/**
		 * Returns an array containing the two character command
		 */
		uint8_t *getCommand() override;

		/**
		 * Returns the command status code.
		 * Zero represents a successful command
		 */
		uint8_t getStatus() override;

		/**
		 * Returns an array containing the command value.
		 * This is only applicable to query commands.
		 */
		uint8_t *getValue() override;

		/**
		 * Returns the length of the command value array.
		 */
		uint8_t getValueLength() override;

		/**
		 * Returns the 16-bit address of the remote radio
		 */
		uint16_t getRemoteAddress16();

		/**
		 * Returns the 64-bit address of the remote radio
		 */
		XBeeAddress64 &getRemoteAddress64();

		/**
		 * Returns true if command was successful
		 */
		bool isOk() override;

		static constexpr uint8_t API_ID = REMOTE_AT_COMMAND_RESPONSE;

	private:
		XBeeAddress64 _remoteAddress64;
	};
}
