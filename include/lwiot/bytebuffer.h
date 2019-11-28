/*
 * Byte array wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file bytebuffer.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/countable.h>

#define BYTEBUFFER_DEFAULT_SIZE 32

#ifdef __cplusplus
namespace lwiot
{
	/**
	 * @brief Raw buffer wrapper.
	 * @ingroup util
	 */
	struct RawBuffer {
	public:
		constexpr RawBuffer() : _buffer(nullptr), _size(0)
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

	/**
	 * @brief Wrapper around a C-style buffer of bytes.
	 * @ingroup util
	 */
	class ByteBuffer : public Countable<uint8_t> {
	public:
		/**
		 * @brief Construct a new byte buffer.
		 */
		explicit ByteBuffer();
		/**
		 * @brief Construct a new byte buffer.
		 * @param size Initial size.
		 * @param exactfit Allocate exactly.
		 */
		explicit ByteBuffer(const size_t& size, bool exactfit = false);
		/**
		 * @brief Construct a new byte buffer.
		 * @param other ByteBuffer to copy.
		 */
		ByteBuffer(const ByteBuffer& other);
		/**
		 * @brief Construct a new byte buffer.
		 * @param other ByteBuffer to move.
		 */
		ByteBuffer(ByteBuffer&& other) noexcept;
		/**
		 * @brief Destruct a ByteBuffer.
		 */
		~ByteBuffer() override ;

		/**
		 * @brief Copy assignment operator.
		 * @param other ByteBuffer to copy.
		 * @return A reference to \p *this.
		 */
		ByteBuffer& operator =(const ByteBuffer& other);
		/**
		 * @brief Move assignment operator.
		 * @param other ByteBuffer to move.
		 * @return A reference to \p *this.
		 */
		ByteBuffer& operator =(ByteBuffer&& other) noexcept;

		/**
		 * @brief Access operator.
		 * @param idx Index to access.
		 * @return A reference to the value at \p idx.
		 */
		const uint8_t& operator[] (const size_t& idx) const override;

		/**
		 * @brief Access operator.
		 * @param idx Index to access.
		 * @return A reference to the value at \p idx.
		 */
		uint8_t& operator[] (const size_t& idx) override;

		/**
		 * @brief Access the first byte.
		 * @return A reference to the first byte.
		 */
		uint8_t *begin() const;

		/**
		 * @brief Access the last byte.
		 * @return A reference to the first byte.
		 */
		uint8_t *end() const;

		/**
		 * @brief Access the hunderlying buffer.
		 * @return The underlying buffer.
		 */
		uint8_t *data() const { return this->_data; }

		/**
		 * @brief Set the internal index.
		 * @param index Index to set.
		 */
		void setIndex(size_t index) { this->_index = index; }
		/**
		 * @brief Get the internal index.
		 * @return Index value.
		 */
		size_t index() const { return this->_index; }

		/**
		 * @brief Equality operator.
		 * @param rhs Object to compare against \p *this.
		 * @return Success indicator.
		 */
		bool operator ==(const ByteBuffer& rhs) const;
		/**
		 * @brief Equality operator.
		 * @param rhs Object to compare against \p *this.
		 * @return Success indicator.
		 */
		bool operator !=(const ByteBuffer& rhs) const;

		/**
		 * @brief Write a value into \p *this.
		 * @param byte Value to write.
		 */
		void write(uint8_t byte);

		/**
		 * @brief Copy a C-style buffer into \p *this.
		 * @param bytes Bytes to copy.
		 * @param num Number of bytes to write.
		 */
		void write(const void *bytes, size_t num);

		/**
		 * @brief Copy a RawBuffer buffer into \p *this.
		 * @param raw Buffer to copy.
		 */
		void write(const RawBuffer& raw);

		/**
		 * @brief Write a ByteBuffer into \p *this.
		 * @param buffer Buffer to copy.
		 */
		void write(const ByteBuffer& buffer);

		/**
		 * @brief Iterate over \p *this.
		 * @tparam Func Functor type.
		 * @param f Functor object.
		 */
		template<typename Func>
		void foreach(Func f) const
		{
			for(auto idx = 0UL; idx < this->_index; idx++) {
				f(this->_data[idx]);
			}
		}

		/**
		 * @brief Clear \p *this.
		 */
		void reset();

	protected:
		size_t _index; //!< Internal index.

		/**
		 * @brief Grow the internal buffer.
		 * @param size Number of additional bytes to allocate.
		 */
		void grow(const size_t& size) override;

		/**
		 * @brief Move operation.
		 * @param other Buffer to move.
		 */
		void move(ByteBuffer& other);

		/**
		 * @brief Copy a byte buffer.
		 * @param other Buffer to copy.
		 */
		void copy(const ByteBuffer& other);

		/**
		 * @brief Reset the internal buffer and index.
		 * @param newsize New size of the internal buffer.
		 */
		virtual void reset(const size_t& newsize);

	private:
		uint8_t *_data;
		bool _exactfit;
		uint8_t _default;
	};
}
#endif
