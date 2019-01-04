/*
 * MQTT token bucket implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/linkedlist.h>

#include "mqtttoken.h"
#include "mqtttokenbucket.h"

namespace lwiot
{
	MqttTokenBucket::MqttTokenBucket() : _tokens()
	{
	}

	MqttToken MqttTokenBucket::create(lwiot::Event &event, uint16_t id)
	{
		MqttToken token(event, id, *this);

		this->_tokens.push_front(token);
		return token;
	}

	MqttToken MqttTokenBucket::front() const
	{
		return this->_tokens.front();
	}

	bool MqttTokenBucket::contains(uint16_t id) const
	{
		for(const auto& token : this->_tokens) {
			if(token.id == id) {
				return true;
			}
		}

		return false;
	}

	bool MqttTokenBucket::contains(const MqttToken& id) const
	{
		for(const auto& token : this->_tokens) {
			if(unlikely(token == id)) {
				return true;
			}
		}

		return false;
	}

	void MqttTokenBucket::remove(const lwiot::MqttToken &token)
	{
		this->remove(token.id);
	}

	void MqttTokenBucket::remove(uint16_t id)
	{
		auto end = this->_tokens.end();

		for(auto iter = this->_tokens.begin(); iter != end; ++iter) {
			if(iter->id == id) {
				this->_tokens.remove(iter.node());
				break;
			}
		}
	}
}
