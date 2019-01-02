/*
 * Virtual data stream definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>

#include <lwiot/types.h>
#include <lwiot/string.h>

#ifdef __cplusplus
namespace lwiot {
	class Stream {
	public:
		explicit Stream();
		explicit Stream(const time_t& timeout);
		virtual ~Stream() = default;

		virtual size_t available() const = 0;
		virtual Stream& operator << (char x) = 0;
		virtual Stream& operator << (short x) = 0;
		virtual Stream& operator << (int  x) = 0;
		virtual Stream& operator << (const long&  x) = 0;
		virtual Stream& operator << (const long long&  x) = 0;

		virtual Stream& operator << (unsigned char x) = 0;
		virtual Stream& operator << (unsigned short x) = 0;
		virtual Stream& operator << (unsigned int  x) = 0;
		virtual Stream& operator << (const unsigned long&  x) = 0;
		virtual Stream& operator << (const unsigned long long&  x) = 0;

		virtual Stream& operator << (const double& x) = 0;
		virtual Stream& operator << (const float& x) = 0;

		virtual Stream& operator << (const String& str) = 0;
		virtual Stream& operator << (const char *cstr) = 0;

		virtual uint8_t read() = 0;
		virtual ssize_t read(void *output, const size_t& length) = 0;
		virtual ssize_t read(String& output);

		virtual String readString();
		virtual String readStringUntil(char terminator);

		virtual bool write(uint8_t byte) = 0;
		virtual ssize_t write(const void *bytes, const size_t& length) = 0;
		virtual ssize_t write(const String& data);

	private:
		time_t _timeout;
		time_t _millis;
	};
}
#endif
