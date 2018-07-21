/*
 * ESP32 UART.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>
#include <lwiot/uart.h>
#include <lwiot/esp8266/esp8266uart.h>

#include "esp8266-softuart.h"

#define UART_GPIO_TXD 2

namespace lwiot
{
	Esp8266Uart::Esp8266Uart(int num, int rx, int tx, long baud, uint32_t config) : Uart(tx, rx, baud, config), _num((uint8_t)num)
	{
		esp_softuart_open(this->_num, baud, rx, tx);
		this->initialized = true;
	}

	Esp8266Uart::~Esp8266Uart()
	{
		if(!this->initialized)
			return;

		this->initialized = false;
		esp_softuart_close(this->_num);
	}

	size_t Esp8266Uart::available() const
	{
		auto avail = esp_softuart_available(this->_num);
		return static_cast<size_t>(avail);
	}

	uint8_t Esp8266Uart::read()
	{
		while(!this->available());
		return esp_softuart_read(this->_num);
	}

	ssize_t Esp8266Uart::read(uint8_t *buffer, const size_t& size)
	{
		for(size_t idx = 0; idx < size; idx++) {
			buffer[idx] = this->read();
		}

		return (ssize_t)size;
	}

	void Esp8266Uart::write(uint8_t byte)
	{
		esp_softuart_put(this->_num, static_cast<char>(byte));
	}

	void Esp8266Uart::write(const uint8_t *buffer, const size_t& length)
	{
		for(unsigned int idx = 0; idx < length; idx++) {
			this->write(buffer[idx]);
		}
	}
}
