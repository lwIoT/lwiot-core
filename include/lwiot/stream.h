/*
 * Virtual data stream definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file stream.h

#pragma once

#include <stdint.h>

#include <lwiot/types.h>
#include <lwiot/stl/string.h>

#ifdef __cplusplus
namespace lwiot
{
	/**
	 * @brief Stream class.
	 * @ingroup util
	 */
	class Stream {
	public:
		/**
		 * @brief Construct a stream.
		 */
		explicit Stream();
		/**
		 * @brief Construct a stream.
		 * @param timeout Stream timeout.
		 */
		explicit Stream(const time_t& timeout);

		virtual ~Stream() = default; //!< Destroy a Stream.

		/**
		 * @brief Check for available bytes.
		 * @return The number of available bytes.
		 */
		virtual size_t available() const = 0;

		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (char x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (short x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (int  x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const long&  x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const long long&  x) = 0;

		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (unsigned char x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (unsigned short x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (unsigned int  x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const unsigned long&  x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const unsigned long long&  x) = 0;

		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const double& x) = 0;
		/**
		 * @brief Stream operator.
		 * @param x Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const float& x) = 0;

		/**
		 * @brief Stream operator.
		 * @param str Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const String& str) = 0;
		/**
		 * @brief Stream operator.
		 * @param cstr Value to write to the stream.
		 * @return A reference to \p *this.
		 */
		virtual Stream& operator << (const char *cstr) = 0;

		/**
		 * @brief Read a byte from \p *this.
		 * @return The first byte of incoming data.
		 */
		virtual uint8_t read() = 0;

		/**
		 * @brief Read a number of bytes.
		 * @param output Output buffer.
		 * @param length Number of bytes to read.
		 * @return Actual number of bytes read into \p output.
		 */
		virtual ssize_t read(void *output, const size_t& length) = 0;
		virtual ssize_t read(String& output);

		/**
		 * @brief Read a string.
		 * @return String object.
		 */
		virtual String readString();

		/**
		 * @brief Read a string until \p terminator is encountered.
		 * @param terminator Terminator character.
		 * @return String object.
		 */
		virtual String readStringUntil(char terminator);

		/**
		 * @brief Write a byte.
		 * @param byte Byte to write.
		 * @return Success indicator.
		 */
		virtual bool write(uint8_t byte) = 0;

		/**
		 * @brief Write a buffer.
		 * @param bytes Buffer object.
		 * @param length Number of bytes to write.
		 * @return Number of bytes written.
		 */
		virtual ssize_t write(const void *bytes, const size_t& length) = 0;

		/**
		 * @brief Write a string.
		 * @param data String to write.
		 * @return The number of bytes written.
		 */
		virtual ssize_t write(const String& data);

		/**
		 * @brief Set the timeout value.
		 * @param tmo Timeout value to set.
		 * @see _timeout
		 */
		virtual void setTimeout(time_t tmo);

	protected:
		time_t _timeout; //!< Stream timeout.
	};
}
#endif
