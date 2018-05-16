/*
 * AVR ATmega GPIO implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/gpiochip.h>
#include <lwiot/avrgpiochip.h>

extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];
extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

namespace lwiot
{
	AvrGpioChip::AvrGpioChip() : GpioChip(PINS)
	{ }

	void AvrGpioChip::mode(int pin, const PinMode& mode)
	{
		uint8_t bit, port;
		uint8_t old;
		volatile uint8_t *reg, *out;

		bit = digitalPinToBitMask(pin);
		port = digitalPinToPort(pin);

		if(port == NOT_A_PIN)
			return;

		reg = portModeRegister(port);
		out = portOutputRegister(port);

		old = SREG;
		cli();

		switch(mode) {
		case INPUT:
			*reg &= ~bit;
			*out &= ~bit;
			break;

		case INPUT_NOPULLUP:
			*reg &= ~bit;
			*out &= ~bit;
			break;

		case INPUT_PULLUP:
			*reg &= ~bit;
			*out |= bit;
			break;

		case OUTPUT_OPEN_DRAIN:
			*out &= ~bit;
		case OUTPUT:
			*reg |= bit;
			break;

		default:
			break;
		}

		SREG = old;
	}

	void AvrGpioChip::write(int pin, bool value)
	{
		uint8_t bit, port;
		uint8_t old;
		volatile uint8_t *out;

		bit = digitalPinToBitMask(pin);
		port = digitalPinToPort(pin);

		if(port == NOT_A_PIN)
			return;

		out = portOutputRegister(port);

		old = SREG;
		cli();

		if(value)
			*out &= ~bit;
		else
			*out |= bit;

		SREG = old;
	}

	bool AvrGpioChip::read(int pin)
	{
		uint8_t bit, port;
		uint8_t old;
		volatile uint8_t *in;

		bit = digitalPinToBitMask(pin);
		port = digitalPinToPort(pin);

		if(port == NOT_A_PIN)
			return false;

		in = portInputRegister(port);

		old = SREG;
		cli();
		auto rv = *in & bit;
		SREG = old;

		return !!rv;
	}

	void AvrGpioChip::setOpenDrain(int pin)
	{
		this->input(pin);
		this->write(pin, false);
	}

	void AvrGpioChip::odWrite(int pin, bool value)
	{
		if(value)
			this->input(pin);
		else
			this->output(pin);
	}
}

static lwiot::AvrGpioChip avr_gpio;
lwiot::GpioChip& gpio = avr_gpio;
