/*
 * UART interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/uart.h>

namespace lwiot
{
	Uart::Uart(int tx, int rx, long baud, uint32_t config) : Uart(GpioPin(tx), GpioPin(rx), baud, config)
	{ }
	
	Uart::Uart(const GpioPin& tx, const GpioPin& rx, long baud, uint32_t config) :
		Stream(),
		_tx(tx), _rx(rx), _baud(baud), _config(config)
	{ }

	Stream& Uart::operator << (char x)
	{
		this->write(x);
		return *this;
	}
	
	Stream& Uart::operator << (short x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (int  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (const long&  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (const long long&  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (unsigned char x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (unsigned short x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (unsigned int  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (const unsigned long&  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (const unsigned long long&  x)
	{
		this->write((uint8_t) x);
		return *this;
	}

	Stream& Uart::operator << (const double& flt)
	{
		this->write((uint8_t*)&flt, sizeof(flt));
		return *this;
	}

	Stream& Uart::operator << (const float& flt)
	{
		this->write((uint8_t*)&flt, sizeof(flt));
		return *this;
	}

	Stream& Uart::operator << (const String& str)
	{
		*this << str.c_str();
		return *this;
	}

	Stream& Uart::operator << (const char *cstr)
	{
		this->write((uint8_t*)cstr, strlen(cstr));
		return *this;
	}
}
