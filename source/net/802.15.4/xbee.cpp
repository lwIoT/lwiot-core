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
#include <lwiot/system.h>

#include <lwiot/network/xbee/constants.h>
#include <lwiot/network/xbee/xbeeaddress.h>
#include <lwiot/network/xbee/xbeeresponse.h>
#include <lwiot/network/xbee/xbeerequest.h>
#include <lwiot/network/xbee/xbee.h>

#include <lwiot/stl/move.h>
#include <lwiot/kernel/uniquelock.h>
#include <lwiot/network/stdnet.h>

namespace lwiot
{
	XBee::XBee() : _response(XBeeResponse())
	{
		_pos = 0;
		_escape = false;
		_checksumTotal = 0;
		_nextFrameId = 0;

		_response.init();
		_response.setFrameData(_responseFrameData);
	}

	XBee::XBee(const lwiot::XBee &xb)
	{
		this->copy(xb);
	}

	XBee::XBee(const lwiot::XBee &&xb) noexcept
	{
		this->copy(xb);
	}

	XBee& XBee::operator=(const lwiot::XBee &xb)
	{
		this->copy(xb);
		return *this;
	}

	XBee& XBee::operator=(const lwiot::XBee &&xb) noexcept
	{
		this->copy(xb);
		return *this;
	}

	void XBee::setSleepPin(const lwiot::GpioPin &pin)
	{
		this->_sleep_pin = pin;
		this->_sleep_pin.output();
	}

	void XBee::sleep()
	{
		/* Assert the sleep pin (high) */
		if(this->_sleep_pin.pin() <= 0)
			return;

		this->_sleep_pin.write(true);
	}

	void XBee::wakeUp()
	{
		if(this->_sleep_pin.pin() <= 0)
			return;

		this->_sleep_pin.write(false);
	}

	void XBee::copy(const XBee& rhs)
	{
		this->_checksumTotal = rhs._checksumTotal;
		this->_escape = rhs._escape;
		this->_nextFrameId = rhs._nextFrameId;
		this->_pos = rhs._pos;
		this->_checksumTotal = rhs._checksumTotal;
		this->_response = rhs._response;
		this->_nextFrameId = rhs._nextFrameId;
		this->_serial = rhs._serial;

		memcpy(this->_responseFrameData, rhs._responseFrameData, MAX_FRAME_DATA_SIZE);
	}

	void XBee::writeToFlash()
	{
		uint8_t cmd[] = {'W', 'R'};
		this->sendCommand(cmd, 1000);
	}

	void XBee::resetResponse()
	{
		this->_pos = 0;
		this->_escape = false;
		this->_checksumTotal = 0;
		this->_response.reset();
	}

	void XBee::send(ZigbeeAddress addr, const lwiot::ByteBuffer &buffer) const
	{
		auto raw = buffer.data();
		ZBTxRequest tx;

		tx.setPayload(raw);
		tx.setPayloadLength(buffer.index());

		if(addr.is64Bit())
			tx.setAddress64(addr.getAddress64());
		else
			tx.setAddress16(addr.getAddress16());

		this->send(tx);
	}

	void XBee::send(lwiot::ZigbeeAddress addr, const lwiot::ByteBuffer &buffer, uint16_t profile, uint16_t cluster) const
	{
		auto raw = buffer.data();
		ZBExplicitTxRequest tx;

		if(addr.is64Bit())
			tx.setAddress64(addr.getAddress64());
		else
			tx.setAddress16(addr.getAddress16());

		tx.setPayload(raw, buffer.index());
		tx.setClusterId(cluster);
		tx.setProfileId(profile);
		this->send(tx);
	}

	void XBee::send(XBeeRequest &request) const
	{
		sendByte(START_BYTE, false);

		uint8_t msbLen = ((request.getFrameDataLength() + 2) >> 8) & 0xff;
		uint8_t lsbLen = (request.getFrameDataLength() + 2) & 0xff;

		sendByte(msbLen, true);
		sendByte(lsbLen, true);

		sendByte(request.getApiId(), true);
		sendByte(request.getFrameId(), true);

		uint8_t checksum = 0;

		checksum += request.getApiId();
		checksum += request.getFrameId();

		for(int i = 0; i < request.getFrameDataLength(); i++) {
			sendByte(request.getFrameData(i), true);
			checksum += request.getFrameData(i);
		}

		checksum = 0xff - checksum;
		sendByte(checksum, true);
	}

	void XBee::sendByte(uint8_t byte, bool escape) const
	{
		if(escape && (byte == START_BYTE || byte == ESCAPE || byte == XON || byte == XOFF)) {
			write(ESCAPE);
			write(byte ^ 0x20);
		} else {
			write(byte);
		}
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
		this->fetchMaxPayloadSize();
	}

	void XBee::setSerial(Stream &serial)
	{
		this->_serial = serial;
	}

	bool XBee::available()
	{
		return _serial->available();
	}

	uint8_t XBee::read()
	{
		return _serial->read();
	}

	void XBee::write(uint8_t val) const
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
#if MAX_FRAME_DATA_SIZE < UINT8_MAX
				if(_pos > MAX_FRAME_DATA_SIZE) {
					_response.setErrorCode(PACKET_EXCEEDS_BYTE_ARRAY_LENGTH);
					return;
				}
#endif

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

	void XBee::setZigbeePro(bool enabled)
	{
		uint8_t value;
		uint8_t cmd[] = {'Z', 'S'};

		value = enabled ? 2 : 0;
		this->sendCommand(cmd, 100, &value, sizeof(value));
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


	void XBee::apply()
	{
		uint8_t cmd[] = {'A', 'C'};
		this->sendCommand(cmd, 400);
	}

	void XBee::setNetworkID(uint16_t netid)
	{
		uint16_t id = to_netorders(netid);
		uint8_t cmd[] = {'I', 'D'};

		this->sendCommand(cmd, 10, (uint8_t*) &id, sizeof(id));
	}

	uint64_t XBee::getHardwareAddress()
	{
		uint32_t lo, hi;
		uint8_t sl[] = {'S', 'L'};
		uint8_t sh[] = {'S', 'H'};
		uint8_t* buf;
		uint64_t addr;

		auto sl_value = stl::move(this->sendCommand(sl, 10));
		auto sh_value = stl::move(this->sendCommand(sh, 10));

		if(sl_value.index() != 4 || sh_value.index() != 4)
			return 0ULL;

		lo = hi = 0U;
		buf = (uint8_t*) &lo;

		for(auto idx = 0; idx < sl_value.index(); idx++) {
			buf[idx] = sl_value[idx];
		}

		buf = (uint8_t*) &hi;

		for(auto idx = 0; idx < sh_value.index(); idx++) {
			buf[idx] = sh_value[idx];
		}

		hi = to_netorderl(hi);
		lo = to_netorderl(lo);

		addr = lo;
		addr |= ((uint64_t )hi) << 32;

		print_dbg("64-bit network address: 0x%llX\n", addr);
		return addr;
	}

	ByteBuffer XBee::sendCommand(const uint8_t *cmd, int tmo, const uint8_t *value, size_t length)
	{
		AtCommandRequest rq;
		AtCommandResponse response;
		ByteBuffer buffer(4, true);

		rq.setCommand((uint8_t*)cmd);

		if(length > 0) {
			rq.setCommandValue((uint8_t*)value);
			rq.setCommandValueLength(length);
		}

		this->send(rq);
		System::delay(tmo);

		this->readPacketUntilAvailable();

		if(this->getResponse().getApiId() == AT_COMMAND_RESPONSE) {
			this->getResponse().getAtCommandResponse(response);

			if(response.isOk()) {
				if(response.getValueLength() > 0)
					buffer = ByteBuffer(response.getValueLength(), true);

				print_dbg("AT command: %c%c - Value: 0x",
				          response.getCommand()[0], response.getCommand()[1]);

				for(auto idx = 0; idx < response.getValueLength(); idx++) {
					auto value = response.getValue();

					printf("%X", value[idx]);
					buffer.write(value[idx]);
				}

				printf("\n");
				buffer.setIndex(response.getValueLength());
				this->resetResponse();
			}
		}

		return stl::move(buffer);
	}

	uint16_t XBee::getNetworkAddress()
	{
		uint8_t cmd[] = {'M', 'Y'};
		uint16_t rv;

		auto value = this->sendCommand(cmd, 100);
		rv = (value[0] << 8) | value[1];

		return rv;
	}

	void XBee::setChannel(uint16_t channel)
	{
		uint8_t cmd[] = {'S', 'C'};

		channel = to_netorders(channel);
		this->sendCommand(cmd, 10, (uint8_t*) &channel, sizeof(channel));
	}

	void XBee::setSleepMode(lwiot::XBee::SleepMode sleepmode)
	{
		uint8_t cmd[] = {'S', 'M'};
		uint8_t value = sleepmode;

		this->sendCommand(cmd, 100, &value, sizeof(value));
	}

	void XBee::enableEncryption(bool enabled)
	{
		uint8_t cmd[] = {'E', 'E'};
		uint8_t enable = enabled;

		this->sendCommand(cmd, 100, &enable, sizeof(enable));
	}

	void XBee::setEncryptionOptions(lwiot::XBee::EncryptionOptions opts)
	{
		uint8_t options = opts;
		uint8_t cmd[] = {'E', 'O'};

		this->sendCommand(cmd, 100, &options, sizeof(options));
	}

	uint16_t XBee::getParentAddress()
	{
		uint16_t rv;
		uint8_t cmd[] = {'M', 'P'};

		auto result = stl::move(this->sendCommand(cmd, 100));
		rv = (result[0] << 8) | result[1];

		return rv;
	}

	void XBee::setNetworkKey(const lwiot::ByteBuffer &key)
	{
		uint8_t* data;
		uint8_t null = 0;
		uint8_t cmd[] = {'N', 'K'};
		size_t length;

		if(key.index() == 0) {
			data = &null;
			length = sizeof(null);
		} else {
			data = key.data();
			length = key.index();
		}

		this->sendCommand(cmd, 100, data, length);
	}

	void XBee::setNodeIdentifier(const lwiot::stl::String &id)
	{
		uint8_t cmd[] = {'N', 'I'};

		if(id.length() == 0) {
			this->sendCommand(cmd, 100);
		} else {
			this->sendCommand(cmd, 100, (uint8_t *)id.c_str(), id.length());
		}
	}

	void XBee::setMaxHops(uint8_t max)
	{
		uint8_t cmd[] = {'N', 'H'};
		this->sendCommand(cmd, 1000, &max, sizeof(max));
	}

	void XBee::setLinkKey(const lwiot::ByteBuffer &key)
	{
		uint8_t* data;
		uint8_t null = 0;
		uint8_t cmd[] = {'K', 'Y'};
		size_t length;

		if(key.index() == 0) {
			data = &null;
			length = sizeof(null);
		} else {
			data = key.data();
			length = key.index();
		}

		this->sendCommand(cmd, 100, data, length);
	}

	uint8_t XBee::getMaxPayloadSize() const
	{
		return this->_max_payload;
	}

	void XBee::fetchMaxPayloadSize()
	{
		uint8_t cmd[] = {'N', 'P'};

		auto result = stl::move(this->sendCommand(cmd, 100));
		this->_max_payload = result.at(0);
	}

	void XBee::enableCoordinator(bool enable)
	{
		uint8_t cmd[] = {'C', 'E'};
		uint8_t arg = enable;

		this->sendCommand(cmd, 100, &arg, sizeof(arg));
		this->readPacket(200);
		this->resetResponse();
	}
}
