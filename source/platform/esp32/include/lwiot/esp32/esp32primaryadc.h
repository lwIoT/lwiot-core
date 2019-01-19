/*
 * ADC chip for the ESP32 SoC.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>

#ifdef CXX

namespace lwiot { namespace esp32
{
	class PrimaryAdc : lwiot::AdcChip {
	public:
		explicit PrimaryAdc();
		virtual ~PrimaryAdc() = default;

		void begin();
		size_t read(int pin) const override;
	};
}
}

#endif
