/*
 * MQTT token bucket header.
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
#include <lwiot/linkedlist.h>

#include "mqtttoken.h"

namespace lwiot
{
	class MqttTokenBucket {
	public:
		typedef LinkedList<MqttToken>::Iterator iterator;
		typedef MqttToken token_type;

		explicit MqttTokenBucket();

		MqttToken create(Event& event, uint16_t id);
		MqttToken front() const;

		bool contains(uint16_t id) const;
		bool contains(const token_type& token) const;
		void remove(const token_type& token);
		void remove(uint16_t id);

	private:
		LinkedList<MqttToken> _tokens;
	};
}
