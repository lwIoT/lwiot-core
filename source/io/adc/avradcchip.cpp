/*
 * AVR analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/avr/avradcchip.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

#ifdef ARDUINO_MEGA
#define ADC_PINS 16
#elif defined(ARDUINO_UNO)
#define ADC_PINS 6
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

namespace lwiot { namespace avr
{
	AdcChip::AdcChip() : lwiot::AdcChip(ADC_PINS, 5000, 1023)
	{ }

	void AdcChip::begin()
	{
#if defined(ADCSRA)
	#if F_CPU >= 16000000
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 8000000
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 4000000
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 2000000
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 1000000
		cbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#else
		cbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#endif

	sbi(ADCSRA, ADEN);
#endif
	}

	size_t AdcChip::read(int pin) const
	{
		uint8_t high, low;
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
	pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		if (pin >= 54)
			pin -= 54;
#elif defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
		if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
		if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
		ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
		ADMUX = _BV(REFS0) | (pin & 0x07);
#endif
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);

#if defined(ADCSRA) && defined(ADCL)
		// start the conversion
		sbi(ADCSRA, ADSC);

		// ADSC is cleared when the conversion finishes
		while (bit_is_set(ADCSRA, ADSC));

		// we have to read ADCL first; doing so locks both ADCL
		// and ADCH until ADCH is read.  reading ADCL second would
		// cause the results of each conversion to be discarded,
		// as ADCL and ADCH would be locked when it completed.
		low  = ADCL;
		high = ADCH;
#else
		// we dont have an ADC, return 0
		low  = 0;
		high = 0;
#endif

		// combine the two bytes
		auto value = high << 8 | low;
		return this->toVoltage(value);
	}
}
}

static lwiot::avr::AdcChip _avradc;
lwiot::AdcChip& adc = _avradc;
