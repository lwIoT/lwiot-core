/*
 * Hosted (Windows / Unix) GPIO chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/gpiochip.h>
#include <lwiot/pair.h>
#include <lwiot/hostedgpiochip.h>

#include <vector>
#include <map>

#ifdef _DEBUG
#undef _DEBUG
#include <matplotlibcpp.h>
#define _DEBUG 1
#else
#include <matplotlibcpp.h>
#endif

namespace plt = matplotlibcpp;

namespace lwiot
{
	HostedGpioChip::HostedGpioChip(int pins ) : GpioChip(pins)
	{ }

	void HostedGpioChip::write(int pin, bool value)
	{
		auto& values = this->_pindata[pin];
		auto stamp = lwiot_tick();

		Pair<time_t, bool> p(stamp, value);
		values.push_back(p);
	}

	void HostedGpioChip::odWrite(int pin, bool value)
	{
		this->write(pin, value);
	}

	bool HostedGpioChip::read(int pin)
	{
		return this->pop(pin);
	}

	bool HostedGpioChip::pop(int pin)
	{
		auto& data = this->_pindata.at(pin);

		assert(!data.empty());
		auto retval = data.at(0);
		data.erase(data.begin());

		return retval.second();
	}

	void HostedGpioChip::mode(int pin, const PinMode& mode)
	{
	}

	void HostedGpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
	}

	void HostedGpioChip::plot(const String& output, bool dialog, int pin1, int pin2) const
	{
		std::vector<double> y1, y2;
		std::vector<double> x1, x2;

		this->split(pin1, 0, x1, y1);
		this->split(pin2, 1.4f, x2, y2);

		plt::step(x1, y1);

		if(!x2.empty() && !y2.empty())
			plt::step(x2, y2, "r--");

		if(dialog)
			plt::show();

		plt::save(output.c_str());
	}

	void HostedGpioChip::attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge)
	{
	}

	void HostedGpioChip::split(int pin, double yoff, std::vector<double>& x, std::vector<double>& y) const
	{
		double first_entry;

		if(pin < 0)
			return;

		auto data = this->_pindata.at(pin);
		assert(x.empty());
		assert(y.empty());

		first_entry = static_cast<double>(data[0].first());
		x.push_back(-500);
		y.push_back(data.front().second() + yoff);

		for(auto& entry : data) {
			auto rel = entry.first() - first_entry;
			x.push_back(rel);
			y.push_back(entry.second() + yoff);
		}
	}
}

static lwiot::HostedGpioChip hosted_gpio(10);
lwiot::GpioChip& gpio = hosted_gpio;
