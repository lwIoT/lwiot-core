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
#include <lwiot/countable.h>

namespace lwiot {
	class BufferedStream : public Stream, public Countable<uint8_t> {
	public:
		explicit BufferedStream();
		explicit BufferedStream(int size);
		explicit BufferedStream(const BufferedStream& stream);

		virtual ~BufferedStream();

		virtual Stream& operator =(const Stream& stream) override;

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

		virtual uint8_t& operator [] (const size_t& indx) override;
		virtual const uint8_t& operator[] (const size_t& idx) const override;

		const void *data() const;
		const size_t& size() const;

		virtual size_t available() override;
		virtual String toString();

		virtual uint8_t read() override;
		virtual ssize_t read(uint8_t *output, const size_t& length) override;
		virtual ssize_t read(String& output) override final;

		virtual void write(uint8_t byte) override;
		virtual void write(const uint8_t *bytes, const size_t& length) override;
		using Stream::write;

	protected:
		virtual size_t length() const;

	private:
		/* methods */
		void append(const void *data, size_t length);
		void grow();
		void grow(int num);

		/* attributes */
		uint8_t *_data;
		size_t rd_idx, wr_idx;
	};
}
