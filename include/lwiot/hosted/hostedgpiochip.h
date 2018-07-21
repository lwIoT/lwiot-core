/*
 * Hosted (Windows / Unix) GPIO chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <vector>
#include <map>

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/gpiochip.h>
#include <lwiot/error.h>
#include <lwiot/pair.h>
#include <lwiot/string.h>

namespace lwiot
{
	class HostedGpioChip : public GpioChip {
	public:
		explicit HostedGpioChip(int pins);
		virtual ~HostedGpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) const override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;

		void plot(const String& output, bool dialog, int pin1, int pin2 = -1) const;

	private:
		mutable std::map<double, std::vector<Pair<time_t, bool>>> _pindata;

		/* Methods */
		bool pop(int pin) const;
		void split(int pin, double yoff, std::vector<double>& x, std::vector<double>& y) const;
	};
}
