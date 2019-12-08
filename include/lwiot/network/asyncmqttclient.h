/*
 * Asynchronous MQTT client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file asyncmqttclient.h

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
	/**
	 * @brief Async MQTT client.
	 * @ingroup net
	 */
	class AsyncMqttClient : private MqttClient {
	public:
		typedef Function<void(const ByteBuffer&)> AsyncHandler; //!< Async handler.
		typedef Function<void(void)> ReconnectHandler; //!< Reconnect handler.

		/**
		 * @brief Create a new async MQTT client.
		 * @param tmo Timeout.
		 */
		explicit AsyncMqttClient(int tmo = 1000);

		/**
		 * @brief Construct ana sync MQTT client.
		 * @param handler Reconnect handler.
		 * @param tmo Timeout value.
		 */
		explicit AsyncMqttClient(const ReconnectHandler& handler, int tmo = 1000);
		explicit AsyncMqttClient(const AsyncMqttClient& rhs) = delete;
		explicit AsyncMqttClient(AsyncMqttClient&& rhs) noexcept  = delete;

		/**
		 * @brief Async MQTT client destructor.
		 */
		~AsyncMqttClient() override;

		AsyncMqttClient& operator=(const AsyncMqttClient& rhs) = delete;
		AsyncMqttClient& operator=(AsyncMqttClient&& rhs) noexcept  = delete;

		/**
		 * @brief Set the reconnect handler.
		 * @param handler The reconnect handler.
		 */
		void setReconnectHandler(const ReconnectHandler& handler);

		/**
		 * @brief Start the MQTT client.
		 * @param client TCP client.
		 * @return A success indicator.
		 */
		bool start(TcpClient& client);

		/**
		 * @brief Stop the MQTT client.
		 */
		void stop();

		/**
		 * @brief Connect to an MQTT broker.
		 * @param id Client ID.
		 * @param user Username.
		 * @param pass Password.
		 * @param willTopic Will topic.
		 * @param willQos Will QoS.
		 * @param willRetain Will retain indicator.
		 * @param willMessage Will message.
		 * @param cleanSession Clean session indicator.
		 * @return A success indicator.
		 */
		bool connect(const String& id, const String& user, const String& pass,
		                     const String& willTopic, uint8_t willQos, bool willRetain,
		                     const String& willMessage, bool cleanSession) override ;
		using MqttClient::connect;

		/**
		 * @brief Subscribe to a topic.
		 * @param topic Topic to subscribe to.
		 * @param handler Message handler.
		 * @param qos QoS level.
		 * @return Success indicator.
		 */
		bool subscribe(const stl::String& topic, AsyncHandler handler, QoS qos = QOS0);

		/**
		 * @brief Subscribe to a topic.
		 * @tparam Func Functor type.
		 * @param topic Topic to subscribe to.
		 * @param handler Message handler.
		 * @param qos QoS level.
		 * @return Success indicator.
		 */
		template <typename Func>
		inline bool subscribe(const stl::String& topic, Func&& handler, QoS qos = QOS0)
		{
			ScopedLock lock(this->_lock);

			this->_handlers.add(topic, handler);
			return MqttClient::subscribe(topic, qos);
		}

		/**
		 * @brief Unsubscribe from a topic.
		 * @param topic Topic to unsubscribe from.
		 * @return A success indicator.
		 */
		bool unsubscribe(const stl::String& topic) override;

		/**
		 * @brief Publish a new message.
		 * @param topic Topic to publish to.
		 * @param data Data to publish.
		 * @param retained Retain indicator.
		 * @return A success indicator.
		 */
		bool publish(const stl::String& topic, const ByteBuffer& data, bool retained) override;
		using MqttClient::publish;

		/**
		 * @brief Check if \p *this is connected to a broker.
		 * @return Connected indicator.
		 */
		inline bool connected() override
		{
			ScopedLock lock(this->_lock);
			return MqttClient::connected();
		}

		/**
		 * @brief Check the current MQTT state.
		 * @return The current client state.
		 */
		inline int state() const override
		{
			ScopedLock lock(this->_lock);
			return MqttClient::state();
		}

	protected:
		/**
		 * @brief Client runner.
		 * @see Thread::run
		 */
		virtual void run();

	private:
		stl::Map<stl::String, AsyncHandler> _handlers;
		ReconnectHandler _reconnect_handler;
		FunctionalThread _executor;
		mutable Lock _lock;
		bool _running;

		stl::String _id, _user, _pass, _will_topic, _will;
		uint8_t _will_qos;
		bool _will_retain;
		bool _clean;
		int _tmo;

		/* Methods */
		void invoke(const String& topic, const ByteBuffer& data) const;
	};
}
