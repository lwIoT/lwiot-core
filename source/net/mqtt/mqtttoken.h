/*
 * MQTT token header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/event.h>

namespace lwiot
{
	class MqttTokenBucket;

	struct MqttToken {
	public:
		explicit MqttToken(Event& event, uint16_t id, MqttTokenBucket& bucket);
		void clear();

		bool operator==(const MqttToken& rhs) const;
		bool operator!=(const MqttToken& rhs) const;

		Event& event;
		uint16_t id;

	private:
		MqttTokenBucket& _bucket;
	};
}
