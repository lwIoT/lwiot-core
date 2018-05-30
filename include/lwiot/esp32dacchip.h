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

#include <lwiot/dacchip.h>

namespace lwiot
{
	class Esp32DacChip : public DacChip {
	public:
		explicit Esp32DacChip();
		virtual ~Esp32DacChip() = default;

		virtual void enable(int pin) override;
		virtual void disable(int pin) override;
		void begin() override;

		void write(int pin, const size_t& voltage) override;
	};
}
