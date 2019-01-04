/*
 * MQTT token implementation.
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
	MqttToken::MqttToken(lwiot::Event &event, uint16_t id, lwiot::MqttTokenBucket &bucket) :
			event(event), id(id), _bucket(bucket)
	{ }

	void MqttToken::clear()
	{
		this->_bucket.remove(this->id);
	}

	bool MqttToken::operator==(const lwiot::MqttToken &rhs) const
	{
		return this->id == rhs.id;
	}

	bool MqttToken::operator!=(const lwiot::MqttToken &rhs) const
	{
		return !(*this == rhs);
	}
}
