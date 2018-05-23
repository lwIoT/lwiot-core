/*
 * AVR analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/adcchip.h>

namespace lwiot
{
	class AvrAdcChip : public AdcChip {
	public:
		explicit AvrAdcChip();
		virtual ~AvrAdcChip() = default;

		void begin() override;
		size_t read(int pin) const override;
	};
}
