/*
 * ESP8266 analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/adcchip.h>
#include <lwiot/esp8266/esp8266adcchip.h>

#include <espressif/esp_system.h>

namespace lwiot
{
	Esp8266AdcChip::Esp8266AdcChip() : AdcChip(1, 3300, 1024)
	{ }

	void Esp8266AdcChip::begin()
	{ }

	size_t Esp8266AdcChip::read(int pin) const
	{
		if(pin != 0)
			return 0;

		auto value = sdk_system_adc_read();
		return AdcChip::toVoltage(value);		
	}
}

static lwiot::Esp8266AdcChip _espadc;
lwiot::AdcChip& adc = _espadc;
