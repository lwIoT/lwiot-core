/*
 * File I/O wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file file.h

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/stream.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief File mode.
	 * @ingroup io
	 */
	enum class FileMode {
		Read,
		Write,
		Append,
		ReadWrite,
		ReadWriteNoCreate,
		ReadWriteAppend
	};

	/**
	 * @brief File abstraction layer.
	 * @note This object uses the RAII-principle to open and close files.
	 * @ingroup io
	 */
	class File : public virtual Stream {
	public:
		/**
		 * @brief Construct a new file object.
		 * @param fname File name.
		 * @param mode File mode.
		 */
		explicit File(const String& fname, FileMode mode);

		/**
		 * @brief File destructor.
		 * @note The destructor will also close the underlying file.
		 */
		~File() override;

		/**
		 * @brief Number of available bytes.
		 * @return
		 */
		size_t available() const override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(char x) override;
		Stream &operator<<(short x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(int x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const long &x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const long long &x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(unsigned char x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(unsigned short x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(unsigned int x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const unsigned long &x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const unsigned long long &x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const double &x) override;

		/**
		 * @brief Stream write operator.
		 * @param x Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const float &x) override;

		/**
		 * @brief Stream write operator.
		 * @param str Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const String &str) override;

		/**
		 * @brief Stream write operator.
		 * @param cstr Value to write.
		 * @return A reference to \p *this.
		 */
		Stream &operator<<(const char *cstr) override;

		/**
		 * @brief Read the next byte from the underlying file.
		 * @return Byte read.
		 */
		uint8_t read() override;

		/**
		 * @brief Read a series of bytes from the underlying file.
		 * @param output Input buffer.
		 * @param length Number of bytes to read.
		 * @return Actual number of bytes read.
		 */
		ssize_t read(void *output, const size_t &length) override;

		/**
		 * @brief Write a byte.
		 * @param byte Byte to write.
		 * @return Success indicator.
		 */
		bool write(uint8_t byte) override;

		/**
		 * @brief Write a buffer.
		 * @param bytes Output buffer.
		 * @param length Number of bytes to write.
		 * @return Actual number of bytes written.
		 */
		ssize_t write(const void *bytes, const size_t &length) override;
		using Stream::write;

		/**
		 * @brief Write a formatted string.
		 * @param format Format string.
		 * @param ... Arguments to \p format.
		 * @return Number of bytes written.
		 */
		ssize_t write(const String& format, ...);
		
	private:
		SharedPointer<Lock> _lock;
		FILE* _io;
		FileMode _mode;
		size_t _available;

		/* Methods */
		static void fileModeToString(FileMode mode, char *output);
	};
}
