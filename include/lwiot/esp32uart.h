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
	class Esp32Uart : public Stream {
	public:
		explicit Esp32Uart(int num, long baud, uint32_t config = SERIAL_8N1);
		virtual ~Esp32Uart();

		virtual uint8_t read() override;
		virtual ssize_t read(uint8_t *buffer, const size_t& length) override;

		void write(uint8_t byte) override;
		void write(const uint8_t *buffer, const size_t& length) override;

		virtual size_t available()
		{
			return 0;
		}

		virtual Stream& operator << (char x) override
		{
			this->write(x);
			return *this;
		}
		
		virtual Stream& operator << (short x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (int  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (const long&  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (const long long&  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (unsigned char x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (unsigned short x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (unsigned int  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (const unsigned long&  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (const unsigned long long&  x) override
		{
			this->write((uint8_t) x);
			return *this;
		}

		virtual Stream& operator << (const double& flt) override
		{
			this->write((uint8_t*)&flt, sizeof(flt));
			return *this;
		}

		virtual Stream& operator << (const float& flt) override
		{
			this->write((uint8_t*)&flt, sizeof(flt));
			return *this;
		}

		virtual Stream& operator << (const String& str) override
		{
			*this << str.c_str();
			return *this;
		}

		virtual Stream& operator << (const char *cstr) override
		{
			this->write((uint8_t*)cstr, strlen(cstr));
			return *this;
		}

	private:
		GpioPin _rx, _tx;
		uart_config_t _setup;
		uart_port_t _uart_num;
	};
}
