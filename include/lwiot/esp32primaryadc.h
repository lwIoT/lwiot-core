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
	class Esp32PrimaryAdc : AdcChip {
	public:
		explicit Esp32PrimaryAdc();
		virtual ~Esp32PrimaryAdc() = default;

		void begin();
		size_t read(int pin) const override;
	};
}

#endif
