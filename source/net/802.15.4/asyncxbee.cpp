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
	AsyncXbee::AsyncXbee() : Thread("axbee"), _lock(false), _running(false)
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
		this->init();
	}

	void AsyncXbee::begin(const lwiot::AsyncXbee::ResponseHandler &handler)
	{
		this->_handler = handler;
		this->init();
	}

	void AsyncXbee::init()
	{
		this->_xb.setZigbeePro(true);
		this->_running = true;
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
			Thread::sleep(100);
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

	void AsyncXbee::setNetworkID(uint16_t netid) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setNetworkID(netid);
	}

	void AsyncXbee::setChannel(uint16_t channel) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setChannel(channel);
	}

	uint64_t AsyncXbee::getHardwareAddress() const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.getNetworkAddress();
	}

	void AsyncXbee::setZigbeePro(bool enabled) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setZigbeePro(enabled);
	}

	uint16_t AsyncXbee::getNetworkAddress() const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.getNetworkAddress();
	}

	void AsyncXbee::enableEncryption(bool enabled) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.enableEncryption(enabled);
	}

	void AsyncXbee::setNetworkKey(const lwiot::ByteBuffer &key) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setNetworkKey(key);
	}

	void AsyncXbee::setLinkKey(const lwiot::ByteBuffer &key) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setLinkKey(key);
	}

	void AsyncXbee::setEncryptionOptions(lwiot::XBee::EncryptionOptions opts) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.getNetworkAddress();
	}

	void AsyncXbee::setDevice(lwiot::XBee &xb)
	{
		this->_xb = xb;
	}

	XBee& AsyncXbee::getDevice()
	{
		return this->_xb;
	}
}
