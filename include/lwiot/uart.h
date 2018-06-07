/*
 * UART interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>

#define SERIAL_5N1 0x8000010
#define SERIAL_6N1 0x8000014
#define SERIAL_7N1 0x8000018
#define SERIAL_8N1 0x800001c
#define SERIAL_5N2 0x8000030
#define SERIAL_6N2 0x8000034
#define SERIAL_7N2 0x8000038
#define SERIAL_8N2 0x800003c
#define SERIAL_5E1 0x8000012
#define SERIAL_6E1 0x8000016
#define SERIAL_7E1 0x800001a
#define SERIAL_8E1 0x800001e
#define SERIAL_5E2 0x8000032
#define SERIAL_6E2 0x8000036
#define SERIAL_7E2 0x800003a
#define SERIAL_8E2 0x800003e
#define SERIAL_5O1 0x8000013
#define SERIAL_6O1 0x8000017
#define SERIAL_7O1 0x800001b
#define SERIAL_8O1 0x800001f
#define SERIAL_5O2 0x8000033
#define SERIAL_6O2 0x8000037
#define SERIAL_7O2 0x800003b
#define SERIAL_8O2 0x800003f

namespace lwiot
{
	class Uart {
	public:
		explicit Uart(long baud, const GpioPin& rx, const GpioPin& tx, uint32_t config = SERIAL_8N1);
		explicit Uart(long baud, uint32_t config = SERIAL_8N1);
		virtual ~Uart() = default;

		virtual uint8_t read() = 0;
		virtual ssize_t read(uint8_t *buffer, size_t length) = 0;
		virtual ssize_t read(String& output);
		virtual void write(uint8_t byte) = 0;
		virtual void write(const uint8_t *buffer, size_t length) = 0;
		virtual void write(const String& string);

		virtual bool available() const = 0;
		virtual operator bool() const;

		virtual const uint32_t& config() const final;
		virtual const GpioPin& rx() const final;
		virtual const GpioPin& tx() const final;
		virtual const long& baud() const final;

	protected:
		long _baud;
		GpioPin _rx;
		GpioPin _tx;
		uint32_t _config;
	};
}
