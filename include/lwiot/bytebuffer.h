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

#ifdef __cplusplus
namespace lwiot
{
	struct RawBuffer {
	public:
		RawBuffer() : _buffer(nullptr), _size(0UL)
		{ }

		explicit RawBuffer(void *buf, size_t size) : _buffer(buf), _size(size)
		{ }

		constexpr void* buffer() const
		{
			return this->_buffer;
		}

		constexpr size_t size() const
		{
			return this->_size;
		}

		void *_buffer;
		size_t _size;
	};

	class ByteBuffer : public Countable<uint8_t> {
	public:
		explicit ByteBuffer();
		explicit ByteBuffer(const size_t& size, bool exactfit = false);
		explicit ByteBuffer(const ByteBuffer& other);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
		explicit ByteBuffer(ByteBuffer&& other) noexcept;
#endif
		virtual ~ByteBuffer();

		ByteBuffer& operator =(const ByteBuffer& other);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
		ByteBuffer& operator =(ByteBuffer&& other) noexcept;
#endif

		virtual const uint8_t& operator[] (const size_t& idx) const override;
		virtual uint8_t& operator[] (const size_t& idx) override;

		uint8_t *begin() const;
		uint8_t *end() const;
		uint8_t *data() const { return this->_data; }

		void setIndex(size_t index) { this->_index = index; }
		size_t index() const { return this->_index; }

		bool operator ==(const ByteBuffer& rhs) const;

		void write(uint8_t byte);
		void write(const void *bytes, size_t num);
		void write(const RawBuffer& raw);
		void write(const ByteBuffer& buffer);

		template<typename Func> void foreach(Func f)
		{
			for(auto idx = 0UL; idx < this->_index; idx++) {
				f(this->_data[idx]);
			}
		}

	protected:
		size_t _index;

		void grow(const size_t& size) override;
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
		void move(ByteBuffer& other);
#endif

	private:
		uint8_t *_data;
		bool _exactfit;
	};
}
#endif
