/*
 * lwIoT logging header
 *
 * Author: Michel Megens
 * Date: 04/12/2017
 * Email: dev@bietje.net
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <lwiot_opts.h>
#include <stdlib.h>
#include <stdio.h>

#include <lwiot/stl/string.h>
#include <lwiot/lwiot.h>

#ifdef HAVE_DEBUG
CDECL
extern DLL_EXPORT void print_dbg(const char *fmt, ...);
CDECL_END
#else
#define print_dbg(...)
#endif

CDECL
extern DLL_EXPORT void log_init(const FILE *file);
extern DLL_EXPORT void panic(const char *fmt, ...);
CDECL_END

#ifdef __cplusplus
#include <lwiot/stl/string.h>

namespace lwiot {
	class Logger {
	public:
		enum Visibility {
			None = 0,
			Critical,
			Info,
			Debug,
			Trace
		};

		class NewLine {
		public:
			NewLine() = default;
		};

		explicit Logger();
		explicit Logger(FILE *output);
		explicit Logger(const String& subsys, FILE *output = stdout);

		void setStreamVisibility(Visibility visibility);

		Logger& operator <<(NewLine nl);
		Logger& operator <<(const char *txt);
		Logger& operator <<(const String& txt);
		Logger& operator <<(int num);
		Logger& operator <<(long num);
		Logger& operator <<(unsigned int num);
		Logger& operator <<(unsigned long num);
		Logger& operator <<(long long num);
		Logger& operator <<(unsigned long long num);
		Logger& operator <<(float fp);
		Logger& operator <<(double fp);

		Logger& debug(const String& str);
		Logger& critical(const String& str);
		Logger& info(const String& str);

		void setVisibility(Visibility v);
		Visibility visibility() const;

		static NewLine newline;

	private:
		FILE *_f_output;
		bool _newline;
		String _subsys;
		Visibility _visibility;
		Visibility _output;

		void format(const char *fmt, ...);
		void print_newline();
	};
}
#endif

#endif // !__LOG_H__
