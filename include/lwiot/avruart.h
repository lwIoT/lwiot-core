/*
 * AVR ATmega UART definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/log.h>
#include <lwiot/uart.h>

#define SS_RX_BUFFER_SIZE 64

namespace lwiot
{
	class AvrUart : public Uart {
	public:
		explicit AvrUart(int rx, int tx, long baud = 9600);
		virtual ~AvrUart();

		virtual uint8_t read() override;
		virtual ssize_t read(uint8_t *buffer, const size_t& length) override;
		using Stream::read;

		virtual void write(uint8_t byte) override;
		virtual void write(const uint8_t *buffer, const size_t& length) override;
		virtual void flush();
		using Stream::write;

		virtual size_t available() override;

	private:
		/* Methods */
		inline void set_tx_high() const;
		inline void set_tx_low() const;
		inline bool get_rx_value() const;
		inline void delay(uint16_t delay) const;
		void init();
		bool isListening();
		friend void isr_entry();
		bool end();
		void setIrqMask(bool enabled);

		bool listen();
		void recv();

		/* Statics */
		static char _receive_buffer[SS_RX_BUFFER_SIZE];
		static volatile uint8_t _receive_buffer_tail;
		static volatile uint8_t _receive_buffer_head;
		static AvrUart *active_object;
		static inline void handle_isr();

		/* Attribs */
		bool initialized;

		volatile uint8_t *port;
		volatile uint8_t *pin;
		int8_t txbit, rxbit;

		uint16_t _rx_delay_centering;
		uint16_t _rx_delay_intrabit;
		uint16_t _rx_delay_stopbit;
		uint16_t _tx_delay;

		uint16_t _buffer_overflow:1;
	};
}