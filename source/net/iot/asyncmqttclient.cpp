/*
 * Asynchronous MQTT client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/function.h>
#include <lwiot/scopedlock.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/map.h>

#include <lwiot/network/asyncmqttclient.h>

#include <lwiot/kernel/functionalthread.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	AsyncMqttClient::AsyncMqttClient() : MqttClient(), _executor("mqtt"), _lock(false), _running(false)
	{
	}

	AsyncMqttClient::AsyncMqttClient(const lwiot::AsyncMqttClient::ReconnectHandler &handler) : AsyncMqttClient()
	{
		this->_reconnect_handler = handler;
	}

	AsyncMqttClient::~AsyncMqttClient()
	{
		ScopedLock lock(this->_lock);

		if(!this->_running)
			return;

		lock.unlock();
		this->stop();
	}

	void AsyncMqttClient::setReconnectHandler(const lwiot::AsyncMqttClient::ReconnectHandler &handler)
	{
		ScopedLock lock(this->_lock);
		this->_reconnect_handler = handler;
	}

	void AsyncMqttClient::start(lwiot::TcpClient &client)
	{
		ScopedLock lock(this->_lock);

		this->begin(client);
		this->_running = true;
		this->_executor.start([&]() -> void {
			this->run();
		});
	}

	void AsyncMqttClient::stop()
	{
		ScopedLock lock(this->_lock);
		this->_running = false;

		lock.unlock();
		Thread::sleep(200);
		lock.lock();

		this->_executor.stop();
		this->_executor.join();
		this->disconnect();
	}

	void AsyncMqttClient::run()
	{
		this->_lock.lock();
		auto running = this->_running;

		this->setCallback([&](const String& topic, const ByteBuffer& buffer) {
			this->invoke(topic, buffer);
		});

		this->_lock.unlock();

		while(running) {
			ScopedLock lock(this->_lock);

			while(!MqttClient::connected() && this->_running && this->_id.length() != 0) {
				if(!this->reconnect())
					continue;

				MqttClient::connect(this->_id, this->_user, this->_pass,
				                    this->_will_topic, this->_will_qos,
				                    this->_will_retain, this->_will, this->_clean);

				if(MqttClient::connected()) {
					lock.unlock();
					this->_reconnect_handler();
					lock.lock();
				}

				lock.unlock();
				Thread::sleep(100);
				lock.lock();
			}

			this->loop();
			running = this->_running;

			lock.unlock();
			Thread::sleep(100);
		}
	}

	bool AsyncMqttClient::unsubscribe(const lwiot::String &topic)
	{
		ScopedLock lock(this->_lock);
		return MqttClient::unsubscribe(topic);
	}

	bool AsyncMqttClient::publish(const lwiot::String &topic, const lwiot::ByteBuffer &data, bool retained)
	{
		ScopedLock lock(this->_lock);
		return MqttClient::publish(topic, data, retained);
	}

	bool AsyncMqttClient::connect(const lwiot::String &id, const lwiot::String &user, const lwiot::String &pass,
	                              const lwiot::String &willTopic, uint8_t willQos, bool willRetain,
	                              const lwiot::String &willMessage, bool cleanSession)
	{
		ScopedLock lock(this->_lock);

		this->_id = id;
		this->_user = user;
		this->_pass = pass;
		this->_will = willMessage;
		this->_will_qos = willQos;
		this->_will_retain = willRetain;
		this->_will_topic = willTopic;
		this->_clean = cleanSession;

		return MqttClient::connect(id, user, pass, willTopic, willQos, willRetain, willMessage, cleanSession);
	}

	bool AsyncMqttClient::subscribe(const String &topic, AsyncMqttClient::AsyncHandler handler, QoS qos)
	{
		ScopedLock lock(this->_lock);

		this->_handlers.add(topic, stl::move(handler));
		return MqttClient::subscribe(topic, qos);
	}

	void AsyncMqttClient::invoke(const lwiot::String &topic, const lwiot::ByteBuffer &data) const
	{
		const AsyncHandler& handler = this->_handlers[topic];

		if(!handler)
			return;

		handler(data);
	}
}
