/*
 * GPIO chip for the ESP32 chip.
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
	class Esp32AdcChip : public AdcChip {
	public:
		explicit Esp32AdcChip();
		virtual ~Esp32AdcChip() = default;

		void begin() override;
		size_t read(int pin) const override;
	};
}
#endif
