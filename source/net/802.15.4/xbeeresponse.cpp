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

#include <lwiot/network/xbee/xbeeresponse.h>

namespace lwiot
{
	XBeeResponse::XBeeResponse()
	{
	}

	uint8_t XBeeResponse::getApiId()
	{
		return _apiId;
	}

	void XBeeResponse::setApiId(uint8_t apiId)
	{
		_apiId = apiId;
	}

	uint8_t XBeeResponse::getMsbLength()
	{
		return _msbLength;
	}

	void XBeeResponse::setMsbLength(uint8_t msbLength)
	{
		_msbLength = msbLength;
	}

	uint8_t XBeeResponse::getLsbLength()
	{
		return _lsbLength;
	}

	void XBeeResponse::setLsbLength(uint8_t lsbLength)
	{
		_lsbLength = lsbLength;
	}

	uint8_t XBeeResponse::getChecksum()
	{
		return _checksum;
	}

	void XBeeResponse::setChecksum(uint8_t checksum)
	{
		_checksum = checksum;
	}

	uint8_t XBeeResponse::getFrameDataLength()
	{
		return _frameLength;
	}

	void XBeeResponse::setFrameLength(uint8_t frameLength)
	{
		_frameLength = frameLength;
	}

	bool XBeeResponse::isAvailable()
	{
		return _complete;
	}

	void XBeeResponse::setAvailable(bool complete)
	{
		_complete = complete;
	}

	bool XBeeResponse::isError()
	{
		return _errorCode > 0;
	}

	uint8_t XBeeResponse::getErrorCode()
	{
		return _errorCode;
	}

	void XBeeResponse::setErrorCode(uint8_t errorCode)
	{
		_errorCode = errorCode;
	}

	void XBeeResponse::setCommon(XBeeResponse &target)
	{
		target.setApiId(getApiId());
		target.setAvailable(isAvailable());
		target.setChecksum(getChecksum());
		target.setErrorCode(getErrorCode());
		target.setFrameLength(getFrameDataLength());
		target.setMsbLength(getMsbLength());
		target.setLsbLength(getLsbLength());
	}

	void XBeeResponse::getModemStatusResponse(XBeeResponse &modemStatusResponse)
	{
		ModemStatusResponse *modem = static_cast<ModemStatusResponse *>(&modemStatusResponse);

		// pass pointer array to subclass
		modem->setFrameData(getFrameData());
		setCommon(modemStatusResponse);

	}

	void XBeeResponse::getAtCommandResponse(XBeeResponse &atCommandResponse)
	{

		AtCommandResponse *at = static_cast<AtCommandResponse *>(&atCommandResponse);

		// pass pointer array to subclass
		at->setFrameData(getFrameData());
		setCommon(atCommandResponse);
	}

	uint16_t XBeeResponse::getPacketLength()
	{
		return ((_msbLength << 8) & 0xff) + (_lsbLength & 0xff);
	}

	uint8_t *XBeeResponse::getFrameData()
	{
		return _frameDataPtr;
	}

	void XBeeResponse::setFrameData(uint8_t *frameDataPtr)
	{
		_frameDataPtr = frameDataPtr;
	}

	void XBeeResponse::init()
	{
		_complete = false;
		_errorCode = NO_ERROR;
		_checksum = 0;
	}

	void XBeeResponse::reset()
	{
		init();
		_apiId = 0;
		_msbLength = 0;
		_lsbLength = 0;
		_checksum = 0;
		_frameLength = 0;

		_errorCode = NO_ERROR;
	}

	void XBeeResponse::getRemoteAtCommandResponse(XBeeResponse &response)
	{
		RemoteAtCommandResponse *at = static_cast<RemoteAtCommandResponse *>(&response);

		at->setFrameData(getFrameData());
		setCommon(response);

		at->getRemoteAddress64().setMsb((uint32_t(getFrameData()[1]) << 24) + (uint32_t(getFrameData()[2]) << 16) +
		                                (uint16_t(getFrameData()[3]) << 8) + getFrameData()[4]);
		at->getRemoteAddress64().setLsb((uint32_t(getFrameData()[5]) << 24) + (uint32_t(getFrameData()[6]) << 16) +
		                                (uint16_t(getFrameData()[7]) << 8) + (getFrameData()[8]));

	}

	void XBeeResponse::getZBTxStatusResponse(XBeeResponse &zbXBeeResponse)
	{
		ZBTxStatusResponse *zb = static_cast<ZBTxStatusResponse *>(&zbXBeeResponse);

		zb->setFrameData(getFrameData());
		setCommon(zbXBeeResponse);
	}

	void XBeeResponse::getZBRxIoSampleResponse(XBeeResponse &response)
	{
		ZBRxIoSampleResponse *zb = static_cast<ZBRxIoSampleResponse *>(&response);

		zb->setFrameData(getFrameData());
		setCommon(response);

		zb->getRemoteAddress64().setMsb((uint32_t(getFrameData()[0]) << 24) + (uint32_t(getFrameData()[1]) << 16) +
		                                (uint16_t(getFrameData()[2]) << 8) + getFrameData()[3]);
		zb->getRemoteAddress64().setLsb((uint32_t(getFrameData()[4]) << 24) + (uint32_t(getFrameData()[5]) << 16) +
		                                (uint16_t(getFrameData()[6]) << 8) + (getFrameData()[7]));
	}

	void XBeeResponse::getZBRxResponse(XBeeResponse &rxResponse)
	{

		ZBRxResponse *zb = static_cast<ZBRxResponse *>(&rxResponse);

		//TODO verify response api id matches this api for this response

		// pass pointer array to subclass
		zb->setFrameData(getFrameData());
		setCommon(rxResponse);

		zb->getRemoteAddress64().setMsb((uint32_t(getFrameData()[0]) << 24) + (uint32_t(getFrameData()[1]) << 16) +
		                                (uint16_t(getFrameData()[2]) << 8) + getFrameData()[3]);
		zb->getRemoteAddress64().setLsb((uint32_t(getFrameData()[4]) << 24) + (uint32_t(getFrameData()[5]) << 16) +
		                                (uint16_t(getFrameData()[6]) << 8) + (getFrameData()[7]));
	}

	void XBeeResponse::getZBExplicitRxResponse(XBeeResponse &rxResponse)
	{
		getZBRxResponse(rxResponse);
	}

	void XBeeResponse::getTxStatusResponse(XBeeResponse &txResponse)
	{

		TxStatusResponse *txStatus = static_cast<TxStatusResponse *>(&txResponse);

		txStatus->setFrameData(getFrameData());
		setCommon(txResponse);
	}

	void XBeeResponse::getRx16Response(XBeeResponse &rx16Response)
	{

		Rx16Response *rx16 = static_cast<Rx16Response *>(&rx16Response);

		rx16->setFrameData(getFrameData());
		setCommon(rx16Response);
	}

	void XBeeResponse::getRx64Response(XBeeResponse &rx64Response)
	{

		Rx64Response *rx64 = static_cast<Rx64Response *>(&rx64Response);

		rx64->setFrameData(getFrameData());
		setCommon(rx64Response);

		rx64->getRemoteAddress64().setMsb((uint32_t(getFrameData()[0]) << 24) + (uint32_t(getFrameData()[1]) << 16) +
		                                  (uint16_t(getFrameData()[2]) << 8) + getFrameData()[3]);
		rx64->getRemoteAddress64().setLsb((uint32_t(getFrameData()[4]) << 24) + (uint32_t(getFrameData()[5]) << 16) +
		                                  (uint16_t(getFrameData()[6]) << 8) + getFrameData()[7]);
	}

	void XBeeResponse::getRx16IoSampleResponse(XBeeResponse &response)
	{
		Rx16IoSampleResponse *rx = static_cast<Rx16IoSampleResponse *>(&response);

		rx->setFrameData(getFrameData());
		setCommon(response);
	}

	void XBeeResponse::getRx64IoSampleResponse(XBeeResponse &response)
	{
		Rx64IoSampleResponse *rx = static_cast<Rx64IoSampleResponse *>(&response);

		rx->setFrameData(getFrameData());
		setCommon(response);

		rx->getRemoteAddress64().setMsb((uint32_t(getFrameData()[0]) << 24) + (uint32_t(getFrameData()[1]) << 16) +
		                                (uint16_t(getFrameData()[2]) << 8) + getFrameData()[3]);
		rx->getRemoteAddress64().setLsb((uint32_t(getFrameData()[4]) << 24) + (uint32_t(getFrameData()[5]) << 16) +
		                                (uint16_t(getFrameData()[6]) << 8) + getFrameData()[7]);
	}

	/* AT response */
	AtCommandResponse::AtCommandResponse()
	{

	}

	uint8_t *AtCommandResponse::getCommand()
	{
		return getFrameData() + 1;
	}

	uint8_t AtCommandResponse::getStatus()
	{
		return getFrameData()[3];
	}

	uint8_t AtCommandResponse::getValueLength()
	{
		return getFrameDataLength() - 4;
	}

	uint8_t *AtCommandResponse::getValue()
	{
		if(getValueLength() > 0) {
			// value is only included for query commands.  set commands does not return a value
			return getFrameData() + 4;
		}

		return NULL;
	}

	bool AtCommandResponse::isOk()
	{
		return getStatus() == AT_OK;
	}

	RemoteAtCommandResponse::RemoteAtCommandResponse() : AtCommandResponse()
	{

	}

	uint8_t *RemoteAtCommandResponse::getCommand()
	{
		return getFrameData() + 11;
	}

	uint8_t RemoteAtCommandResponse::getStatus()
	{
		return getFrameData()[13];
	}

	bool RemoteAtCommandResponse::isOk()
	{
		return getStatus() == AT_OK;
	}

	uint8_t RemoteAtCommandResponse::getValueLength()
	{
		return getFrameDataLength() - 14;
	}

	uint8_t *RemoteAtCommandResponse::getValue()
	{
		if(getValueLength() > 0) {
			return getFrameData() + 14;
		}

		return NULL;
	}

	uint16_t RemoteAtCommandResponse::getRemoteAddress16()
	{
		return uint16_t((getFrameData()[9] << 8) + getFrameData()[10]);
	}

	XBeeAddress64 &RemoteAtCommandResponse::getRemoteAddress64()
	{
		return _remoteAddress64;
	}

#ifdef SERIES_2

	ZBTxStatusResponse::ZBTxStatusResponse() : FrameIdResponse()
	{

	}

	uint16_t ZBTxStatusResponse::getRemoteAddress()
	{
		return (getFrameData()[1] << 8) + getFrameData()[2];
	}

	uint8_t ZBTxStatusResponse::getTxRetryCount()
	{
		return getFrameData()[3];
	}

	uint8_t ZBTxStatusResponse::getDeliveryStatus()
	{
		return getFrameData()[4];
	}

	uint8_t ZBTxStatusResponse::getDiscoveryStatus()
	{
		return getFrameData()[5];
	}

	bool ZBTxStatusResponse::isSuccess()
	{
		return getDeliveryStatus() == SUCCESS;
	}

	ZBRxResponse::ZBRxResponse() : RxDataResponse()
	{
		_remoteAddress64 = XBeeAddress64();
	}

	uint16_t ZBRxResponse::getRemoteAddress16()
	{
		return (getFrameData()[8] << 8) + getFrameData()[9];
	}

	uint8_t ZBRxResponse::getOption()
	{
		return getFrameData()[10];
	}

// markers to read data from packet array.  this is the index, so the 12th item in the array
	uint8_t ZBRxResponse::getDataOffset()
	{
		return 11;
	}

	uint8_t ZBRxResponse::getDataLength()
	{
		return getPacketLength() - getDataOffset() - 1;
	}

	XBeeAddress64 &ZBRxResponse::getRemoteAddress64()
	{
		return _remoteAddress64;
	}

	ZBExplicitRxResponse::ZBExplicitRxResponse() : ZBRxResponse()
	{
	}

	uint8_t ZBExplicitRxResponse::getSrcEndpoint()
	{
		return getFrameData()[10];
	}

	uint8_t ZBExplicitRxResponse::getDstEndpoint()
	{
		return getFrameData()[11];
	}

	uint16_t ZBExplicitRxResponse::getClusterId()
	{
		return (uint16_t) (getFrameData()[12]) << 8 | getFrameData()[13];
	}

	uint16_t ZBExplicitRxResponse::getProfileId()
	{
		return (uint16_t) (getFrameData()[14]) << 8 | getFrameData()[15];
	}

	uint8_t ZBExplicitRxResponse::getOption()
	{
		auto data = this->getFrameData();
		return data[16];
	}

// markers to read data from packet array.
	uint8_t ZBExplicitRxResponse::getDataOffset()
	{
		return 17;
	}

	uint8_t ZBExplicitRxResponse::getDataLength()
	{
		return getPacketLength() - getDataOffset() - 1;
	}

	ZBRxIoSampleResponse::ZBRxIoSampleResponse() : ZBRxResponse()
	{

	}

	uint8_t ZBRxIoSampleResponse::getDigitalMaskMsb()
	{
		return getFrameData()[12] & 0x1c;
	}

	uint8_t ZBRxIoSampleResponse::getDigitalMaskLsb()
	{
		return getFrameData()[13];
	}

	uint8_t ZBRxIoSampleResponse::getAnalogMask()
	{
		return getFrameData()[14] & 0x8f;
	}

	bool ZBRxIoSampleResponse::containsAnalog()
	{
		return getAnalogMask() > 0;
	}

	bool ZBRxIoSampleResponse::containsDigital()
	{
		return getDigitalMaskMsb() > 0 || getDigitalMaskLsb() > 0;
	}

	bool ZBRxIoSampleResponse::isAnalogEnabled(uint8_t pin)
	{
		return ((getAnalogMask() >> pin) & 1) == 1;
	}

	bool ZBRxIoSampleResponse::isDigitalEnabled(uint8_t pin)
	{
		if(pin <= 7) {
			return ((getDigitalMaskLsb() >> pin) & 1) == 1;
		} else {
			return ((getDigitalMaskMsb() >> (pin - 8)) & 1) == 1;
		}
	}

	uint16_t ZBRxIoSampleResponse::getAnalog(uint8_t pin)
	{
		uint8_t start = 15;

		if(containsDigital()) {
			start += 2;
		}

		// start depends on how many pins before this pin are enabled
		for(int i = 0; i < pin; i++) {
			if(isAnalogEnabled(i)) {
				start += 2;
			}
		}

		return (uint16_t) ((getFrameData()[start] << 8) + getFrameData()[start + 1]);
	}

	bool ZBRxIoSampleResponse::isDigitalOn(uint8_t pin)
	{
		if(pin <= 7) {
			// D0-7
			// DIO LSB is index 5
			return ((getFrameData()[16] >> pin) & 1) == 1;
		} else {
			// D10-12
			// DIO MSB is index 4
			return ((getFrameData()[15] >> (pin - 8)) & 1) == 1;
		}
	}

#endif

#ifdef SERIES_1

	RxResponse::RxResponse() : RxDataResponse()
	{

	}

	uint16_t Rx16Response::getRemoteAddress16()
	{
		return (getFrameData()[0] << 8) + getFrameData()[1];
	}

	XBeeAddress64 &Rx64Response::getRemoteAddress64()
	{
		return _remoteAddress;
	}

	Rx64Response::Rx64Response() : RxResponse()
	{
		_remoteAddress = XBeeAddress64();
	}

	Rx16Response::Rx16Response() : RxResponse()
	{

	}

	RxIoSampleBaseResponse::RxIoSampleBaseResponse() : RxResponse()
	{

	}

	uint8_t RxIoSampleBaseResponse::getSampleOffset()
	{
		// sample starts 2 bytes after rssi
		return getRssiOffset() + 2;
	}


	uint8_t RxIoSampleBaseResponse::getSampleSize()
	{
		return getFrameData()[getSampleOffset()];
	}

	bool RxIoSampleBaseResponse::containsAnalog()
	{
		return (getFrameData()[getSampleOffset() + 1] & 0x7e) > 0;
	}

	bool RxIoSampleBaseResponse::containsDigital()
	{
		return (getFrameData()[getSampleOffset() + 1] & 0x1) > 0 || getFrameData()[getSampleOffset() + 2] > 0;
	}

//uint16_t RxIoSampleBaseResponse::getAnalog0(uint8_t sample) {
//	return getAnalog(0, sample);
//}

	bool RxIoSampleBaseResponse::isAnalogEnabled(uint8_t pin)
	{
		return (((getFrameData()[getSampleOffset() + 1] >> (pin + 1)) & 1) == 1);
	}

	bool RxIoSampleBaseResponse::isDigitalEnabled(uint8_t pin)
	{
		if(pin < 8) {
			return ((getFrameData()[getSampleOffset() + 2] >> pin) & 1) == 1;
		} else {
			return (getFrameData()[getSampleOffset() + 1] & 1) == 1;
		}
	}

	uint8_t RxIoSampleBaseResponse::getSampleStart(uint8_t sample)
	{
		uint8_t spacing = 0;

		if(containsDigital()) {
			spacing += 2;
		}

		for(int i = 0; i <= 5; i++) {
			if(isAnalogEnabled(i)) {
				spacing += 2;
			}
		}

		return getSampleOffset() + 3 + sample * spacing;
	}

	uint16_t RxIoSampleBaseResponse::getAnalog(uint8_t pin, uint8_t sample)
	{
		uint8_t start = getSampleStart(sample);

		if(containsDigital()) {
			start += 2;
		}

		for(int i = 0; i < pin; i++) {
			if(isAnalogEnabled(i)) {
				start += 2;
			}
		}

		return (uint16_t) ((getFrameData()[start] << 8) + getFrameData()[start + 1]);
	}

	bool RxIoSampleBaseResponse::isDigitalOn(uint8_t pin, uint8_t sample)
	{
		if(pin < 8) {
			return ((getFrameData()[getSampleStart(sample) + 1] >> pin) & 1) == 1;
		} else {
			return (getFrameData()[getSampleStart(sample)] & 1) == 1;
		}
	}

	Rx16IoSampleResponse::Rx16IoSampleResponse() : RxIoSampleBaseResponse()
	{

	}

	uint16_t Rx16IoSampleResponse::getRemoteAddress16()
	{
		return (uint16_t) ((getFrameData()[0] << 8) + getFrameData()[1]);
	}

	uint8_t Rx16IoSampleResponse::getRssiOffset()
	{
		return 2;
	}

	Rx64IoSampleResponse::Rx64IoSampleResponse() : RxIoSampleBaseResponse()
	{
		_remoteAddress = XBeeAddress64();
	}

	XBeeAddress64 &Rx64IoSampleResponse::getRemoteAddress64()
	{
		return _remoteAddress;
	}

	uint8_t Rx64IoSampleResponse::getRssiOffset()
	{
		return 8;
	}

	TxStatusResponse::TxStatusResponse() : FrameIdResponse()
	{

	}

	uint8_t TxStatusResponse::getStatus()
	{
		return getFrameData()[1];
	}

	bool TxStatusResponse::isSuccess()
	{
		return getStatus() == SUCCESS;
	}

	uint8_t RxResponse::getRssi()
	{
		return getFrameData()[getRssiOffset()];
	}

	uint8_t RxResponse::getOption()
	{
		return getFrameData()[getRssiOffset() + 1];
	}

	bool RxResponse::isAddressBroadcast()
	{
		return (getOption() & 2) == 2;
	}

	bool RxResponse::isPanBroadcast()
	{
		return (getOption() & 4) == 4;
	}

	uint8_t RxResponse::getDataLength()
	{
		return getPacketLength() - getDataOffset() - 1;
	}

	uint8_t RxResponse::getDataOffset()
	{
		return getRssiOffset() + 2;
	}

	uint8_t Rx16Response::getRssiOffset()
	{
		return RX_16_RSSI_OFFSET;
	}

	uint8_t Rx64Response::getRssiOffset()
	{
		return RX_64_RSSI_OFFSET;
	}

#endif

	RxDataResponse::RxDataResponse() : XBeeResponse()
	{

	}

	uint8_t RxDataResponse::getData(int index)
	{
		return getFrameData()[getDataOffset() + index];
	}

	uint8_t *RxDataResponse::getData()
	{
		return getFrameData() + getDataOffset();
	}

	FrameIdResponse::FrameIdResponse()
	{ }

	uint8_t FrameIdResponse::getFrameId()
	{
		return getFrameData()[0];
	}

	ModemStatusResponse::ModemStatusResponse()
	{
	}

	uint8_t ModemStatusResponse::getStatus()
	{
		return getFrameData()[0];
	}

}
