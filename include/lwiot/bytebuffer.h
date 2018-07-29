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
	class ByteBuffer : public Countable<uint8_t> {
	public:
		explicit ByteBuffer();
		explicit ByteBuffer(const size_t& size);
		explicit ByteBuffer(const ByteBuffer& other);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
		explicit ByteBuffer(ByteBuffer&& other);
#endif
		virtual ~ByteBuffer();

		virtual ByteBuffer& operator =(const ByteBuffer& other);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
		virtual ByteBuffer& operator =(ByteBuffer&& other);
#endif

		virtual const uint8_t& operator[] (const size_t& idx) const override;
		virtual uint8_t& operator[] (const size_t& idx) override;

		const uint8_t *begin() const;
		const uint8_t *end() const;

		void setIndex(size_t index) { this->_index = index; }

		bool operator ==(const ByteBuffer& rhs) const;

		void write(uint8_t byte);
		void write(const uint8_t *bytes, size_t num);

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
	};
}
#endif
