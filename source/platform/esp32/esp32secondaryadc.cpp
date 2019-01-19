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
#include <lwiot/esp32/esp32secondaryadc.h>

#include <driver/adc.h>

namespace lwiot { namespace esp32
{
	SecondaryAdc::SecondaryAdc() : lwiot::AdcChip(10, 3300, 4095)
	{
	}

	void SecondaryAdc::begin()
	{
		for(int i = 0; i < ADC2_CHANNEL_MAX; i++) {
			adc2_config_channel_atten((adc2_channel_t)i, ADC_ATTEN_11db);
		 }
	}

	size_t SecondaryAdc::read(int pin) const
	{
		int readvalue;

		auto status = adc2_get_raw((adc2_channel_t)pin, ADC_WIDTH_12Bit, &readvalue);
		if(status != ESP_OK)
			return 0U;

		return AdcChip::toVoltage(readvalue);
	}
}
}