/*
 * ESP32 UART.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <esp8266.h>
#include <lwiot.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/uart.h>
#include <lwiot/esp8266/esp8266uart.h>

#include "esp8266-softuart.h"

#define UART_GPIO_TXD 2

namespace lwiot { namespace esp8266
{
	Uart::Uart(int num, int rx, int tx, long baud, uint32_t config) :
		lwiot::Uart(tx, rx, baud, config), _num((uint8_t)num)
	{
		esp_softuart_open(this->_num, baud, rx, tx);
		this->initialized = true;
	}

	Uart::~Uart()
	{
		if(!this->initialized)
			return;

		this->initialized = false;
		esp_softuart_close(this->_num);
	}

	size_t Uart::available() const
	{
		auto avail = esp_softuart_available(this->_num);
		return static_cast<size_t>(avail);
	}

	uint8_t Uart::read()
	{
		while(!this->available());
		return esp_softuart_read(this->_num);
	}

	ssize_t Uart::read(uint8_t *buffer, const size_t& size)
	{
		for(size_t idx = 0; idx < size; idx++) {
			buffer[idx] = this->read();
		}

		return (ssize_t)size;
	}

	bool Uart::write(uint8_t byte)
	{
		return esp_softuart_put(this->_num, static_cast<char>(byte));
	}

	ssize_t Uart::write(const void *buf, const size_t& length)
	{
		size_t idx = 0UL;
		const uint8_t *buffer = static_cast<const uint8_t *>(buf);

		for(; idx < length; idx++) {
			if(!this->write(buffer[idx]))
				break;
		}

		return idx + 1;
	}
}
}
