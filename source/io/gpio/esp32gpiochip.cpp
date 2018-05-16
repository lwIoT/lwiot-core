/*
 * GPIO implementation for the ESP32.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/gpiochip.h>
#include <lwiot/log.h>
#include <lwiot/esp32gpiochip.h>

#include <driver/gpio.h>

#ifdef PINS
#undef PINS
#endif

#define PINS 40

#ifndef HIGH
#define HIGH 1
#define LOW  0
#endif

namespace lwiot
{
	Esp32GpioChip::Esp32GpioChip() : GpioChip(PINS)
	{ }

	void Esp32GpioChip::mode(int pin, const PinMode& mode)
	{
		gpio_num_t _pin = (gpio_num_t)pin;

		switch(mode) {
		case INPUT_PULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_en(_pin);
			break;

		case INPUT_NOPULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_dis(_pin);
			break;

		case INPUT:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		case OUTPUT:
			gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
			gpio_pullup_dis(_pin);
			gpio_pulldown_dis(_pin);
			break;

		case OUTPUT_OPEN_DRAIN:
			gpio_set_direction(_pin, GPIO_MODE_INPUT_OUTPUT_OD);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		default:
			print_dbg("Attempting to set invalid GPIO mode!\n");
			break;
		}
	}

	void Esp32GpioChip::write(int pin, bool value)
	{
		auto level = value ? HIGH : LOW;
		gpio_set_level((gpio_num_t)pin, level);
	}

	bool Esp32GpioChip::read(int pin)
	{
		return gpio_get_level((gpio_num_t)pin) == 1U;
	}

	void Esp32GpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
		this->write(pin, true);
	}

	void Esp32GpioChip::odWrite(int pin, bool value)
	{
		this->write(pin, value);
	}
}

static lwiot::Esp32GpioChip esp_gpio;
lwiot::GpioChip& gpio = esp_gpio;
