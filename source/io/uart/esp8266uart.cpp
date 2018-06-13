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
#include <lwiot/esp8266uart.h>

#define UART_GPIO_TXD 2

namespace lwiot
{
	Esp8266Uart::Esp8266Uart(int num, long baud, uint32_t config) : Uart(0,0, baud, config)
	{
	}

	size_t Esp8266Uart::available()
	{
		return false;
	}

	uint8_t Esp8266Uart::read()
	{
		return 0;
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
		this->write(&byte, sizeof(byte));
	}

	void Esp8266Uart::write(const uint8_t *buffer, const size_t& length)
	{
	}
}
