/*
 * ESP8266 analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <esp8266.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>
#include <lwiot/esp8266/esp8266adcchip.h>

#include <espressif/esp_system.h>

namespace lwiot { namespace esp8266
{
	AdcChip::AdcChip() : lwiot::AdcChip(1, 3300, 1024)
	{ }

	void AdcChip::begin()
	{ }

	size_t AdcChip::read(int pin) const
	{
		if(pin != 0)
			return 0;

		auto value = sdk_system_adc_read();
		return lwiot::AdcChip::toVoltage(value);		
	}
}
}

static lwiot::esp8266::AdcChip _espadc;
lwiot::AdcChip& adc = _espadc;
