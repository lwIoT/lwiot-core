/*
 * ArduinoJson printer.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <stdint.h>

#include <ArduinoJson/Arduino/Printer.hpp>

namespace lwiot { namespace json
{
	Printer::Printer()
	{ }

	size_t Printer::print(const char *s)
	{
		size_t n = 0;

		while(*s) {
			n += this->write(*s++);
		}

		return n;
	}

	size_t Printer::print(ArduinoJson::Internals::JsonFloat value, int digits /* = 2 */)
	{
		char tmp[32];
		bool isBigDouble = value > 4294967040.0 || value < -4294967040.0;

		if (isBigDouble) {
			ARDUINOJSON_SNPRINTF(tmp, sizeof(tmp), "%g", value);
		} else {
			ARDUINOJSON_SNPRINTF(tmp, sizeof(tmp), "%.*f", digits, value);
		}

		return print(tmp);
	}

	size_t Printer::print(ArduinoJson::Internals::JsonInteger value)
	{
		char buffer[22];
		size_t n = 0;
		uint8_t i = 0;

		if (value < 0) {
			value = -value;
			n += this->write('-');
		}

		do {
			ArduinoJson::Internals::JsonInteger digit = value % 10;
			value /= 10;
			buffer[i++] = static_cast<char>(digit >= 0 ? '0' + digit : '0' - digit);
		} while (value);

		while (i > 0) {
			n += this->write(buffer[--i]);
		}

		return n;
	}

	size_t Printer::println()
	{
		return write('\r') + write('\n');
	}
}
}
