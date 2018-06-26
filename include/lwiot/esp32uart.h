/*
 * ESP32 UART implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/uart.h>
#include <lwiot/stream.h>
#include <lwiot/gpiopin.h>

#include <driver/uart.h>

namespace lwiot
{
	class Esp32Uart : public Uart {
	public:
		explicit Esp32Uart(int num, long baud, uint32_t config = SERIAL_8N1);
		virtual ~Esp32Uart();

		virtual uint8_t read() override;
		virtual ssize_t read(uint8_t *buffer, const size_t& length) override;
		using Uart::read;

		void write(uint8_t byte) override;
		void write(const uint8_t *buffer, const size_t& length) override;
		using Uart::write;

		virtual size_t available() override;

	private:
		uart_config_t _setup;
		uart_port_t _uart_num;
	};
}
