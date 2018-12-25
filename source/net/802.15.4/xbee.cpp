/*
 * XBee wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeeaddress.h>
#include <lwiot/network/xbee/xbeeresponse.h>
#include <lwiot/network/xbee/xbeerequest.h>
#include <lwiot/network/xbee/xbee.h>

namespace lwiot
{
	XBee::XBee() : _response(XBeeResponse()), _serial(nullptr)
	{
		_pos = 0;
		_escape = false;
		_checksumTotal = 0;
		_nextFrameId = 0;

		_response.init();
		_response.setFrameData(_responseFrameData);
	}

	uint8_t XBee::getNextFrameId()
	{
		_nextFrameId++;

		if(_nextFrameId == 0) {
			_nextFrameId = 1;
		}

		return _nextFrameId;
	}

	void XBee::begin(Stream &serial)
	{
		this->setSerial(serial);
	}

	void XBee::setSerial(Stream &serial)
	{
		_serial = &serial;
	}

	bool XBee::available()
	{
		return _serial->available();
	}

	uint8_t XBee::read()
	{
		return _serial->read();
	}

	void XBee::write(uint8_t val)
	{
		_serial->write(val);
	}

	XBeeResponse &XBee::getResponse()
	{
		return _response;
	}

	void XBee::getResponse(XBeeResponse &response)
	{

		response.setMsbLength(_response.getMsbLength());
		response.setLsbLength(_response.getLsbLength());
		response.setApiId(_response.getApiId());
		response.setFrameLength(_response.getFrameDataLength());

		response.setFrameData(_response.getFrameData());
	}

	void XBee::readPacketUntilAvailable()
	{
		while(!(getResponse().isAvailable() || getResponse().isError())) {
			// read some more
			readPacket();
		}
	}

	void XBee::readPacket()
	{
		if(_response.isAvailable() || _response.isError()) {
			resetResponse();
		}

		while(available()) {
			b = read();

			if(_pos > 0 && b == START_BYTE && ATAP == 2) {
				_response.setErrorCode(UNEXPECTED_START_BYTE);
				return;
			}

			if(_pos > 0 && b == ESCAPE) {
				if(available()) {
					b = read();
					b = 0x20 ^ b;
				} else {
					_escape = true;
					continue;
				}
			}

			if(this->_escape) {
				b = 0x20 ^ b;
				this->_escape = false;
			}

			if(_pos >= API_ID_INDEX) {
				_checksumTotal += b;
			}

			switch(_pos) {
			case 0:
				if(b == START_BYTE) {
					_pos++;
				}

				break;
			case 1:
				_response.setMsbLength(b);
				_pos++;

				break;
			case 2:
				_response.setLsbLength(b);
				_pos++;

				break;
			case 3:
				_response.setApiId(b);
				_pos++;
				break;

			default:
				if(_pos > MAX_FRAME_DATA_SIZE) {
					_response.setErrorCode(PACKET_EXCEEDS_BYTE_ARRAY_LENGTH);
					return;
				}

				if(_pos == (_response.getPacketLength() + 3)) {
					if((_checksumTotal & 0xff) == 0xff) {
						_response.setChecksum(b);
						_response.setAvailable(true);

						_response.setErrorCode(NO_ERROR);
					} else {
						_response.setErrorCode(CHECKSUM_FAILURE);
					}

					_response.setFrameLength(_pos - 4);
					_pos = 0;

					return;
				} else {
					_response.getFrameData()[_pos - 4] = b;
					_pos++;
				}
			}
		}
	}

	bool XBee::readPacket(int timeout)
	{
		time_t tick;
		time_t start;

		if(timeout < 0) {
			return false;
		}

		start = lwiot_tick_ms();
		tick = start;

		while((tick - start) < timeout) {
			readPacket();

			if(getResponse().isAvailable()) {
				return true;
			} else if(getResponse().isError()) {
				return false;
			}

			tick = lwiot_tick_ms();
		}

		return false;
	}
}
