/*
 * ADC chip for the ESP32 SoC.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/adcchip.h>

#ifdef CXX
namespace lwiot
{
	class Esp32SecondaryAdc : AdcChip {
	public:
		explicit Esp32SecondaryAdc();
		virtual ~Esp32SecondaryAdc() = default;

		void begin();
		size_t read(int pin) const override;
	};
}
#endif
