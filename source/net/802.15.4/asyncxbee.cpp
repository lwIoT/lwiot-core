/*
 * Async wrapper around the Xbee libraries.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/network/stdnet.h>

#include <lwiot/network/xbee/xbee.h>
#include <lwiot/network/xbee/asyncxbee.h>

namespace lwiot
{
	AsyncXbee::AsyncXbee() : _lock(false), _running(false)
	{
	}

	AsyncXbee::AsyncXbee(lwiot::XBee &xb) : AsyncXbee()
	{
		this->_xb = xb;
	}

	AsyncXbee::AsyncXbee(lwiot::Stream &stream) : AsyncXbee()
	{
		this->_xb.begin(stream);
	}

	void AsyncXbee::begin(lwiot::AsyncXbee::ResponseHandler &&handler)
	{
		this->_handler = stl::move(handler);
		this->start();
	}

	void AsyncXbee::begin(const lwiot::AsyncXbee::ResponseHandler &handler)
	{
		this->_handler = handler;
		this->start();
	}

	AsyncXbee::~AsyncXbee()
	{
		ScopedLock lock(this->_lock);
		this->_running = false;
	}

	void AsyncXbee::setHandler(const ResponseHandler &handler)
	{
		ScopedLock lock(this->_lock);
		this->_handler = handler;
	}

	void AsyncXbee::run()
	{
		while(true) {
			UniqueLock<Lock> lock(this->_lock);

			if(!this->_running || !this->_handler)
				break;

			if(this->_xb.readPacket(500)) {
				XBeeResponse rx;

				this->_xb.getResponse(rx);
				this->_handler(rx);
			}
		}
	}

	void AsyncXbee::setDevice(lwiot::XBee &xb)
	{
		this->_xb = xb;
	}

	XBee& AsyncXbee::getDevice()
	{
		return this->_xb;
	}

	void AsyncXbee::setNetworkID(uint16_t netid)
	{
		uint16_t id = to_netorders(netid);
		uint8_t cmd[] = {'I', 'D'};

		this->sendCommand(cmd, (uint8_t*) &id, sizeof(id));
	}

	uint64_t AsyncXbee::getNetworkAddress()
	{
		uint32_t lo, hi;
		uint8_t sl[] = {'S', 'L'};
		uint8_t sh[] = {'S', 'H'};
		uint8_t* buf;
		uint64_t addr;

		auto sl_value = stl::move(this->sendCommand(sl));
		auto sh_value = stl::move(this->sendCommand(sh));

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

	ByteBuffer AsyncXbee::sendCommand(const uint8_t *cmd, const uint8_t *value, size_t length)
	{
		AtCommandRequest rq;
		AtCommandResponse response;
		ByteBuffer buffer(4, true);

		rq.setCommand((uint8_t*)cmd);

		if(length > 0) {
			rq.setCommandValue((uint8_t*)value);
			rq.setCommandValueLength(length);
		}

		UniqueLock<Lock> lock(this->_lock);

		this->_xb.send(rq);
		Thread::sleep(10);
		this->_xb.readPacket();

		if(this->_xb.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
			this->_xb.getResponse().getAtCommandResponse(response);

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
			}
		}


		return stl::move(buffer);
	}

	void AsyncXbee::setChannel(uint8_t channel)
	{
		uint8_t cmd[] = {'C', 'H'};
		this->sendCommand(cmd, &channel, sizeof(channel));
	}
}
