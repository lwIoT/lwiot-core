/*
 * AVR analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>

namespace lwiot { namespace avr
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
