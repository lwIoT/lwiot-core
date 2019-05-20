/*
 * File I/O wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/stream.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>

namespace lwiot
{
	enum class FileMode {
		Read,
		Write,
		Append,
		ReadWrite,
		ReadWriteNoCreate,
		ReadWriteAppend
	};

	class File : public virtual Stream {
	public:
		explicit File(const String& fname, FileMode mode);
		virtual ~File();

		size_t available() const override;

		Stream &operator<<(char x) override;
		Stream &operator<<(short x) override;
		Stream &operator<<(int x) override;
		Stream &operator<<(const long &x) override;
		Stream &operator<<(const long long &x) override;
		Stream &operator<<(unsigned char x) override;
		Stream &operator<<(unsigned short x) override;
		Stream &operator<<(unsigned int x) override;
		Stream &operator<<(const unsigned long &x) override;
		Stream &operator<<(const unsigned long long &x) override;
		Stream &operator<<(const double &x) override;
		Stream &operator<<(const float &x) override;
		Stream &operator<<(const String &str) override;
		Stream &operator<<(const char *cstr) override;

		uint8_t read() override;
		ssize_t read(void *output, const size_t &length) override;

		bool write(uint8_t byte) override;
		ssize_t write(const void *bytes, const size_t &length) override;
		using Stream::write;

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
