/*
 * MQTT client class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
	class MqttClient {
	public:
		typedef Function<void(const String&, const ByteBuffer&)> Handler;
		enum QoS {
			QOS0 = 0,
			QOS1,
			QOS2
		};

		explicit MqttClient();

		void begin(TcpClient& client);
		void reconnect();

		void setCallback(const Handler& cb)
		{
			this->_cb = cb;
		}

		template <typename CB>
		void setCallback(CB&& cb)
		{
			this->_cb = stl::forward<CB>(cb);
		}

		bool connect(const String& id, const String& user, const String& pass);
		bool connect(const String& id, const String& user, const String& pass, const String& willTopic, uint8_t willQos, bool willRetain, const String& willMessage, bool cleanSession);
		void disconnect();

		bool subscribe(const stl::String& topic, QoS qos = QOS0);
		bool unsubscribe(const stl::String& topic);

		bool loop();
		bool connected();

		bool publish(const stl::String& topic, const ByteBuffer& data, bool retained = false);
		bool publish(const stl::String& topic, const stl::String& data, bool retained = false);

		constexpr int state() const
		{
			return this->_state;
		}

		static constexpr int MQTT_MAX_PACKET_SIZE = 512;
		static constexpr int MQTT_MAX_HEADER_SIZE =   5;
		static constexpr int MQTT_SOCKET_TIMEOUT  =  15;

	private:
		uint16_t _port;
		IPAddress _addr;
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
	};
}
