/*
 * Asynchronous MQTT client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/function.h>
#include <lwiot/scopedlock.h>

#include <lwiot/kernel/functionalthread.h>
#include <lwiot/kernel/lock.h>

#include <lwiot/network/mqttclient.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/stdnet.h>

#include <lwiot/stl/map.h>
#include <lwiot/stl/string.h>

namespace lwiot
{
	class AsyncMqttClient : private MqttClient {
	public:
		explicit AsyncMqttClient();
		virtual ~AsyncMqttClient();

		typedef Function<void(const ByteBuffer&)> AsyncHandler;

		void start(TcpClient& client);
		void stop();

		bool connect(const String& id, const String& user, const String& pass,
		                     const String& willTopic, uint8_t willQos, bool willRetain,
		                     const String& willMessage, bool cleanSession) override ;
		using MqttClient::connect;

		bool subscribe(const stl::String& topic, AsyncHandler handler, QoS qos = QOS0);

		template <typename Func>
		inline bool subscribe(const stl::String& topic, Func&& handler, QoS qos = QOS0)
		{
			ScopedLock lock(this->_lock);

			this->_handlers.add(topic, handler);
			return MqttClient::subscribe(topic, qos);
		}

		bool unsubscribe(const stl::String& topic) override;
		bool publish(const stl::String& topic, const ByteBuffer& data, bool retained) override;
		using MqttClient::publish;


		inline bool connected() override
		{
			ScopedLock lock(this->_lock);
			return MqttClient::connected();
		}

		inline int state() const override
		{
			ScopedLock lock(this->_lock);
			return MqttClient::state();
		}

	protected:
		virtual void run();

	private:
		stl::Map<stl::String, AsyncHandler> _handlers;
		FunctionalThread _executor;
		mutable Lock _lock;
		bool _running;

		stl::String _id, _user, _pass, _will_topic, _will;
		uint8_t _will_qos;
		bool _will_retain;
		bool _clean;

		/* Methods */
		void invoke(const String& topic, const ByteBuffer& data) const;
	};
}
