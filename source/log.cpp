/*
 * Logging stream implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 * @date   04/05/2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lwiot.h>

#include <lwiot/string.h>
#include <lwiot/log.h>

namespace lwiot {
	Logger::NewLine Logger::newline;

	Logger::Logger(FILE *output) : _f_output(output), _newline(true)
	{ }

	Logger::Logger()
	{
		this->_f_output = stdout;
	}

	Logger::Logger(const String& subsys, FILE *output /* = stdout */) :
		_f_output(output), _newline(true), _subsys(subsys)
	{
	}

	void Logger::print_newline(void)
	{
#ifdef WIN32
		this->format("\r\n");
#else
		this->format("\n");
#endif
		this->_newline = true;
	}

	Logger& Logger::operator<<(NewLine nl)
	{
		this->print_newline();
		return *this;
	}

	Logger& Logger::operator<<(const char *txt)
	{
		this->format("%s", txt);
		return *this;
	}

	Logger& Logger::operator<<(const String& txt)
	{
		this->format("%s", txt.c_str());
		return *this;
	}

	Logger& Logger::operator <<(int num)
	{
		this->format("%i", num);
		return *this;
	}

	Logger& Logger::operator <<(long num)
	{
		this->format("%i", num);
		return *this;
	}

	Logger& Logger::operator <<(unsigned int num)
	{
		this->format("%u", num);
		return *this;
	}

	Logger& Logger::operator <<(unsigned long num)
	{
		this->format("%lu", num);
		return *this;
	}

	Logger& Logger::operator <<(long long num)
	{
		this->format("%lli", num);
		return *this;
	}

	Logger& Logger::operator <<(unsigned long long num)
	{
		this->format("%llu", num);
		return *this;
	}

	Logger& Logger::operator <<(float fp)
	{
		this->format("%f", fp);
		return *this;
	}

	Logger& Logger::operator <<(double fp)
	{
		this->format("%f", fp);
		return *this;
	}

	void Logger::format(const char *fmt, ...)
	{
		va_list va;

		if(fmt == nullptr)
			return;

		if(this->_newline) {
			this->_newline = false;

			if(this->_subsys.length() > 0)
				fprintf(this->_f_output, "[lwiot][%s]: ", this->_subsys.c_str());
			else
				fprintf(this->_f_output, "[lwIoT]: ");
		}

		va_start(va, fmt);
		if(this->_f_output != nullptr)
			vfprintf(this->_f_output, fmt, va);
		va_end(va);
	}
}
