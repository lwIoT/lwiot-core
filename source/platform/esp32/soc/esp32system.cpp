/*
 * SoC system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <soc/soc.h>

#include <lwiot/log.h>
#include <lwiot/lwiot.h>
#include <lwiot/system.h>

#include <lwiot/esp32/esp32system.h>


namespace lwiot { namespace esp32
{
	System::System(SleepState state) : lwiot::System(state)
	{
	}

	void System::sleep(const uint32_t& ms) const
	{
		uint64_t us;

		switch(this->sleepState()) {
		case SLEEP_DEEP:
		default:
			us = ms * 1000ULL;
			esp_deep_sleep(us);
			break;
		}
	}

	void System::restart() const
	{
		esp_restart();
	}
}
}
