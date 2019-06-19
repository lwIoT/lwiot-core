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
				this->_xb.resetResponse();
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

	bool AsyncXbee::transmit(const lwiot::String &data, uint16_t addr) const
	{
		UniqueLock<Lock> lock(this->_lock);
		ZBExplicitTxRequest transmit;

		transmit.setAddress64(0xFFFFFFFFFFFFFFFF);
		transmit.setAddress16(addr);
		transmit.setPayload((uint8_t *)data.c_str());
		transmit.setPayloadLength(data.length());
		transmit.setProfileId(DEFAULT_PROFILE_ID);
		transmit.setFrameId(DEFAULT_FRAME_ID);
		transmit.setClusterId(DEFAULT_CLUSTER_ID);

		this->_xb.send(transmit);
		return this->validateTxRequest();
	}

	bool AsyncXbee::transmit(const lwiot::String &data, uint64_t addr) const
	{
		UniqueLock<Lock> lock(this->_lock);
		ZBExplicitTxRequest transmit;

		transmit.setAddress64(addr);
		transmit.setAddress16(0xFFFE);
		transmit.setPayload((uint8_t *)data.c_str());
		transmit.setPayloadLength(data.length());
		transmit.setProfileId(DEFAULT_PROFILE_ID);
		transmit.setFrameId(DEFAULT_FRAME_ID);
		transmit.setClusterId(DEFAULT_CLUSTER_ID);

		this->_xb.send(transmit);
		return this->validateTxRequest();
	}

	uint16_t AsyncXbee::getParentAddress() const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.getParentAddress();
	}

	bool AsyncXbee::send(lwiot::XBeeRequest &request) const
	{
		UniqueLock<Lock> lock(this->_lock);

		this->_xb.send(request);
		return this->validateTxRequest();
	}

	void AsyncXbee::setSleepMode(lwiot::XBee::SleepMode mode) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.setSleepMode(mode);
	}

	uint8_t AsyncXbee::getMaxPayloadSize() const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.getMaxPayloadSize();
	}

	void AsyncXbee::setNodeIdentifier(const lwiot::stl::String &id) const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.setNodeIdentifier(id);
	}

	void AsyncXbee::setMaxHops(uint8_t max) const
	{
		UniqueLock<Lock> lock(this->_lock);
		return this->_xb.setMaxHops(max);
	}

	void AsyncXbee::enableCoordinator(bool enable) const
	{
		UniqueLock<Lock> lock(this->_lock);
		this->_xb.enableCoordinator(enable);
	}

	bool AsyncXbee::validateTxRequest() const
	{
		ZBTxStatusResponse zbtx;

		this->_xb.readPacket(500);
		this->_xb.getResponse(zbtx);
		this->_xb.resetResponse();

		return zbtx.isSuccess();
	}
}
