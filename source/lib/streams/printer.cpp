/*
 * Printer interface implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/printer.h>

extern "C" {
#include <time.h>
}

namespace lwiot
{
	size_t Printer::write(const uint8_t *buffer, size_t size)
	{
		size_t n = 0;
		while(size--) {
			n += write(*buffer++);
		}
		return n;
	}

	size_t Printer::printf(const char *format, ...)
	{
		char loc_buf[64];
		char * temp = loc_buf;
		va_list arg;
		va_list copy;

		va_start(arg, format);
		va_copy(copy, arg);
		size_t len = vsnprintf(NULL, 0, format, arg);
		va_end(copy);

		if(len >= sizeof(loc_buf)){
			temp = (char*)lwiot_mem_alloc(len + 1);
			if(temp == NULL)
				return 0;
		}

		len = vsnprintf(temp, len+1, format, arg);
		write((uint8_t*)temp, len);
		va_end(arg);

		if(len > 64)
			lwiot_mem_free(temp);

		return len;
	}

	size_t Printer::print(const String &s)
	{
		return write(s.c_str(), s.length());
	}

	size_t Printer::print(const char str[])
	{
		return write(str);
	}

	size_t Printer::print(char c)
	{
		return write(c);
	}

	size_t Printer::print(unsigned char b, int base)
	{
		return print((unsigned long) b, base);
	}

	size_t Printer::print(int n, int base)
	{
		return print((long) n, base);
	}

	size_t Printer::print(unsigned int n, int base)
	{
		return print((unsigned long) n, base);
	}

	size_t Printer::print(long n, int base)
	{
		if(base == 0) {
			return write(n);
		} else if(base == 10) {
			if(n < 0) {
				int t = print('-');
				n = -n;
				return printNumber(n, 10) + t;
			}
			return printNumber(n, 10);
		} else {
			return printNumber(n, base);
		}
	}

	size_t Printer::print(unsigned long n, int base)
	{
		if(base == 0) {
			return write(n);
		} else {
			return printNumber(n, base);
		}
	}

	size_t Printer::print(double n, int digits)
	{
		return printFloat(n, digits);
	}

	size_t Printer::print(const Printable& x)
	{
		return x.printTo(*this);
	}

	size_t Printer::print(struct tm * timeinfo, const char * format)
	{
		const char * f = format;
		if(!f){
			f = "%c";
		}
		char buf[64];
		size_t written = strftime(buf, 64, f, timeinfo);
		print(buf);
		return written;
	}

	size_t Printer::println(void)
	{
		return print("\r\n");
	}

	size_t Printer::println(const String &s)
	{
		size_t n = print(s);
		n += println();
		return n;
	}

	size_t Printer::println(const char c[])
	{
		size_t n = print(c);
		n += println();
		return n;
	}

	size_t Printer::println(char c)
	{
		size_t n = print(c);
		n += println();
		return n;
	}

	size_t Printer::println(unsigned char b, int base)
	{
		size_t n = print(b, base);
		n += println();
		return n;
	}

	size_t Printer::println(int num, int base)
	{
		size_t n = print(num, base);
		n += println();
		return n;
	}

	size_t Printer::println(unsigned int num, int base)
	{
		size_t n = print(num, base);
		n += println();
		return n;
	}

	size_t Printer::println(long num, int base)
	{
		size_t n = print(num, base);
		n += println();
		return n;
	}

	size_t Printer::println(unsigned long num, int base)
	{
		size_t n = print(num, base);
		n += println();
		return n;
	}

	size_t Printer::println(double num, int digits)
	{
		size_t n = print(num, digits);
		n += println();
		return n;
	}

	size_t Printer::println(const Printable& x)
	{
		size_t n = print(x);
		n += println();
		return n;
	}

	size_t Printer::println(struct tm * timeinfo, const char * format)
	{
		size_t n = print(timeinfo, format);
		n += println();
		return n;
	}

	// Private Methods /////////////////////////////////////////////////////////////

	size_t Printer::printNumber(unsigned long n, uint8_t base)
	{
		char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
		char *str = &buf[sizeof(buf) - 1];

		*str = '\0';

		// prevent crash if called with base == 1
		if(base < 2) {
			base = 10;
		}

		do {
			unsigned long m = n;
			n /= base;
			char c = m - base * n;
			*--str = c < 10 ? c + '0' : c + 'A' - 10;
		} while(n);

		return write(str);
	}

	size_t Printer::printFloat(double number, uint8_t digits)
	{
		size_t n = 0;

		if(isnan(number)) {
			return print("nan");
		}
		if(isinf(number)) {
			return print("inf");
		}
		if(number > 4294967040.0) {
			return print("ovf");
		}
		if(number < -4294967040.0) {
			return print("ovf");
		}

		if(number < 0.0) {
			n += print('-');
			number = -number;
		}

		double rounding = 0.5;
		for(uint8_t i = 0; i < digits; ++i) {
			rounding /= 10.0;
		}

		number += rounding;

		unsigned long int_part = (unsigned long) number;
		double remainder = number - (double) int_part;
		n += print(int_part);

		if(digits > 0) {
			n += print(".");
		}

		while(digits-- > 0) {
			remainder *= 10.0;
			int to_print = int(remainder);
			n += print(to_print);
			remainder -= to_print;
		}

		return n;
	}
}