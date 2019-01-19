/*
 * ADC implementation for the ESP32.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <esp_system.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>
#include <lwiot/esp32/esp32adcchip.h>

#include <driver/adc.h>

#define ADC1_PINS 8
#define ADC2_START_PIN ADC1_PINS

namespace lwiot { namespace esp32
{
	AdcChip::AdcChip() : lwiot::AdcChip(18, 3300, 4095)
	{
	}

	void AdcChip::begin()
	{
		adc_power_on();
		this->primary.begin();
		this->secondary.begin();
	}

	size_t AdcChip::read(int pin) const
	{
		if(pin >= this->pins())
			return 0U;

		if(pin >= ADC2_START_PIN)
			return this->secondary.read(pin - ADC2_START_PIN);

		return this->primary.read(pin);
	}
}
}

static lwiot::esp32::AdcChip _espadc;
lwiot::AdcChip& adc = _espadc;
