/*
 * Streamed byte stream implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/stream.h>
#include <lwiot/string.h>

namespace lwiot {
	class BufferedStream : public Stream {
	public:
		explicit BufferedStream();
		explicit BufferedStream(int size);

		virtual ~BufferedStream();

		virtual Stream& operator << (char x) override;
		virtual Stream& operator << (short x) override;
		virtual Stream& operator << (int  x) override;
		virtual Stream& operator << (const long&  x) override;
		virtual Stream& operator << (const long long&  x) override;

		virtual Stream& operator << (unsigned char x) override;
		virtual Stream& operator << (unsigned short x) override;
		virtual Stream& operator << (unsigned int  x) override;
		virtual Stream& operator << (const unsigned long&  x) override;
		virtual Stream& operator << (const unsigned long long&  x) override;

		virtual Stream& operator << (const double& flt) override;
		virtual Stream& operator << (const float& flt) override;

		virtual Stream& operator << (const String& str) override;
		virtual Stream& operator << (const char *cstr) override;

		virtual uint8_t& operator [] (size_t indx);

		const void *data() const;
		const size_t& size() const;

		virtual size_t available() override;
		virtual uint8_t read() override;
		virtual String toString();

	protected:
		virtual size_t length() const;

	private:
		/* methods */
		void append(void *data, size_t length);
		void grow();
		void grow(int num);

		/* attributes */
		uint8_t *_data;
		size_t _size;
		size_t rd_idx, wr_idx;
	};
}
