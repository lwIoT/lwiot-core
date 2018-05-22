/*
 * ESP8266 analog to digital converter.
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
	class Esp8266AdcChip : public AdcChip {
	public:
		explicit Esp8266AdcChip();
		virtual ~Esp8266AdcChip() = default;

		void begin() override;
		size_t read(int pin) const override;
	};
}
#endif
