/*
 * Digital to analog conversion pin
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/dacchip.h>

namespace lwiot
{
	class DacPin {
	public:
		DacPin(int pin);
		explicit DacPin(int pin, DacChip& chip);
		virtual ~DacPin() = default;

		void write(const size_t& value);

	private:
		DacChip& _chip;
		int _pin;
	};
}
