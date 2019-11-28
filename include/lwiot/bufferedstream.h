/*
 * Streamed byte stream implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file bufferedstream.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/stream.h>
#include <lwiot/stl/string.h>
#include <lwiot/countable.h>

namespace lwiot {
	/**
	 * @brief Buffered stream class.
	 * @ingroup util
	 */
	class BufferedStream : public Stream, public Countable<uint8_t> {
	public:
		/**
		 * @brief Buffered stream constructor.
		 */
		explicit BufferedStream();
		/**
		 * @brief Buffered stream constructor.
		 * @param size Initial size.
		 */
		explicit BufferedStream(int size);

		/**
		 * @brief Buffered stream constructor.
		 * @param stream Stream to copy.
		 */
		BufferedStream(const BufferedStream& stream);
		/**
		 * @brief Buffered stream constructor.
		 * @param other Stream to move.
		 */
		BufferedStream(BufferedStream&& other) noexcept;

		/**
		 * @brief Destroy a buffered stream.
		 */
		~BufferedStream() override;

		/**
		 * @brief Assignment operator.
		 * @param stream Stream to copy.
		 * @return A reference to \p *this.
		 */
		BufferedStream& operator =(const Stream& stream);

		/**
		 * @brief Assignment operator.
		 * @param stream Stream to move.
		 * @return A reference to \p *this.
		 */
		BufferedStream& operator =(Stream&& stream) noexcept;

		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (char x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (short x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (int  x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const long&  x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const long long&  x) override;

		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (unsigned char x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (unsigned short x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (unsigned int  x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const unsigned long&  x) override;
		/**
		 * @brief Assignment operator.
		 * @param x Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const unsigned long long&  x) override;

		/**
		 * @brief Assignment operator.
		 * @param flt Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const double& flt) override;
		/**
		 * @brief Assignment operator.
		 * @param flt Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const float& flt) override;

		/**
		 * @brief Assignment operator.
		 * @param str Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const String& str) override;
		/**
		 * @brief Assignment operator.
		 * @param cstr Value to write into the stream.
		 * @return A reference to Stream.
		 */
		Stream& operator << (const char *cstr) override;

		/**
		 * @brief Access operator.
		 * @param indx Buffer index to access.
		 * @return A reference to the byte at \p indx.
		 */
		uint8_t& operator [] (const size_t& indx) override;

		/**
		 * @brief Access operator.
		 * @param idx Buffer index to access.
		 * @return A reference to the byte at \p indx.
		 */
		const uint8_t& operator[] (const size_t& idx) const override;

		/**
		 * @brief Getter for the underlying buffer.
		 * @return A pointer to the underlying buffer.
		 */
		const void *data() const;

		/**
		 * @brief Size getter.
		 * @return The size of data.
		 */
		const size_t& size() const;

		/**
		 * @brief Get the number of available bytes.
		 * @return The number of available bytes.
		 */
		size_t available() const override;

		/**
		 * @brief Convert \p *this to String.
		 * @return String representation of \p *this.
		 */
		virtual String toString();

		/**
		 * @brief Read the next byte.
		 * @return The next byte.
		 */
		uint8_t read() override;

		/**
		 * @brief Read a number of bytes.
		 * @param output Output buffer.
		 * @param length Number of bytes to read.
		 * @return Number of actual bytes read.
		 */
		ssize_t read(void *output, const size_t& length) override;
		/**
		 * @brief Read a number of bytes.
		 * @param output Output buffer.
		 * @return Number of actual bytes read.
		 */
		ssize_t read(String& output) final;

		/**
		 * @brief Write a byte.
		 * @param byte Byte to write.
		 * @return Success indicator.
		 */
		bool write(uint8_t byte) override;

		/**
		 * @brief Write a number of bytes to the internal buffer.
		 * @param bytes Bytes to write.
		 * @param length Number of bytes to write.
		 * @return Actual number of bytes written.
		 */
		ssize_t write(const void *bytes, const size_t& length) override;
		using Stream::write;

	protected:
		virtual size_t length() const; //!< Length of the buffer.
		void grow(const size_t& num) override; //!< Grow the internal buffer.

	private:
		/* methods */
		void append(const void *data, size_t length);
		void grow();

		void move(BufferedStream& other);

		/* attributes */
		uint8_t *_data;
		size_t rd_idx, wr_idx;
	};
}
