/*
 * MQTT client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/stream.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/mqttclient.h>

#include <lwiot/stl/referencewrapper.h>
#include <lwiot/kernel/thread.h>

#include "mqtt.h"

/*
 * Derrived from the PubSubClient implementation of the MQTT protocol. Due to licensing issue's, this
 * file is licenced under the MIT license instead of the Apache License. License text:
 *
 * Copyright (c) 2008-2015 Nicholas O'Leary
 * Copyright (c) 2019 Michel Megens
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define CHECK_STRING_LENGTH(l, s) if (l+2+s.length() > MQTT_MAX_PACKET_SIZE) {_io->close();return false;}
#define MQTT_TIMEOUT 2

namespace lwiot
{
	MqttClient::MqttClient() : _stream(nullptr), _state(MQTT_DISCONNECTED), _buffer(MQTT_MAX_PACKET_SIZE, true)
	{
	}

	void MqttClient::begin(lwiot::TcpClient &client)
	{
		this->setClient(client);
	}

	void MqttClient::setClient(lwiot::TcpClient &client)
	{
		if(this->_io) {
			this->disconnect();
		}

		this->_io = client;
		this->_io->setTimeout(MQTT_TIMEOUT);
	}

	bool MqttClient::reconnect()
	{
		auto ip = this->_io->remote();
		auto port = this->_io->port();

		print_dbg("Reconnecting to: %s:%u\n", this->_io->remote().toString().c_str(), this->_io->port());
		auto rv = this->_io->connect(ip, port);
		return rv;
	}

	bool MqttClient::publish(const lwiot::String &topic, const lwiot::String &data, bool retained)
	{
		ByteBuffer buf(data.length());

		buf.write(data.c_str(), data.length());
		return this->publish(topic, buf, retained);
	}

	bool MqttClient::publish(const lwiot::String &topic, const lwiot::ByteBuffer &data, bool retained)
	{
		auto plength = data.count();
		auto rv = false;
		uint8_t header = MQTTPUBLISH;

		if(this->isConnected()) {
			if(MQTT_MAX_PACKET_SIZE < MQTT_MAX_HEADER_SIZE + 2 + topic.length() + plength)
				return false;

			this->_buffer.reset();

			// Leave room in the buffer for header and variable length field
			uint16_t length = MQTT_MAX_HEADER_SIZE;
			length = this->write(topic, length);

			for(auto i = 0; i < plength; i++) {
				this->_buffer[length++] = data[i];
			}

			if(retained)
				header |= 1;

			rv = this->write(header, length - MQTT_MAX_HEADER_SIZE);
		}

		return rv;
	}

	bool MqttClient::subscribe(const lwiot::String &topic, lwiot::MqttClient::QoS qos)
	{
		auto rv = false;

		if(qos > 1)

			if(MQTT_MAX_PACKET_SIZE < 9 + topic.length())
				return rv;

		if(this->isConnected()) {
			this->_buffer.reset();

			uint16_t length = MQTT_MAX_HEADER_SIZE;
			this->_nextMsgId++;

			if(this->_nextMsgId == 0)
				this->_nextMsgId = 1;

			this->_buffer[length++] = (this->_nextMsgId >> 8);
			this->_buffer[length++] = (this->_nextMsgId & 0xFF);
			length = this->write(topic, length);
			this->_buffer[length++] = qos;

			rv = write(MQTTSUBSCRIBE | MQTTQOS1, length - MQTT_MAX_HEADER_SIZE);
		}

		return rv;
	}

	bool MqttClient::unsubscribe(const lwiot::String &topic)
	{
		auto rv = false;

		if(MQTT_MAX_PACKET_SIZE < 9 + topic.length())
			return rv;

		if(isConnected()) {
			uint16_t length = MQTT_MAX_HEADER_SIZE;
			this->_nextMsgId++;

			if(this->_nextMsgId == 0)
				this->_nextMsgId = 1;

			this->_buffer[length++] = (this->_nextMsgId >> 8);
			this->_buffer[length++] = (this->_nextMsgId & 0xFF);
			length = this->write(topic, length);
			rv = write(MQTTUNSUBSCRIBE | MQTTQOS1, length - MQTT_MAX_HEADER_SIZE);
		}

		return rv;
	}

	uint16_t MqttClient::write(const lwiot::String &data, uint16_t pos)
	{
		const char *idp = data.c_str();
		uint16_t len = data.length();

		pos += 2;

		for(int idx = 0; idx < len; idx++) {
			this->_buffer[pos++] = *idp++;
		}

		this->_buffer[pos - len - 2] = (len >> 8);
		this->_buffer[pos - len - 1] = (len & 0xFF);

		return pos;
	}

	bool MqttClient::write(uint8_t header, size_t length)
	{
		uint16_t rc;
		uint8_t hlen = this->build(header, length);

#ifdef MQTT_MAX_TRANSFER_SIZE
		uint8_t *writeBuf = this->_buffer.data() + (MQTT_MAX_HEADER_SIZE - hlen);
		uint16_t bytesRemaining = length + hlen;  //Match the length type
		uint8_t bytesToWrite;
		bool result = true;

		while((bytesRemaining > 0) && result) {
			bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE) ? MQTT_MAX_TRANSFER_SIZE : bytesRemaining;
			rc = this->_io->write(writeBuf, bytesToWrite);
			result = (rc == bytesToWrite);
			bytesRemaining -= rc;
			writeBuf += rc;
		}

		return result;
#else
		rc = this->_io->write(this->_buffer.data() + (MQTT_MAX_HEADER_SIZE - hlen), length + hlen);
		this->_lastOutActivity = lwiot_tick_ms();

		return (rc == hlen + length);
#endif
	}

	size_t MqttClient::write(uint8_t data)
	{
		this->_lastOutActivity = lwiot_tick_ms();
		return this->_io->write(data);
	}

	bool MqttClient::read(uint8_t *result)
	{
		uint32_t currentMillis;
		uint32_t previousMillis = lwiot_tick_ms();

		while(!this->_io->available()) {
			Thread::yield();
			currentMillis = lwiot_tick_ms();

			if(currentMillis - previousMillis >= ((int32_t) MQTT_SOCKET_TIMEOUT * 1000))
				return false;
		}

		*result = this->_io->read();
		return true;
	}

	bool MqttClient::read(uint8_t *result, uint16_t *index)
	{
		uint16_t current_index = *index;
		uint8_t *write_address = &result[current_index];

		if(this->read(write_address)) {
			*index = current_index + 1;
			return true;
		}
		return false;
	}

	uint16_t MqttClient::readPacket(uint8_t *data)
	{
		uint16_t len = 0;
		auto buffer = this->_buffer.data();
		bool isPublish;
		uint32_t multiplier = 1;
		uint16_t length = 0;
		uint8_t digit = 0;
		uint16_t skip = 0;
		uint8_t start = 0;

		this->_buffer.reset();

		if(!this->read(buffer, &len))
			return 0;

		isPublish = (buffer[0] & 0xF0) == MQTTPUBLISH;

		do {
			if(len == 5) {
				// Invalid remaining length encoding - kill the connection
				_state = MQTT_DISCONNECTED;
				this->_io->close();
				return 0;
			}

			if(!this->read(&digit))
				return 0;

			buffer[len++] = digit;
			length += (digit & 127) * multiplier;
			multiplier *= 128;
		} while((digit & 128) != 0);
		*data = len - 1;

		if(isPublish) {
			// Read in topic length to calculate bytes to skip over for Stream writing
			if(!this->read(buffer, &len))
				return 0;
			if(!this->read(buffer, &len))
				return 0;
			skip = (buffer[*data + 1] << 8) + buffer[*data + 2];
			start = 2;

			if(buffer[0] & MQTTQOS1) {
				// skip message id
				skip += 2;
			}
		}

		for(uint16_t i = start; i < length; i++) {
			if(!this->read(&digit))
				return 0;
			if(this->_stream) {
				if(isPublish && len - *data - 2 > skip) {
					this->_stream->write(digit);
				}
			}
			if(len < MQTT_MAX_PACKET_SIZE) {
				buffer[len] = digit;
			}
			len++;
		}

		if(!this->_stream && len > MQTT_MAX_PACKET_SIZE) {
			len = 0;
		}

		return len;
	}

	bool MqttClient::connect(const lwiot::String &id, const lwiot::String &user, const lwiot::String &pass)
	{
		return this->connect(id, user, pass, "", QOS0, false, "", true);
	}

	bool MqttClient::connect(const lwiot::String &id, const lwiot::String &user, const lwiot::String &pass,
	                         const lwiot::String &willTopic, uint8_t willQos, bool willRetain,
	                         const lwiot::String &willMessage, bool cleanSession)
	{
		if(!this->isConnected()) {

			bool result = this->_io->connected();

			if(!result)
				result = this->_io->connect(this->_io->remote(), this->_io->port());

			if(result) {
				uint16_t length = MQTT_MAX_HEADER_SIZE;
				unsigned int j;

				this->_buffer.reset();
				this->_nextMsgId = 1;

#if MQTT_VERSION == MQTT_VERSION_3_1
				uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
				uint8_t d[7] = {0x00, 0x04, 'M', 'Q', 'T', 'T', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
				for(j = 0; j < MQTT_HEADER_VERSION_LENGTH; j++) {
					this->_buffer[length++] = d[j];
				}

				uint8_t v;

				if(willTopic.length() > 0)
					v = 0x04 | (willQos << 3) | (willRetain << 5);
				else
					v = 0x00;

				if(cleanSession)
					v = v | 0x02;

				if(user.length() != 0) {
					v = v | 0x80;

					if(pass.length() != 0)
						v = v | (0x80 >> 1);
				}

				this->_buffer[length++] = v;

				this->_buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
				this->_buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);

				CHECK_STRING_LENGTH(length, id)
				length = this->write(id, length);
				if(willTopic.length() > 0) {
					CHECK_STRING_LENGTH(length, willTopic)
					length = this->write(willTopic, length);
					CHECK_STRING_LENGTH(length, willMessage)
					length = this->write(willMessage, length);
				}

				if(user.length() != 0) {
					CHECK_STRING_LENGTH(length, user)
					length = this->write(user, length);
					if(pass.length() != 0) {
						CHECK_STRING_LENGTH(length, pass)
						length = this->write(pass, length);
					}
				}

				this->write(MQTTCONNECT, length - MQTT_MAX_HEADER_SIZE);

				this->_lastInActivity = this->_lastOutActivity = lwiot_tick_ms();

				while(!this->_io->available()) {
					unsigned long t = lwiot_tick_ms();
					if(t - this->_lastInActivity >= ((int32_t) MQTT_SOCKET_TIMEOUT * 1000UL)) {
						_state = MQTT_CONNECTION_TIMEOUT;
						this->_io->close();
						return false;
					}
				}

				uint8_t llen;
				uint16_t len = this->readPacket(&llen);

				if(len == 4) {
					if(this->_buffer[3] == 0) {
						this->_lastInActivity = lwiot_tick_ms();
						this->_pingOutstanding = false;
						_state = MQTT_CONNECTED;
						return true;
					} else {
						_state = this->_buffer[3];
					}
				}

				this->_io->close();
			} else {
				_state = MQTT_CONNECT_FAILED;
			}

			return false;
		}

		return true;
	}

	size_t MqttClient::build(uint8_t header, uint16_t length) const
	{
		uint8_t lengthbuf[4];
		uint8_t digit;
		uint8_t pos = 0, llen = 0;
		uint16_t l = length;

		do {
			digit = l % 128;
			l = l / 128;

			if(l > 0)
				digit |= 0x80;

			lengthbuf[pos++] = digit;
			llen++;
		} while(l > 0);

		this->_buffer[4 - llen] = header;

		for(int i = 0; i < llen; i++) {
			this->_buffer[MQTT_MAX_HEADER_SIZE - llen + i] = lengthbuf[i];
		}

		return llen + 1;
	}

	bool MqttClient::loop()
	{
		unsigned long t = lwiot_tick_ms();

		if(!this->isConnected())
			return false;

		if((t - this->_lastInActivity > MQTT_KEEPALIVE * 1000UL) ||
		   (t - this->_lastOutActivity > MQTT_KEEPALIVE * 1000UL)) {
			if(this->_pingOutstanding) {
				this->_state = MQTT_CONNECTION_TIMEOUT;
				this->_io->close();

				return false;
			} else {
				this->_buffer[0] = MQTTPINGREQ;
				this->_buffer[1] = 0;
				this->_io->write(this->_buffer.data(), 2);
				this->_lastOutActivity = t;
				this->_lastInActivity = t;
				this->_pingOutstanding = true;
			}
		}

		if(this->_io->available()) {
			uint8_t llen;
			uint16_t len = readPacket(&llen);
			uint16_t msgId = 0;
			uint8_t *payload;

			if(len > 0) {
				this->_lastInActivity = t;
				uint8_t type = this->_buffer[0] & 0xF0;
				if(type == MQTTPUBLISH) {
					if(this->_cb) {
						uint16_t tl = (this->_buffer[llen + 1] << 8) + this->_buffer[llen + 2];

						memmove(this->_buffer.data() + llen + 2, this->_buffer.data() + llen + 3, tl);
						this->_buffer[llen + 2 + tl] = 0;
						char *topic = (char *) this->_buffer.data() + llen + 2;

						if((this->_buffer[0] & 0x06) == MQTTQOS1) {
							msgId = (this->_buffer[llen + 3 + tl] << 8) + this->_buffer[llen + 3 + tl + 1];
							payload = this->_buffer.data() + llen + 3 + tl + 2;
							ByteBuffer buf(len - llen - 3 - tl - 2);
							buf.write(payload, len - llen - 3 - tl - 2);

							this->_cb(topic, buf);

							this->_buffer[0] = MQTTPUBACK;
							this->_buffer[1] = 2;
							this->_buffer[2] = (msgId >> 8);
							this->_buffer[3] = (msgId & 0xFF);
							this->_io->write(this->_buffer.data(), 4);
							this->_lastOutActivity = t;

						} else {
							payload = this->_buffer.data() + llen + 3 + tl;
							ByteBuffer buf(len - llen - 3 - tl);
							buf.write(payload, len - llen - 3 - tl);
							this->_cb(topic, buf);
						}
					}
				} else if(type == MQTTPINGREQ) {
					this->_buffer[0] = MQTTPINGRESP;
					this->_buffer[1] = 0;
					this->_io->write(this->_buffer.data(), 2);
				} else if(type == MQTTPINGRESP) {
					this->_pingOutstanding = false;
				}
			} else if(!this->isConnected()) {
				return false;
			}
		}

		return true;
	}

	void MqttClient::disconnect()
	{
		uint8_t buf[] = {MQTTDISCONNECT, 0};

		this->_buffer.reset();
		this->_io->write(buf, 2);
		this->_state = MQTT_DISCONNECTED;
		this->_io->close();
	}

	bool MqttClient::isConnected()
	{
		bool rc;

		rc = this->_io->connected();

		if(!rc && this->_state == MQTT_CONNECTED) {
			this->_state = MQTT_CONNECTION_LOST;
			this->_io->close();
		}

		return rc && this->_state == MQTT_CONNECTED;
	}
}
