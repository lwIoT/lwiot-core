/*
 * Generic digital to analog converter class.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#ifdef CXX
namespace lwiot
{
	class DacChip {
	public:
		virtual ~DacChip() = default;

		virtual void begin(bool zeropin) final;
		virtual void begin();

		virtual void enable(int pin) = 0;
		virtual void disable(int pin) = 0;

		virtual const int& pins() const;
		virtual uint8_t width() const;
		virtual uint16_t vref() const;

		virtual void write(int pin, const size_t& voltage) = 0;

	protected:
		explicit DacChip() = delete;
		explicit DacChip(int pins, uint8_t width, uint16_t vref);
		virtual size_t map(const size_t& voltage) const;

		bool _zero_pin;

	private:
		int _pins;
		uint8_t _width;
		uint16_t _vref;
	};
}

extern lwiot::DacChip& dac;
#endif
