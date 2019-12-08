/*
 * MQTT client class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file mqttclient.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/stream.h>
#include <lwiot/bytebuffer.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpclient.h>

#include <lwiot/stl/map.h>
#include <lwiot/function.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief MQTT client.
	 * @ingroup net
	 */
	class MqttClient {
	public:
		typedef Function<void(const String&, const ByteBuffer&)> Handler; //!< Handler type.

		/**
		 * @brief Quality of Service indicator.
		 */
		enum QoS {
			QOS0 = 0,
			QOS1,
			QOS2
		};

		explicit MqttClient(); //!< Construct a MQTT client.
		virtual ~MqttClient() = default; //!< Destruct a MQTT client.

		/**
		 * @brief Start the client socket.
		 * @param client TCP socket.
		 */
		void begin(TcpClient& client);

		/**
		 * @brief Set the underlying TCP socket.
		 * @param client The underlying TCP socket to set.
		 */
		void setClient(TcpClient& client);
		bool reconnect(); //!< Reconnect to the broker.

		/**
		 * @brief Set the callback handler.
		 * @param cb Callback handler.
		 */
		void setCallback(const Handler& cb)
		{
			this->_cb = cb;
		}

		/**
		 * @brief Set the callback handler.
		 * @tparam CB Functor type.
		 * @param cb Callback handler.
		 */
		template <typename CB>
		void setCallback(CB&& cb)
		{
			this->_cb = stl::forward<CB>(cb);
		}

		/**
		 * @brief Connect to a MQTT broker.
		 * @param id Client ID.
		 * @param user Username.
		 * @param pass Password.
		 * @return Success indicator.
		 */
		bool connect(const String& id, const String& user, const String& pass);

		/**
		 * @brief Connect to a MQTT broker.
		 * @param id Client ID.
		 * @param user Username.
		 * @param pass Password.
		 * @param willTopic Will topic.
		 * @param willQos QoS level.
		 * @param willRetain Retain flag.
		 * @param willMessage Will message.
		 * @param cleanSession Clean session flag.
		 * @return Success indicator.
		 */
		virtual bool connect(const String& id, const String& user, const String& pass,
				const String& willTopic, uint8_t willQos, bool willRetain,
				const String& willMessage, bool cleanSession);
		void disconnect(); //!< Disconnect from the MQTT broker.

		/**
		 * @brief Subscribe to a topic.
		 * @param topic Topic to subscribe to.
		 * @param qos QoS level.
		 * @return Success indicator.
		 */
		bool subscribe(const stl::String& topic, QoS qos);

		/**
		 * @brief Unsubscribe from a topic.
		 * @param topic Topic to unsubscribe from.
		 * @return A success indicator.
		 */
		virtual bool unsubscribe(const stl::String& topic);

		bool loop(); //!< Handle MQTT messages.

		virtual bool connected() //!< Check if connected.
		{
			return this->isConnected();
		}

		/**
		 * @brief Publish a new message.
		 * @param topic Topic to publish to.
		 * @param data Data to publish.
		 * @param retained Retain indicator.
		 * @return A success indicator.
		 */
		virtual bool publish(const stl::String& topic, const ByteBuffer& data, bool retained);

		/**
		 * @brief Publish a new message.
		 * @param topic Topic to publish to.
		 * @param data Data to publish.
		 * @param retained Retain indicator.
		 * @return A success indicator.
		 */
		bool publish(const stl::String& topic, const stl::String& data, bool retained = false);

		virtual inline int state() const //!< Get the MQTT state.
		{
			return this->_state;
		}

		static constexpr int MQTT_MAX_PACKET_SIZE = 512;
		static constexpr int MQTT_MAX_HEADER_SIZE =   5;
		static constexpr int MQTT_SOCKET_TIMEOUT  =  15;

	private:
		stl::ReferenceWrapper<TcpClient> _io;
		Stream* _stream;
		int _state;

		/* Internals */
		mutable ByteBuffer _buffer;
		uint16_t _nextMsgId;
		unsigned long _lastOutActivity;
		unsigned long _lastInActivity;
		bool _pingOutstanding;
		Handler _cb;

		/* Methods */
		size_t build(uint8_t header, uint16_t length) const;
		uint16_t readPacket(uint8_t* data);
		bool read(uint8_t * result);
		bool read(uint8_t * result, uint16_t * index);

		uint16_t write(const stl::String& data, uint16_t pos);
		size_t write(uint8_t);
		bool write(uint8_t header, size_t length);
		bool isConnected();
	};
}
