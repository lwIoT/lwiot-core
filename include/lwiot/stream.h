/*
 * Virtual data stream definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/string.h>

namespace lwiot {
	class Stream {
	public:
		explicit Stream();
		Stream(const time_t& timeout);
		virtual ~Stream();

		virtual size_t available() = 0;
		virtual uint8_t read() = 0;

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

	private:
		time_t _timeout;
		time_t _millis;
	};
}