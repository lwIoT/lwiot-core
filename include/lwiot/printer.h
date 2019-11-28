/*
 * Printer interface.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/stl/string.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

namespace lwiot
{
	class Printer;

	/**
	 * @brief Pritable interface.
	 * @ingroup util
	 */
	class Printable
	{
	public:
		virtual ~Printable() = default;
		virtual size_t printTo(Printer& p) const = 0; //!< Print to a printer.
	};

	/**
	 * @brief Printer class.
	 * @ingroup util
	 */
	class Printer {
	public:

		/**
		 * @brief Construct a new printer.
		 */
		explicit Printer() : write_error(0)
		{
		}

		virtual ~Printer() = default; //!< Destruct a printer.

		/**
		 * @brief Get the last error code.
		 */
		int getWriteError()
		{
			return write_error;
		}

		/**
		 * @brief Clear the last error code.
		 */
		void clearWriteError()
		{
			setWriteError(0);
		}

		virtual size_t write(uint8_t byte) = 0; //!< Write a byte value.

		/**
		 * @brief Write a string value.
		 * @param str String to write.
		 * @return Number of bytes written.
		 */
		size_t write(const char *str)
		{
			if(str == nullptr) {
				return 0;
			}
			return write((const uint8_t *) str, strlen(str));
		}

		/**
		 * @brief Write a buffer.
		 * @param buffer Buffer to write.
		 * @param size Number of bytes to write.
		 * @return Number of bytes written.
		 */
		virtual size_t write(const uint8_t *buffer, size_t size);

		/**
		 * @brief Write a C-style string.
		 * @param buffer Buffer to write.
		 * @param size Number of bytes to write.
		 * @return Number of bytes written.
		 */
		size_t write(const char *buffer, size_t size)
		{
			return write((const uint8_t *) buffer, size);
		}

#ifdef WIN32
		size_t printf(const char * format, ...);
#else
		/**
		 * @brief Format print.
		 * @param format Format string.
		 * @param ... Arguments to the format.
		 * @return Number of bytes printed.
		 */
		size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));
#endif

		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 */
		size_t print(const String & value);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 */
		size_t print(const char* value);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 */
		size_t print(char value);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param base Integer base.
		 * @return Number of bytes printed.
		 */
		size_t print(unsigned char value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param base Integer base.
		 * @return Number of bytes printed.
		 */
		size_t print(int value, int = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param base Integer base.
		 * @return Number of bytes printed.
		 */
		size_t print(unsigned int value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param base Integer base.
		 * @return Number of bytes printed.
		 */
		size_t print(long value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param base Integer base.
		 * @return Number of bytes printed.
		 */
		size_t print(unsigned long value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param digits Number of digits to print.
		 * @return Number of bytes printed.
		 */
		size_t print(double value, int digits  = 2);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 */
		size_t print(const Printable& value);
		/**
		 * @brief Print function.
		 * @param timeinfo Value to print.
		 * @param format Time format.
		 * @return Number of bytes printed.
		 */
		size_t print(struct tm * timeinfo, const char * format = nullptr);

		/**
		 * @brief Print function.
		 * @param s Value to print.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(const String &s);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(const char*  value);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(char value);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @param base Integer base.
		 * @note A new line is automatically appended.
		 */
		size_t println(unsigned char value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @param base Integer base.
		 * @note A new line is automatically appended.
		 */
		size_t println(int value, int = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @param base Integer base.
		 * @note A new line is automatically appended.
		 */
		size_t println(unsigned int value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @param base Integer base.
		 * @note A new line is automatically appended.
		 */
		size_t println(long value, int = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @param base Integer base.
		 * @note A new line is automatically appended.
		 */
		size_t println(unsigned long value, int base = DEC);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @param digits Number of digits to print.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(double value, int digits = 2);
		/**
		 * @brief Print function.
		 * @param value Value to print.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(const Printable& value);
		/**
		 * @brief Print function.
		 * @param timeinfo Value to print.
		 * @param format Time format.
		 * @return Number of bytes printed.
		 * @note A new line is automatically appended.
		 */
		size_t println(struct tm * timeinfo, const char * format = nullptr);
		/**
		 * @brief Print function.
		 * @note A new line is automatically appended.
		 */
		size_t println();

	private:
		int write_error;
		size_t printNumber(unsigned long, uint8_t);
		size_t printFloat(double, uint8_t);

	protected:
		/**
		 * @brief Set a write error code.
		 * @param err Error code to set.
		 */
		void setWriteError(int err = 1)
		{
			write_error = err;
		}
	};
}
