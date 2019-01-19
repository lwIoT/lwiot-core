/*
 * ESP8266 analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>

#ifdef CXX
namespace lwiot { namespace esp8266
{
	class AdcChip : public lwiot::AdcChip {
	public:
		explicit AdcChip();
		virtual ~AdcChip() = default;

		void begin() override;
		size_t read(int pin) const override;
	};
}
}
#endif
