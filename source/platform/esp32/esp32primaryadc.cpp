/*
 * ADC chip for the ESP32 SoC.
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
#include <lwiot/esp32/esp32primaryadc.h>

#include <driver/adc.h>

namespace lwiot { namespace esp32
{
	PrimaryAdc::PrimaryAdc() : lwiot::AdcChip(8, 3300, 4095)
	{
	}

	void PrimaryAdc::begin()
	{
		adc1_config_width(ADC_WIDTH_12Bit);

		for(int i = 0; i < ADC1_CHANNEL_MAX; i++) {
			adc1_config_channel_atten((adc1_channel_t)i, ADC_ATTEN_11db);
		 }
	}

	size_t PrimaryAdc::read(int pin) const
	{
		int raw;

		raw = adc1_get_raw((adc1_channel_t)pin);
		if(raw < 0)
			return 0;

		return AdcChip::toVoltage(raw);
	}
}
}
