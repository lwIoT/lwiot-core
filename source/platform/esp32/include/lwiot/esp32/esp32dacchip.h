/*
 * ESP32 digital to analog converter class.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/io/dacchip.h>

#include <driver/gpio.h>
#include <driver/dac.h>

namespace lwiot { namespace esp32
{
	class DacChip : public lwiot::DacChip {
	public:
		explicit DacChip();
		virtual ~DacChip() = default;

		virtual void enable(int pin) override;
		virtual void disable(int pin) override;
		void begin() override;

		void write(int pin, const size_t& voltage) override;
	};
}
}
