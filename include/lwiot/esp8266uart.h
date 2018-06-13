/*
 * ESP8266 UART implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/uart.h>
#include <lwiot/stream.h>

#include <esp/uart.h>
#include <esp/uart_regs.h>

namespace lwiot
{
	typedef int uart_port_t;
	class Esp8266Uart : public Uart {
	public:
		explicit Esp8266Uart(int num, long baud, uint32_t config = SERIAL_8N1);
		virtual ~Esp8266Uart() = default;

		virtual uint8_t read();
		virtual ssize_t read(uint8_t *buffer, const size_t& length);
		using Stream::read;

		virtual void write(uint8_t byte);
		virtual void write(const uint8_t *buffer, const size_t& length);
		using Stream::write;

		virtual size_t available() override;

	private:
		uart_port_t _port;
		bool initialized;
	};
}
