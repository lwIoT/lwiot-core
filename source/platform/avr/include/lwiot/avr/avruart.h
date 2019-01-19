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
#include <lwiot/io/uart.h>

#define SS_RX_BUFFER_SIZE 64

namespace lwiot { namespace avr
{
	class Uart : public lwiot::Uart {
	public:
		explicit Uart(int rx, int tx, long baud = 9600);
		virtual ~Uart();

		virtual uint8_t read() override;
		virtual ssize_t read(void *buffer, const size_t& length) override;
		using Stream::read;

		bool write(uint8_t byte) override;
		ssize_t write(const void *buffer, const size_t& length) override;
		virtual void flush();
		using Stream::write;

		virtual size_t available() const override;

	private:
		/* Methods */
		inline void set_tx_high();
		inline void set_tx_low();
		inline bool get_rx_value();
		inline void delay(uint16_t delay) const;
		void init();
		bool isListening() const;
		friend void isr_entry();
		bool end();
		void setIrqMask(bool enabled);

		bool listen();
		void recv();

		/* Statics */
		static char _receive_buffer[SS_RX_BUFFER_SIZE];
		static volatile uint8_t _receive_buffer_tail;
		static volatile uint8_t _receive_buffer_head;
		static Uart *active_object;
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
}
