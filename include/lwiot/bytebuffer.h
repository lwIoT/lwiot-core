/*
 * Byte array wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/countable.h>

#define BYTEBUFFER_DEFAULT_SIZE 32

#ifdef CXX
namespace lwiot
{
	class ByteBuffer : public Countable<uint8_t> {
	public:
		explicit ByteBuffer();
		explicit ByteBuffer(const size_t& size);
		ByteBuffer(const ByteBuffer& other);
		virtual ~ByteBuffer();

		virtual const uint8_t& operator[] (const size_t& idx) const override;
		virtual uint8_t& operator[] (const size_t& idx) override;

		const uint8_t *begin() const;
		const uint8_t *end() const;

		bool operator ==(const ByteBuffer& rhs) const;

		void write(uint8_t byte);
		void write(const uint8_t *bytes, size_t num);

	protected:
		void grow(const size_t& size) override;

	private:
		size_t _index;
		uint8_t *_data;
	};
}
#endif
