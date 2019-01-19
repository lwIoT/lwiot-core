/*
 * GPIO chip for the ESP32 chip.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>

#include <lwiot/esp32/esp32primaryadc.h>
#include <lwiot/esp32/esp32secondaryadc.h>

#ifdef CXX
namespace lwiot { namespace esp32
{
	class AdcChip : public lwiot::AdcChip {
	public:
		explicit AdcChip();
		virtual ~AdcChip() = default;

		void begin() override;
		size_t read(int pin) const override;

	private:
		PrimaryAdc primary;
		SecondaryAdc secondary;
	};
}
}
#endif
