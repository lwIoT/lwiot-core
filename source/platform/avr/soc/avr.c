/*
 * AVR ATmega specific functions.
 *
 * @author Michel Megens
 * @author Arduino Community
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/avr/extmem.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

void __attribute__((optimize("Os")))  lwiot_udelay(uint32_t us)
{
#if F_CPU >= 24000000L
	if(!us)
		return;

	us *= 6;
	us -= 5;
#elif F_CPU >= 20000000L
	__asm__ __volatile__(
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop");
	if(us <= 1)
		return;

	us = (us << 2) + us;
	us -= 7;

#elif F_CPU >= 16000000L
	if(us <= 1)
		return;

	us <<= 2;
	us -= 5;

#elif F_CPU >= 12000000L
	if(us <= 1)
		return;

	us = (us << 1) + us;
	us -= 5;

#elif F_CPU >= 8000000L
	if(us <= 2)
		return;

	us <<= 1;
	us -= 4;

#else
	if(us <= 16)
		return;

	if(us <= 25)
		return;

	us -= 22;
	us >>= 2;
#endif

	__asm__ __volatile__(
		"1: sbiw %0,1" "\n\t"
		"brne 1b" : "=w" (us) : "0" (us)
	);
}

#define P0		0
#define P1		1
#define P2		2
#define P3		3
#define P4		4
#define P5		5
#define P6		6
#define P7	    7

#ifdef XMCRA
static void avr_sre_enable(char reg, char idx)
{
	switch(reg) {
#ifdef PORTA
	case 0:
		DDRA |= BIT(idx);
		PORTA &= ~BIT(idx);
		break;
#endif

	case 1:
		DDRB |= BIT(idx);
		PORTB &= ~BIT(idx);
		break;

	case 2:
		DDRC |= BIT(idx);
		PORTC &= ~BIT(idx);
		break;

	case 3:
		DDRD |= BIT(idx);
		PORTD &= ~BIT(idx);
		break;

#ifdef PORTE
	case 4:
		DDRE |= BIT(idx);
		PORTE &= ~BIT(idx);
		break;
#endif

#ifdef PORTF 
	case 5:
		DDRF |= BIT(idx);
		PORTF &= ~BIT(idx);
		break;
#endif

#ifdef PORTG
	case 6:
		DDRG |= BIT(idx);
		PORTG &= ~BIT(idx);
		break;
#endif

#ifdef PORTH
	case 7:
		DDRH |= BIT(idx);
		PORTH &= ~BIT(idx);
		break;
#endif

#ifdef PORTJ
	case 8:
		DDRJ |= BIT(idx);
		PORTJ &= ~BIT(idx);
		break;
#endif

#ifdef PORTK
	case 9:
		DDRK |= BIT(idx);
		PORTK &= ~BIT(idx);
		break;
#endif

#ifdef PORTL
	case 10:
		DDRL |= BIT(idx);
		PORTL &= ~BIT(idx);
		break;
#endif

	default:
		break;
	}
}

void extmem_enable(int pin)
{
	char idx;

	/* Enable the external memory interface */
	XMCRA = BIT(SRE);

	idx = pin % 8;
	pin = pin / 8;

	avr_sre_enable(pin, idx);

	/* Set bank selection pins to output */
	DDRL |= BIT(P5) | BIT(P6) | BIT(P7);
	/* Select bank 0 */
	PORTL &= ~(BIT(P5) | BIT(P6) | BIT(P7));
}
#endif
