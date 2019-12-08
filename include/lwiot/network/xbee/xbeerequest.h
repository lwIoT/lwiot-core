/*
 * XBee response (RX) base class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file xbeerequest.h

#pragma once

#include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeeaddress.h>

namespace lwiot
{
	/**
	 * @brief XBee request base class.
	 * @ingroup xb
	 */
	class XBeeRequest {
	public:
		/**
		 * @brief Construct a new request.
		 * @param apiId API ID.
		 * @param frameId Frame ID.
		 */
		explicit XBeeRequest(uint8_t apiId, uint8_t frameId);

		/**
		 * @brief Destroy an XBee request.
		 */
		virtual ~XBeeRequest() = default;

		/**
		 * @brief Set the frame ID.
		 * @param frameId Frame ID to set.
		 */
		void setFrameId(uint8_t frameId);

		/**
		 * @brief Get the frame ID.
		 * @return Current frame ID.
		 */
		uint8_t getFrameId();

		/**
		 * @brief Get the current API ID.
		 * @return The current API ID.
		 */
		uint8_t getApiId();

		/**
		 * @brief Get frame data.
		 * @param pos Byte position.
		 * @return The data at position \p pos.
		 */
		virtual uint8_t getFrameData(uint8_t pos) = 0;

		/**
		 * @brief Get the frame length.
		 * @return The frame length.
		 */
		virtual uint8_t getFrameDataLength() = 0;

	protected:
		/**
		 * @brief Set the API ID.
		 * @param apiId API ID to set.
		 */
		void setApiId(uint8_t apiId);

	private:
		uint8_t _apiId;
		uint8_t _frameId;
	};

	/**
	 * @brief Payload request.
	 * @ingroup xb
	 */
	class PayloadRequest : public XBeeRequest {
	public:
		/**
		 * @brief Payload request.
		 * @param apiId API ID.
		 * @param frameId Frame ID.
		 * @param payload Payload.
		 * @param payloadLength Length of \p payload.
		 */
		PayloadRequest(uint8_t apiId, uint8_t frameId, uint8_t *payload, uint8_t payloadLength);

		/**
		 * @brief Destroy the a payload request.
		 */
		~PayloadRequest() override = default;

		uint8_t *getPayload(); //!< Get the payload.
		void setPayload(uint8_t *payloadPtr); //!< Set the payload.

		/**
		 * @brief Set the payload.
		 * @param payloadPtr Payload pointer.
		 * @param payloadLength Get the payload.
		 */
		void setPayload(uint8_t *payloadPtr, uint8_t payloadLength)
		{
			setPayload(payloadPtr);
			setPayloadLength(payloadLength);
		}

		uint8_t getPayloadLength(); //!< Get the payload length.
		void setPayloadLength(uint8_t payloadLength); //!< Set the payload length.

	private:
		uint8_t *_payloadPtr;
		uint8_t _payloadLength;
	};

#ifdef SERIES_1
	/*
	 * @brief Represents a Series 1 TX packet that corresponds to Api Id: TX_16_REQUEST
	 * @ingroup xb
	 *
	 * Be careful not to send a data array larger than the max packet size of your radio.
	 * This class does not perform any validation of packet size and there will be no indication
	 * if the packet is too large, other than you will not get a TX Status response.
	 * The datasheet says 100 bytes is the maximum, although that could change in future firmware.
	 */
	class Tx16Request : public PayloadRequest {
	public:
		Tx16Request(); //!< Construct a new TX request.

		/**
		 * @brief Construct a new TX request.
		 * @param addr16 Address.
		 * @param option Payload options.
		 * @param payload Payload data.
		 * @param payloadLength Length of \p payload.
		 * @param frameId Frame ID.
		 */
		Tx16Request(uint16_t addr16, uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId);

		/**
		 * @brief Construct a new TX request.
		 * @param addr16 Address.
		 * @param payload Payload data.
		 * @param payloadLength Length of \p payload.
		 */
		Tx16Request(uint16_t addr16, uint8_t *payload, uint8_t payloadLength);

		/**
		 * @brief Destroy at TX request.
		 */
		~Tx16Request() override = default;

		uint16_t getAddress16(); //!< Get the address.
		void setAddress16(uint16_t addr16); //!< Set the address.
		uint8_t getOption(); //!< Get options.
		void setOption(uint8_t option); //!< Set option flags.
		uint8_t getFrameData(uint8_t pos) override; //!< Get the frame data.
		uint8_t getFrameDataLength() override; //!< Set the frame data.

	private:
		uint16_t _addr16;
		uint8_t _option;
	};

	/**
	 * @brief Transmit request.
	 * @ingroup xb
	 */
	class Tx64Request : public PayloadRequest {
	public:
		/**
		 * @brief Construct a new TX request.
		 * @param addr64 Adress.
		 * @param option Option flags.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 * @param frameId Frame ID.
		 */
		Tx64Request(XBeeAddress64 &addr64, uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId);

		/**
		 * @brief Construct a new TX request.
		 * @param addr64 Adress.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 */
		Tx64Request(XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);

		/**
		 * @brief Construct a new TX request.
		 */
		Tx64Request();

		~Tx64Request() override = default; //!< Destroy a TX request.

		XBeeAddress64 &getAddress64(); //!< Get the address.
		void setAddress64(XBeeAddress64 &addr64); //!< Set the address.
		uint8_t getOption(); //!< Get the option flags.
		void setOption(uint8_t option); //!< Set the option flags.
		uint8_t getFrameData(uint8_t pos) override; //!< Get frame data.
		uint8_t getFrameDataLength() override; //!< Get frame data length.

	private:
		XBeeAddress64 _addr64;
		uint8_t _option;
	};

#endif


#ifdef SERIES_2
	/**
	 * @brief ZigBee TX request.
	 * @ingroup xb
	 */
	class ZBTxRequest : public PayloadRequest {
	public:
		/**
		 * @brief Construct a new TX request.
		 * @param addr64 Adress.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 */
		explicit ZBTxRequest(const XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);

		/**
		 * @brief Construct a new TX request.
		 * @param addr64 64-bit address.
		 * @param addr16 16-bit address.
		 * @param broadcastRadius Broadcast radius.
		 * @param option Option flags.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 * @param frameId Frame ID.
		 */
		explicit ZBTxRequest(const XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius, uint8_t option,
		            uint8_t *payload, uint8_t payloadLength, uint8_t frameId);

		/**
		 * @brief Construct a new TX request.
		 */
		ZBTxRequest();
		~ZBTxRequest() override = default; //!< Destroy a TX request.

		/**
		 * @brief Get the address data.
		 * @return XBee address data.
		 */
		XBeeAddress64 &getAddress64();

		/**
		 * @brief Get the address data.
		 * @return XBee address data.
		 */
		uint16_t getAddress16();
		uint8_t getBroadcastRadius(); //!< Get the broadcast radius.
		uint8_t getOption(); //!< Get the option flags.
		void setAddress64(const XBeeAddress64 &addr64); //!< Set the 64-bit address.
		void setAddress16(uint16_t addr16); //!< Set the 16-bit address.
		void setBroadcastRadius(uint8_t broadcastRadius); //!< Set the broadcast radius.
		void setOption(uint8_t option); //!< Set the option flags.

	protected:
		uint8_t getFrameData(uint8_t pos) override; //!< Set the frame data.
		uint8_t getFrameDataLength() override; //!< Set the frame data length.

		XBeeAddress64 _addr64; //!< 64-bit address.
		uint16_t _addr16; //!< 16-bit address.
		uint8_t _broadcastRadius; //!< Broadcast address.
		uint8_t _option; //!< Option flags.
	};

	/**
	 * @brief Represents a Series 2 TX packet that corresponds to API ID: ZB_EXPLICIT_TX_REQUEST
	 * @ingroup xb
	 *
	 * Note that to distinguish reply packets from non-XBee devices, set
	 * AO=1 to enable reception of ZBExplicitRxResponse packets.
	 */
	class ZBExplicitTxRequest : public ZBTxRequest {
	public:
		/**
		 * @brief Construct a new TX request.
		 * @param addr64 Adress.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 */
		ZBExplicitTxRequest(XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength);

		/**
		 * @brief Construct a new TX request.
		 * @param addr64 64-bit address.
		 * @param addr16 16-bit address.
		 * @param broadcastRadius Broadcast radius.
		 * @param option Option flags.
		 * @param payload Payload data.
		 * @param payloadLength Payload length.
		 * @param frameId Frame ID.
		 * @param srcEndpoint Source end point.
		 * @param dstEndpoint Destination end point.
		 * @param clusterId Cluster ID.
		 * @param profileId Profile ID.
		 */
		ZBExplicitTxRequest(XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius, uint8_t option,
		                    uint8_t *payload, uint8_t payloadLength, uint8_t frameId, uint8_t srcEndpoint,
		                    uint8_t dstEndpoint, uint16_t clusterId, uint16_t profileId);
		/**
		 * @brief Construct a new TX request.
		 */
		ZBExplicitTxRequest();
		~ZBExplicitTxRequest() override = default; //!< Destroy a TX request.

		uint8_t getSrcEndpoint(); //!< Get the source end point.
		uint8_t getDstEndpoint(); //!< Get the destination end point.
		uint16_t getClusterId(); //!< Get the cluster ID.
		uint16_t getProfileId(); //!< Get the profile ID.
		void setSrcEndpoint(uint8_t endpoint); //!< Set the source end point.
		void setDstEndpoint(uint8_t endpoint); //!< Set the destination end point.
		void setClusterId(uint16_t clusterId); //!< Set the cluster ID.
		void setProfileId(uint16_t profileId); //!< Set the profile ID.

	protected:
		/**
		 * @brief Get the frame data.
		 * @param pos Frame data position.
		 * @return The frame data at position \p pos.
		 */
		uint8_t getFrameData(uint8_t pos) override;

		/**
		 * @brief Get the frame data length.
		 * @return The current frame data length.
		 */
		uint8_t getFrameDataLength() override;

	private:
		uint8_t _srcEndpoint;
		uint8_t _dstEndpoint;
		uint16_t _profileId;
		uint16_t _clusterId;
	};

#endif

	/**
	 * @brief Represents an AT Command TX packet
	 * @ingroup xb
	 *
	 * The command is used to configure the serially connected XBee radio
	 */
	class AtCommandRequest : public XBeeRequest {
	public:
		AtCommandRequest(); //!< Construct a new AT command.
		AtCommandRequest(uint8_t *command); //!< Construct a new AT command.
		AtCommandRequest(uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength); //!< Construct a new AT command.
		~AtCommandRequest() override = default; //!< Destroy an AT command.

		/**
		 * @brief Get the frame data.
		 * @param pos Frame data position.
		 * @return The frame data at position \p pos.
		 */
		uint8_t getFrameData(uint8_t pos) override;

		/**
		 * @brief Get the frame data length.
		 * @return The current frame data length.
		 */
		uint8_t getFrameDataLength() override;

		uint8_t *getCommand(); //!< Get the AT command.
		void setCommand(uint8_t *command); //!< Set the AT command.
		uint8_t *getCommandValue(); //!< Get the command value.
		void setCommandValue(uint8_t *command); //!< Set the command value.
		uint8_t getCommandValueLength(); //!< Get the command value length.
		void setCommandValueLength(uint8_t length); //!< Set the command value length.
		void clearCommandValue(); //!< Clear the command  value length.

	private:
		uint8_t *_command;
		uint8_t *_commandValue;
		uint8_t _commandValueLength;
	};

	/**
	 * @brief Remote AT command wrapper.
	 * @ingroup xb
	 */
	class RemoteAtCommandRequest : public AtCommandRequest {
	public:
		RemoteAtCommandRequest();  //!< Construct a new AT command.

		/**
		 * @brief Construct a new AT command.
		 * @param remoteAddress16 Remote address.
		 * @param command Command.
		 * @param commandValue Command value/argument.
		 * @param commandValueLength Command value length.
		 */
		RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);

		/**
		 * @brief Construct a new AT command.
		 * @param remoteAddress16 Remote address.
		 * @param command Command.
		 */
		RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command);

		/**
		 * @brief Construct a new AT command.
		 * @param remoteAddress64 Remote address.
		 * @param command Command.
		 * @param commandValue Command value/argument.
		 * @param commandValueLength Command value length.
		 */
		RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);

		/**
		 * @brief Construct a new AT command.
		 * @param remoteAddress64 Remote address.
		 * @param command Command.
		 */
		RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command);
		~RemoteAtCommandRequest() override = default; //!< Remote command value.

		uint16_t getRemoteAddress16(); //!< Get the remote address.
		void setRemoteAddress16(uint16_t remoteAddress16); //!< Set the remote address.
		XBeeAddress64 &getRemoteAddress64();  //!< Get the remote address.
		void setRemoteAddress64(XBeeAddress64 &remoteAddress64);  //!< Set the remote address.
		bool getApplyChanges(); //!< Apply changes to the remote.
		void setApplyChanges(bool applyChanges); //!< Set the apply changes indicator.
		uint8_t getFrameData(uint8_t pos) override; //!< Get the frame data at \p pos.
		uint8_t getFrameDataLength() override; //!< Get the frame data length.

		static XBeeAddress64 broadcastAddress64; //!< Broadcast address.

	private:
		XBeeAddress64 _remoteAddress64;
		uint16_t _remoteAddress16;
		bool _applyChanges;
	};

}
