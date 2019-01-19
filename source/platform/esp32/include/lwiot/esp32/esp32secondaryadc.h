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
	class SecondaryAdc : lwiot::AdcChip {
	public:
		explicit SecondaryAdc();
		virtual ~SecondaryAdc() = default;

		void begin();
		size_t read(int pin) const override;
	};
}
}
#endif
