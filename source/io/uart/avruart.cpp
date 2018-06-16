/*
 * AVR UART implementation.
 * 
 * @author Michel Megens
 * @author arduino.cc
 * @email  dev@bietje.net
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/builtins.h>

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/log.h>
#include <lwiot/uart.h>
#include <lwiot/avrgpiochip.h>
#include <lwiot/avruart.h>
#include <lwiot/error.h>

#ifdef IDE
#undef pgm_read_dword
#define pgm_read_dword(x) 0UL
#endif

typedef struct _DELAY_TABLE {
  long baud;
  unsigned short rx_delay_centering;
  unsigned short rx_delay_intrabit;
  unsigned short rx_delay_stopbit;
  unsigned short tx_delay;
} DELAY_TABLE;

#if F_CPU == 16000000

static const DELAY_TABLE PROGMEM table[] = 
{
  /*  baud    rxcenter   rxintra    rxstop    tx     */
  { 115200,   1,         17,        17,       12,    },
  { 57600,    10,        37,        37,       33,    },
  { 38400,    25,        57,        57,       54,    },
  { 31250,    31,        70,        70,       68,    },
  { 28800,    34,        77,        77,       74,    },
  { 19200,    54,        117,       117,      114,   },
  { 14400,    74,        156,       156,      153,   },
  { 9600,     114,       236,       236,      233,   },
  { 4800,     233,       474,       474,      471,   },
  { 2400,     471,       950,       950,      947,   },
  { 1200,     947,       1902,      1902,     1899,  },
  { 600,      1902,      3804,      3804,     3800,  },
  { 300,      3804,      7617,      7617,     7614,  },
};

const int XMIT_START_ADJUSTMENT = 5;

#elif F_CPU == 8000000

static const DELAY_TABLE table[] PROGMEM = 
{
  /*  baud    rxcenter    rxintra    rxstop  tx      */
  { 115200,   1,          5,         5,      3,      },
  { 57600,    1,          15,        15,     13,     },
  { 38400,    2,          25,        26,     23,     },
  { 31250,    7,          32,        33,     29,     },
  { 28800,    11,         35,        35,     32,     },
  { 19200,    20,         55,        55,     52,     },
  { 14400,    30,         75,        75,     72,     },
  { 9600,     50,         114,       114,    112,    },
  { 4800,     110,        233,       233,    230,    },
  { 2400,     229,        472,       472,    469,    },
  { 1200,     467,        948,       948,    945,    },
  { 600,      948,        1895,      1895,   1890,   },
  { 300,      1895,       3805,      3805,   3802,   },
};

const int XMIT_START_ADJUSTMENT = 4;
#endif

namespace lwiot
{
	void isr_entry()
	{
		lwiot::AvrUart::handle_isr();
	}

	AvrUart::AvrUart(int rx, int tx, long baud) : Uart(tx, rx, baud, SERIAL_8N1)
	{
		auto _txport = digitalPinToPort(tx);
		auto _rxport = digitalPinToPort(rx);

		this->port = portOutputRegister(_txport);
		this->pin = portInputRegister(_rxport);
		this->rxbit = digitalPinToBitMask(rx);
		this->txbit = digitalPinToBitMask(tx);

		this->_rx.mode(INPUT_NOPULLUP);
		this->_tx.mode(OUTPUT);
		this->_tx.write(true);

		this->init();
	}

	AvrUart::~AvrUart()
	{
		this->_rx.mode(INPUT_NOPULLUP);
		this->_tx.mode(INPUT_NOPULLUP);

		/* disable interrupts */
		auto pcmsk = digitalPinToPCMSK(this->_rx.pin());
		if(!pcmsk)
			return;

		*pcmsk &= ~_BV(digitalPinToPCMSKbit(this->_rx.pin()));
	}

	void AvrUart::handle_isr()
	{
		if(active_object)
			active_object->recv();
	}

	void AvrUart::init()
	{
		this->_rx_delay_centering = this->_rx_delay_intrabit = this->_rx_delay_stopbit = this->_tx_delay = 0;

		for(unsigned idx = 0; idx < sizeof(table) / sizeof(table[0]); idx++) {
			long baud = pgm_read_dword(&table[0].baud);

			if(baud == this->_baud) {
				/*
				   Correct baudrate settings found, configure
				   bit delays
				*/

				this->_rx_delay_centering = pgm_read_word(&table[idx].rx_delay_centering);
				this->_rx_delay_intrabit = pgm_read_word(&table[idx].rx_delay_intrabit);
				this->_rx_delay_stopbit = pgm_read_word(&table[idx].rx_delay_stopbit);
				this->_tx_delay = pgm_read_word(&table[idx].tx_delay);
				break;
			}
		}

		if(this->_rx_delay_stopbit) {
			auto pcmsk = digitalPinToPCMSK(this->_rx.pin());
			auto pcicr = digitalPinToPCICR(this->_rx.pin());
			auto pin = digitalPinToPCMSKbit(this->_rx.pin());

			if(pcicr) {
				*pcmsk |= _BV(pin);
				*pcicr |= _BV(pin);
			}

			this->delay(this->_tx_delay);
		}

		this->listen();
	}

	inline void AvrUart::delay(uint16_t _delay) const
	{
		uint8_t tmp = 0;

		asm volatile("sbiw    %0, 0x01 \n\t"
			"ldi %1, 0xFF \n\t"
			"cpi %A0, 0xFF \n\t"
			"cpc %B0, %1 \n\t"
			"brne .-10 \n\t"
			: "+r" (_delay), "+a" (tmp)
			: "0" (_delay)
		);
	}

	bool AvrUart::listen()
	{
		if(AvrUart::active_object == this)
			return false;

		enter_critical();
		this->_buffer_overflow = false;
		this->_receive_buffer_head = this->_receive_buffer_tail = 0;
		AvrUart::active_object = this;
		exit_critical();

		return true;
	}

	bool AvrUart::isListening()
	{
		return this == active_object;
	}

	size_t AvrUart::available()
	{
		if(!this->isListening())
			return 0;

		return (_receive_buffer_tail + SS_RX_BUFFER_SIZE - _receive_buffer_head) % SS_RX_BUFFER_SIZE;
	}

	void AvrUart::recv()
	{
		uint8_t data;

#if GCC_VERSION < 40302
		/*
		   :: WORK AROUND ::
		   On GCC 4.3.0 (OSX) the compiler fails to push several
		   registers onto the stack.
		*/
		asm volatile(
		    "push r18 \n\t"
			"push r19 \n\t"
			"push r20 \n\t"
			"push r21 \n\t"
			"push r22 \n\t"
			"push r23 \n\t"
			"push r26 \n\t"
			"push r27 \n\t"
		);
#endif

		if(this->get_rx_value()) {
			this->delay(_rx_delay_centering);

			data = 0;
			for(uint8_t idx = 1; idx; idx <<= 1U) {
				this->delay(_rx_delay_intrabit);
				uint8_t notidx = ~idx;

				if(this->get_rx_value())
					data |= idx;
				else
					data &= notidx;
					/* This else clause is implemented to ensure
					   that the timing of the if and else clause
					   are as close to equal as possible. */
			}

			this->delay(_rx_delay_stopbit);

			/* Check for buffer overflows */
			if((_receive_buffer_tail + 1) % SS_RX_BUFFER_SIZE != _receive_buffer_head) {
				this->_receive_buffer[_receive_buffer_tail] = data;
				this->_receive_buffer_tail = (_receive_buffer_tail + 1) % SS_RX_BUFFER_SIZE;
			} else {
				/* buffer has flown over */
				this->_buffer_overflow = true;
			}
		}

#if GCC_VERSION < 40302
		asm volatile(
			"pop r27 \n\t"
			"pop r26 \n\t"
			"pop r23 \n\t"
			"pop r22 \n\t"
			"pop r21 \n\t"
			"pop r20 \n\t"
			"pop r19 \n\t"
			"pop r18 \n\t"
			:
			:
		);
#endif
	}

	void AvrUart::write(uint8_t byte)
	{
		if(_tx_delay == 0)
			return;

		enter_critical();
		this->set_tx_low();
		this->delay(this->_tx_delay + XMIT_START_ADJUSTMENT);

		for(uint8_t mask = 1; mask; mask <<= 1) {
			if(byte & mask)
				this->set_tx_high();
			else
				this->set_tx_low();

			this->delay(this->_tx_delay);
		}

		this->set_tx_high();
		this->delay(this->_tx_delay);
		exit_critical();
	}

	void AvrUart::write(const uint8_t *buffer, const size_t& length)
	{
		if(_tx_delay == 0)
			return;

		for(unsigned idx = 0; idx < length; idx++) {
			this->write(buffer[idx]);
		}
	}

	void AvrUart::flush()
	{
		if(!this->isListening())
			return;

		enter_critical();
		this->_receive_buffer_head = _receive_buffer_tail = 0;
		exit_critical();
	}

	uint8_t AvrUart::read()
	{
		if(!this->isListening())
			return 0;

		if(this->_receive_buffer_head == this->_receive_buffer_tail)
			return 0;

		auto data = this->_receive_buffer[this->_receive_buffer_head];
		this->_receive_buffer_head = (this->_receive_buffer_head + 1) % SS_RX_BUFFER_SIZE;
		return data;
	}

	ssize_t AvrUart::read(uint8_t *buffer, const size_t& length)
	{
		unsigned idx;

		if(!this->isListening())
			return -EINVALID;

		idx = 0;
		while(idx < length) {
			if(!available())
				continue;

			buffer[idx] = this->read();
			idx++;
		}

		return idx;
	}

	inline void AvrUart::set_tx_high() const
	{
		*(this->port) |= this->txbit;
	}

	inline void AvrUart::set_tx_low() const
	{
		*(this->port) &= ~this->txbit;
	}

	inline bool AvrUart::get_rx_value() const
	{
		auto value = *(this->pin);

		value &= this->rxbit;
		return !!value;
	}
}

/*
 * IRQ definitions
 */

#ifdef PCINT0_vect
ISR(PCINT0_vect)
{
	lwiot::isr_entry();
}
#endif

#ifdef PCINT1_vect
ISR(PCINT1_vect)
{
	lwiot::isr_entry();
}
#endif

#ifdef PCINT2_vect
ISR(PCINT2_vect)
{
	lwiot::isr_entry();
}
#endif

#ifdef PCINT3_vect
ISR(PCINT3_vect)
{
	lwiot::isr_entry();
}
#endif
