/*
 * ADC implementation for the ESP32.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>

#include <lwiot/lwiot.h>
#include <lwiot/adcchip.h>
#include <lwiot/esp32adcchip.h>

#include <driver/adc.h>

namespace lwiot
{
	Esp32AdcChip::Esp32AdcChip() : AdcChip(8, 3300, 4095)
	{
	}

	void Esp32AdcChip::begin()
	{
		adc_power_on();
		adc1_config_width(ADC_WIDTH_12Bit);

		for(int i = 0; i < ADC1_CHANNEL_MAX; i++) {
			adc1_config_channel_atten((adc1_channel_t)i, ADC_ATTEN_11db);
		 }
	}

	size_t Esp32AdcChip::read(int pin) const
	{
		int raw;

		raw = adc1_get_raw((adc1_channel_t)pin);
		if(raw < 0)
			return 0;

		return AdcChip::toVoltage(raw);
	}
}

static lwiot::Esp32AdcChip _espadc;
lwiot::AdcChip& adc = _espadc;
