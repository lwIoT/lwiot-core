/*
 * UART interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>
#include <lwiot/uart.h>


namespace lwiot
{
	Uart::Uart(long baud, const GpioPin& rx, const GpioPin& tx, uint32_t config) : _baud(baud), _rx(rx), _tx(tx), _config(config)
	{ }

	Uart::Uart(long baud, uint32_t config) : Uart(baud, GpioPin(0), GpioPin(1), config)
	{ }
 
	Uart::operator bool() const
	{
		return this->available();
	}

	const long& Uart::baud() const
	{
		return this->_baud;
	}

	const GpioPin& Uart::tx() const
	{
		return this->_tx;
	}

	void Uart::write(const String& string)
	{
		size_t length;

		auto ary = (const uint8_t*) string.c_str();
		length = string.length();
		this->write(ary, length);
	}

	ssize_t Uart::read(String& output)
	{
		return -1;
	}

	const GpioPin& Uart::rx() const
	{
		return this->_rx;
	}

	const uint32_t& Uart::config() const
	{
		return this->_config;
	}
}
