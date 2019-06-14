/*
 * SoC system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/log.h>
#include <lwiot/lwiot.h>
#include <lwiot/system.h>

namespace lwiot
{
	System::System(SleepState state) : _state(state)
	{ }

	const SleepState& System::sleepState() const
	{
		return this->_state;
	}

	void System::setSleepSate(const SleepState& state)
	{
		this->_state = state;
	}

	void System::delay(int ms)
	{
		lwiot_sleep(ms);
	}
}
