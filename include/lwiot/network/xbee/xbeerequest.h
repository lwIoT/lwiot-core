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
	class XBeeRequest {
	public:
		explicit XBeeRequest(uint8_t apiId, uint8_t frameId);

		virtual ~XBeeRequest() = default;

		void setFrameId(uint8_t frameId);

		uint8_t getFrameId();

		uint8_t getApiId();

		virtual uint8_t getFrameData(uint8_t pos) = 0;

		virtual uint8_t getFrameDataLength() = 0;

	protected:
		void setApiId(uint8_t apiId);

	private:
		uint8_t _apiId;
		uint8_t _frameId;
	};

	class PayloadRequest : public XBeeRequest {
	public:
		PayloadRequest(uint8_t apiId, uint8_t frameId, uint8_t *payload, uint8_t payloadLength);
		~PayloadRequest() override = default;

		uint8_t *getPayload();
		void setPayload(uint8_t *payloadPtr);

		void setPayload(uint8_t *payloadPtr, uint8_t payloadLength)
		{
			setPayload(payloadPtr);
			setPayloadLength(payloadLength);
		}

		uint8_t getPayloadLength();
		void setPayloadLength(uint8_t payloadLength);

	private:
		uint8_t *_payloadPtr;
		uint8_t _payloadLength;
	};

#ifdef SERIES_1
	/*
	 * Represents a Series 1 TX packet that corresponds to Api Id: TX_16_REQUEST
	 * <p/>
	 * Be careful not to send a data array larger than the max packet size of your radio.
	 * This class does not perform any validation of packet size and there will be no indication
	 * if the packet is too large, other than you will not get a TX Status response.
	 * The datasheet says 100 bytes is the maximum, although that could change in future firmware.
	 */
	class Tx16Request : public PayloadRequest {
	public:
		Tx16Request();
		Tx16Request(uint16_t addr16, uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId);
		Tx16Request(uint16_t addr16, uint8_t *payload, uint8_t payloadLength);
		~Tx16Request() override = default;

		uint16_t getAddress16();
		void setAddress16(uint16_t addr16);
		uint8_t getOption();
		void setOption(uint8_t option);
		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;

	protected:
	private:
		uint16_t _addr16;
		uint8_t _option;
	};

	class Tx64Request : public PayloadRequest {
	public:
		Tx64Request(XBeeAddress64 &addr64, uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId);
		Tx64Request(XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);
		Tx64Request();

		~Tx64Request() override = default;

		XBeeAddress64 &getAddress64();
		void setAddress64(XBeeAddress64 &addr64);
		uint8_t getOption();
		void setOption(uint8_t option);
		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;

	private:
		XBeeAddress64 _addr64;
		uint8_t _option;
	};

#endif


#ifdef SERIES_2
	class ZBTxRequest : public PayloadRequest {
	public:
		explicit ZBTxRequest(const XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);
		explicit ZBTxRequest(const XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius, uint8_t option,
		            uint8_t *payload, uint8_t payloadLength, uint8_t frameId);
		ZBTxRequest();
		~ZBTxRequest() override = default;

		XBeeAddress64 &getAddress64();
		uint16_t getAddress16();
		uint8_t getBroadcastRadius();
		uint8_t getOption();
		void setAddress64(const XBeeAddress64 &addr64);
		void setAddress16(uint16_t addr16);
		void setBroadcastRadius(uint8_t broadcastRadius);
		void setOption(uint8_t option);

	protected:
		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;

		XBeeAddress64 _addr64;
		uint16_t _addr16;
		uint8_t _broadcastRadius;
		uint8_t _option;
	};

	/*
	 * Represents a Series 2 TX packet that corresponds to Api Id: ZB_EXPLICIT_TX_REQUEST
	 *
	 * See the warning about maximum packet size for ZBTxRequest above,
	 * which probably also applies here as well.
	 *
	 * Note that to distinguish reply packets from non-XBee devices, set
	 * AO=1 to enable reception of ZBExplicitRxResponse packets.
	 */
	class ZBExplicitTxRequest : public ZBTxRequest {
	public:
		ZBExplicitTxRequest(XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);
		ZBExplicitTxRequest(XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius, uint8_t option,
		                    uint8_t *payload, uint8_t payloadLength, uint8_t frameId, uint8_t srcEndpoint,
		                    uint8_t dstEndpoint, uint16_t clusterId, uint16_t profileId);
		ZBExplicitTxRequest();
		~ZBExplicitTxRequest() override = default;

		uint8_t getSrcEndpoint();
		uint8_t getDstEndpoint();
		uint16_t getClusterId();
		uint16_t getProfileId();
		void setSrcEndpoint(uint8_t endpoint);
		void setDstEndpoint(uint8_t endpoint);
		void setClusterId(uint16_t clusterId);
		void setProfileId(uint16_t profileId);

	protected:
		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;

	private:
		uint8_t _srcEndpoint;
		uint8_t _dstEndpoint;
		uint16_t _profileId;
		uint16_t _clusterId;
	};

#endif

	/*
	 * Represents an AT Command TX packet
	 * The command is used to configure the serially connected XBee radio
	 */
	class AtCommandRequest : public XBeeRequest {
	public:
		AtCommandRequest();
		AtCommandRequest(uint8_t *command);
		AtCommandRequest(uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);
		~AtCommandRequest() override = default;

		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;
		uint8_t *getCommand();
		void setCommand(uint8_t *command);
		uint8_t *getCommandValue();
		void setCommandValue(uint8_t *command);
		uint8_t getCommandValueLength();
		void setCommandValueLength(uint8_t length);
		void clearCommandValue();

	private:
		uint8_t *_command;
		uint8_t *_commandValue;
		uint8_t _commandValueLength;
	};

	class RemoteAtCommandRequest : public AtCommandRequest {
	public:
		RemoteAtCommandRequest();
		RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);
		RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command);
		RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);
		RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command);
		~RemoteAtCommandRequest() override = default;

		uint16_t getRemoteAddress16();
		void setRemoteAddress16(uint16_t remoteAddress16);
		XBeeAddress64 &getRemoteAddress64();
		void setRemoteAddress64(XBeeAddress64 &remoteAddress64);
		bool getApplyChanges();
		void setApplyChanges(bool applyChanges);
		uint8_t getFrameData(uint8_t pos) override;
		uint8_t getFrameDataLength() override;

		static XBeeAddress64 broadcastAddress64;

	private:
		XBeeAddress64 _remoteAddress64;
		uint16_t _remoteAddress16;
		bool _applyChanges;
	};

}
