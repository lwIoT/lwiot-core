/*
 * ESP32 UART implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/uart.h>
#include <lwiot/stream.h>
#include <lwiot/io/gpiopin.h>

#include <driver/uart.h>

namespace lwiot { namespace esp32
{
	class Uart : public lwiot::Uart {
	public:
		explicit Uart(int num, long baud, uint32_t config = SERIAL_8N1);
		virtual ~Uart();

		virtual uint8_t read() override;
		virtual ssize_t read(void *buffer, const size_t& length) override;
		using lwiot::Uart::read;

		bool write(uint8_t byte) override;
		ssize_t write(const void *buffer, const size_t& length) override;
		using lwiot::Uart::write;

		virtual size_t available() const override;

	private:
		uart_config_t _setup;
		uart_port_t _uart_num;
	};
}
}
