/*
 * XBee response.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeerequest.h>

namespace lwiot
{
	XBeeRequest::XBeeRequest(uint8_t apiId, uint8_t frameId)
	{
		_apiId = apiId;
		_frameId = frameId;
	}

	void XBeeRequest::setFrameId(uint8_t frameId)
	{
		_frameId = frameId;
	}

	uint8_t XBeeRequest::getFrameId()
	{
		return _frameId;
	}

	uint8_t XBeeRequest::getApiId()
	{
		return _apiId;
	}

	void XBeeRequest::setApiId(uint8_t apiId)
	{
		_apiId = apiId;
	}

	PayloadRequest::PayloadRequest(uint8_t apiId, uint8_t frameId, uint8_t *payload, uint8_t payloadLength)
			: XBeeRequest(apiId, frameId)
	{
		_payloadPtr = payload;
		_payloadLength = payloadLength;
	}

	uint8_t *PayloadRequest::getPayload()
	{
		return _payloadPtr;
	}

	void PayloadRequest::setPayload(uint8_t *payload)
	{
		_payloadPtr = payload;
	}

	uint8_t PayloadRequest::getPayloadLength()
	{
		return _payloadLength;
	}

	void PayloadRequest::setPayloadLength(uint8_t payloadLength)
	{
		_payloadLength = payloadLength;
	}

#ifdef SERIES_2

	ZBTxRequest::ZBTxRequest() : PayloadRequest(ZB_TX_REQUEST, DEFAULT_FRAME_ID, NULL, 0)
	{
		_addr16 = ZB_BROADCAST_ADDRESS;
		_broadcastRadius = ZB_BROADCAST_RADIUS_MAX_HOPS;
		_option = ZB_TX_UNICAST;
	}

	ZBTxRequest::ZBTxRequest(const XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius, uint8_t option,
	                         uint8_t *data, uint8_t dataLength, uint8_t frameId) : PayloadRequest(ZB_TX_REQUEST,
	                                                                                              frameId, data,
	                                                                                              dataLength)
	{
		_addr64 = addr64;
		_addr16 = addr16;
		_broadcastRadius = broadcastRadius;
		_option = option;
	}

	ZBTxRequest::ZBTxRequest(const XBeeAddress64 &addr64, uint8_t *data, uint8_t dataLength) : PayloadRequest(
			ZB_TX_REQUEST, DEFAULT_FRAME_ID, data, dataLength)
	{
		_addr64 = addr64;
		_addr16 = ZB_BROADCAST_ADDRESS;
		_broadcastRadius = ZB_BROADCAST_RADIUS_MAX_HOPS;
		_option = ZB_TX_UNICAST;
	}

	uint8_t ZBTxRequest::getFrameData(uint8_t pos)
	{
		if(pos == 0) {
			return (_addr64.getMsb() >> 24) & 0xff;
		} else if(pos == 1) {
			return (_addr64.getMsb() >> 16) & 0xff;
		} else if(pos == 2) {
			return (_addr64.getMsb() >> 8) & 0xff;
		} else if(pos == 3) {
			return _addr64.getMsb() & 0xff;
		} else if(pos == 4) {
			return (_addr64.getLsb() >> 24) & 0xff;
		} else if(pos == 5) {
			return (_addr64.getLsb() >> 16) & 0xff;
		} else if(pos == 6) {
			return (_addr64.getLsb() >> 8) & 0xff;
		} else if(pos == 7) {
			return _addr64.getLsb() & 0xff;
		} else if(pos == 8) {
			return (_addr16 >> 8) & 0xff;
		} else if(pos == 9) {
			return _addr16 & 0xff;
		} else if(pos == 10) {
			return _broadcastRadius;
		} else if(pos == 11) {
			return _option;
		} else {
			return getPayload()[pos - ZB_TX_API_LENGTH];
		}
	}

	uint8_t ZBTxRequest::getFrameDataLength()
	{
		return ZB_TX_API_LENGTH + getPayloadLength();
	}

	XBeeAddress64 &ZBTxRequest::getAddress64()
	{
		return _addr64;
	}

	uint16_t ZBTxRequest::getAddress16()
	{
		return _addr16;
	}

	uint8_t ZBTxRequest::getBroadcastRadius()
	{
		return _broadcastRadius;
	}

	uint8_t ZBTxRequest::getOption()
	{
		return _option;
	}

	void ZBTxRequest::setAddress64(const XBeeAddress64 &addr64)
	{
		_addr64 = addr64;
	}

	void ZBTxRequest::setAddress16(uint16_t addr16)
	{
		_addr16 = addr16;
	}

	void ZBTxRequest::setBroadcastRadius(uint8_t broadcastRadius)
	{
		_broadcastRadius = broadcastRadius;
	}

	void ZBTxRequest::setOption(uint8_t option)
	{
		_option = option;
	}


	ZBExplicitTxRequest::ZBExplicitTxRequest() : ZBTxRequest()
	{
		_srcEndpoint = DEFAULT_ENDPOINT;
		_dstEndpoint = DEFAULT_ENDPOINT;
		_profileId = DEFAULT_PROFILE_ID;
		_clusterId = DEFAULT_CLUSTER_ID;
		setApiId(ZB_EXPLICIT_TX_REQUEST);
	}

	ZBExplicitTxRequest::ZBExplicitTxRequest(XBeeAddress64 &addr64, uint16_t addr16, uint8_t broadcastRadius,
	                                         uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId,
	                                         uint8_t srcEndpoint, uint8_t dstEndpoint, uint16_t clusterId,
	                                         uint16_t profileId)
			: ZBTxRequest(addr64, addr16, broadcastRadius, option, payload, payloadLength, frameId)
	{
		_srcEndpoint = srcEndpoint;
		_dstEndpoint = dstEndpoint;
		_profileId = profileId;
		_clusterId = clusterId;
		setApiId(ZB_EXPLICIT_TX_REQUEST);
	}

	ZBExplicitTxRequest::ZBExplicitTxRequest(XBeeAddress64 &addr64, uint8_t *payload, uint8_t payloadLength)
			: ZBTxRequest(addr64, payload, payloadLength)
	{
		_srcEndpoint = DEFAULT_ENDPOINT;
		_dstEndpoint = DEFAULT_ENDPOINT;
		_profileId = DEFAULT_PROFILE_ID;
		_clusterId = DEFAULT_CLUSTER_ID;
		setApiId(ZB_EXPLICIT_TX_REQUEST);
	}

	uint8_t ZBExplicitTxRequest::getFrameData(uint8_t pos)
	{
		if(pos < 10) {
			return ZBTxRequest::getFrameData(pos);
		} else if(pos == 10) {
			return _srcEndpoint;
		} else if(pos == 11) {
			return _dstEndpoint;
		} else if(pos == 12) {
			return (_clusterId >> 8) & 0xff;
		} else if(pos == 13) {
			return _clusterId & 0xff;
		} else if(pos == 14) {
			return (_profileId >> 8) & 0xff;
		} else if(pos == 15) {
			return _profileId & 0xff;
		} else if(pos == 16) {
			return _broadcastRadius;
		} else if(pos == 17) {
			return _option;
		} else {
			return getPayload()[pos - ZB_EXPLICIT_TX_API_LENGTH];
		}
	}

	uint8_t ZBExplicitTxRequest::getFrameDataLength()
	{
		return ZB_EXPLICIT_TX_API_LENGTH + getPayloadLength();
	}

	uint8_t ZBExplicitTxRequest::getSrcEndpoint()
	{
		return _srcEndpoint;
	}

	uint8_t ZBExplicitTxRequest::getDstEndpoint()
	{
		return _dstEndpoint;
	}

	uint16_t ZBExplicitTxRequest::getClusterId()
	{
		return _clusterId;
	}

	uint16_t ZBExplicitTxRequest::getProfileId()
	{
		return _profileId;
	}

	void ZBExplicitTxRequest::setSrcEndpoint(uint8_t endpoint)
	{
		_srcEndpoint = endpoint;
	}

	void ZBExplicitTxRequest::setDstEndpoint(uint8_t endpoint)
	{
		_dstEndpoint = endpoint;
	}

	void ZBExplicitTxRequest::setClusterId(uint16_t clusterId)
	{
		_clusterId = clusterId;
	}

	void ZBExplicitTxRequest::setProfileId(uint16_t profileId)
	{
		_profileId = profileId;
	}

#endif

#ifdef SERIES_1

	Tx16Request::Tx16Request() : PayloadRequest(TX_16_REQUEST, DEFAULT_FRAME_ID, NULL, 0)
	{
		_option = ACK_OPTION;
	}

	Tx16Request::Tx16Request(uint16_t addr16, uint8_t option, uint8_t *data, uint8_t dataLength, uint8_t frameId)
			: PayloadRequest(TX_16_REQUEST, frameId, data, dataLength)
	{
		_addr16 = addr16;
		_option = option;
	}

	Tx16Request::Tx16Request(uint16_t addr16, uint8_t *data, uint8_t dataLength) : PayloadRequest(TX_16_REQUEST,
	                                                                                              DEFAULT_FRAME_ID,
	                                                                                              data, dataLength)
	{
		_addr16 = addr16;
		_option = ACK_OPTION;
	}

	uint8_t Tx16Request::getFrameData(uint8_t pos)
	{

		if(pos == 0) {
			return (_addr16 >> 8) & 0xff;
		} else if(pos == 1) {
			return _addr16 & 0xff;
		} else if(pos == 2) {
			return _option;
		} else {
			return getPayload()[pos - TX_16_API_LENGTH];
		}
	}

	uint8_t Tx16Request::getFrameDataLength()
	{
		return TX_16_API_LENGTH + getPayloadLength();
	}

	uint16_t Tx16Request::getAddress16()
	{
		return _addr16;
	}

	void Tx16Request::setAddress16(uint16_t addr16)
	{
		_addr16 = addr16;
	}

	uint8_t Tx16Request::getOption()
	{
		return _option;
	}

	void Tx16Request::setOption(uint8_t option)
	{
		_option = option;
	}

	Tx64Request::Tx64Request() : PayloadRequest(TX_64_REQUEST, DEFAULT_FRAME_ID, NULL, 0)
	{
		_option = ACK_OPTION;
	}

	Tx64Request::Tx64Request(XBeeAddress64 &addr64, uint8_t option, uint8_t *data, uint8_t dataLength, uint8_t frameId)
			: PayloadRequest(TX_64_REQUEST, frameId, data, dataLength)
	{
		_addr64 = addr64;
		_option = option;
	}

	Tx64Request::Tx64Request(XBeeAddress64 &addr64, uint8_t *data, uint8_t dataLength) : PayloadRequest(TX_64_REQUEST,
	                                                                                                    DEFAULT_FRAME_ID,
	                                                                                                    data,
	                                                                                                    dataLength)
	{
		_addr64 = addr64;
		_option = ACK_OPTION;
	}

	uint8_t Tx64Request::getFrameData(uint8_t pos)
	{

		if(pos == 0) {
			return (_addr64.getMsb() >> 24) & 0xff;
		} else if(pos == 1) {
			return (_addr64.getMsb() >> 16) & 0xff;
		} else if(pos == 2) {
			return (_addr64.getMsb() >> 8) & 0xff;
		} else if(pos == 3) {
			return _addr64.getMsb() & 0xff;
		} else if(pos == 4) {
			return (_addr64.getLsb() >> 24) & 0xff;
		} else if(pos == 5) {
			return (_addr64.getLsb() >> 16) & 0xff;
		} else if(pos == 6) {
			return (_addr64.getLsb() >> 8) & 0xff;
		} else if(pos == 7) {
			return _addr64.getLsb() & 0xff;
		} else if(pos == 8) {
			return _option;
		} else {
			return getPayload()[pos - TX_64_API_LENGTH];
		}
	}

	uint8_t Tx64Request::getFrameDataLength()
	{
		return TX_64_API_LENGTH + getPayloadLength();
	}

	XBeeAddress64 &Tx64Request::getAddress64()
	{
		return _addr64;
	}

	void Tx64Request::setAddress64(XBeeAddress64 &addr64)
	{
		_addr64 = addr64;
	}

	uint8_t Tx64Request::getOption()
	{
		return _option;
	}

	void Tx64Request::setOption(uint8_t option)
	{
		_option = option;
	}

#endif

	AtCommandRequest::AtCommandRequest() : XBeeRequest(AT_COMMAND_REQUEST, DEFAULT_FRAME_ID)
	{
		_command = NULL;
		clearCommandValue();
	}

	AtCommandRequest::AtCommandRequest(uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength)
			: XBeeRequest(AT_COMMAND_REQUEST, DEFAULT_FRAME_ID)
	{
		_command = command;
		_commandValue = commandValue;
		_commandValueLength = commandValueLength;
	}

	AtCommandRequest::AtCommandRequest(uint8_t *command) : XBeeRequest(AT_COMMAND_REQUEST, DEFAULT_FRAME_ID)
	{
		_command = command;
		clearCommandValue();
	}

	uint8_t *AtCommandRequest::getCommand()
	{
		return _command;
	}

	uint8_t *AtCommandRequest::getCommandValue()
	{
		return _commandValue;
	}

	uint8_t AtCommandRequest::getCommandValueLength()
	{
		return _commandValueLength;
	}

	void AtCommandRequest::setCommand(uint8_t *command)
	{
		_command = command;
	}

	void AtCommandRequest::setCommandValue(uint8_t *value)
	{
		_commandValue = value;
	}

	void AtCommandRequest::setCommandValueLength(uint8_t length)
	{
		_commandValueLength = length;
	}

	uint8_t AtCommandRequest::getFrameData(uint8_t pos)
	{

		if(pos == 0) {
			return _command[0];
		} else if(pos == 1) {
			return _command[1];
		} else {
			return _commandValue[pos - AT_COMMAND_API_LENGTH];
		}
	}

	void AtCommandRequest::clearCommandValue()
	{
		_commandValue = NULL;
		_commandValueLength = 0;
	}

//void AtCommandRequest::reset() {
//	 XBeeRequest::reset();
//}

	uint8_t AtCommandRequest::getFrameDataLength()
	{
		// command is 2 byte + length of value
		return AT_COMMAND_API_LENGTH + _commandValueLength;
	}

	XBeeAddress64 RemoteAtCommandRequest::broadcastAddress64 = XBeeAddress64(0x0, BROADCAST_ADDRESS);

	RemoteAtCommandRequest::RemoteAtCommandRequest() : AtCommandRequest(NULL, NULL, 0)
	{
		_remoteAddress16 = 0;
		_applyChanges = false;
		setApiId(REMOTE_AT_REQUEST);
	}

	RemoteAtCommandRequest::RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command, uint8_t *commandValue,
	                                               uint8_t commandValueLength) : AtCommandRequest(command, commandValue,
	                                                                                              commandValueLength)
	{
		_remoteAddress64 = broadcastAddress64;
		_remoteAddress16 = remoteAddress16;
		_applyChanges = true;
		setApiId(REMOTE_AT_REQUEST);
	}

	RemoteAtCommandRequest::RemoteAtCommandRequest(uint16_t remoteAddress16, uint8_t *command) : AtCommandRequest(
			command, NULL, 0)
	{
		_remoteAddress64 = broadcastAddress64;
		_remoteAddress16 = remoteAddress16;
		_applyChanges = false;
		setApiId(REMOTE_AT_REQUEST);
	}

	RemoteAtCommandRequest::RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command,
	                                               uint8_t *commandValue, uint8_t commandValueLength)
			: AtCommandRequest(command, commandValue, commandValueLength)
	{
		_remoteAddress64 = remoteAddress64;
		// don't worry.. works for series 1 too!
		_remoteAddress16 = ZB_BROADCAST_ADDRESS;
		_applyChanges = true;
		setApiId(REMOTE_AT_REQUEST);
	}

	RemoteAtCommandRequest::RemoteAtCommandRequest(XBeeAddress64 &remoteAddress64, uint8_t *command) : AtCommandRequest(
			command, NULL, 0)
	{
		_remoteAddress64 = remoteAddress64;
		_remoteAddress16 = ZB_BROADCAST_ADDRESS;
		_applyChanges = false;
		setApiId(REMOTE_AT_REQUEST);
	}

	uint16_t RemoteAtCommandRequest::getRemoteAddress16()
	{
		return _remoteAddress16;
	}

	void RemoteAtCommandRequest::setRemoteAddress16(uint16_t remoteAddress16)
	{
		_remoteAddress16 = remoteAddress16;
	}

	XBeeAddress64 &RemoteAtCommandRequest::getRemoteAddress64()
	{
		return _remoteAddress64;
	}

	void RemoteAtCommandRequest::setRemoteAddress64(XBeeAddress64 &remoteAddress64)
	{
		_remoteAddress64 = remoteAddress64;
	}

	bool RemoteAtCommandRequest::getApplyChanges()
	{
		return _applyChanges;
	}

	void RemoteAtCommandRequest::setApplyChanges(bool applyChanges)
	{
		_applyChanges = applyChanges;
	}


	uint8_t RemoteAtCommandRequest::getFrameData(uint8_t pos)
	{
		if(pos == 0) {
			return (_remoteAddress64.getMsb() >> 24) & 0xff;
		} else if(pos == 1) {
			return (_remoteAddress64.getMsb() >> 16) & 0xff;
		} else if(pos == 2) {
			return (_remoteAddress64.getMsb() >> 8) & 0xff;
		} else if(pos == 3) {
			return _remoteAddress64.getMsb() & 0xff;
		} else if(pos == 4) {
			return (_remoteAddress64.getLsb() >> 24) & 0xff;
		} else if(pos == 5) {
			return (_remoteAddress64.getLsb() >> 16) & 0xff;
		} else if(pos == 6) {
			return (_remoteAddress64.getLsb() >> 8) & 0xff;
		} else if(pos == 7) {
			return _remoteAddress64.getLsb() & 0xff;
		} else if(pos == 8) {
			return (_remoteAddress16 >> 8) & 0xff;
		} else if(pos == 9) {
			return _remoteAddress16 & 0xff;
		} else if(pos == 10) {
			return _applyChanges ? 2 : 0;
		} else if(pos == 11) {
			return getCommand()[0];
		} else if(pos == 12) {
			return getCommand()[1];
		} else {
			return getCommandValue()[pos - REMOTE_AT_COMMAND_API_LENGTH];
		}
	}

	uint8_t RemoteAtCommandRequest::getFrameDataLength()
	{
		return REMOTE_AT_COMMAND_API_LENGTH + getCommandValueLength();
	}
}
