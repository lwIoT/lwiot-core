/*
 * lwIoT logging header
 *
 * Author: Michel Megens
 * Date: 04/12/2017
 * Email: dev@bietje.net
 */

/// @file log.h

#ifndef __LOG_H__
#define __LOG_H__

#include <lwiot_opts.h>
#include <stdlib.h>
#include <stdio.h>

#include <lwiot/stl/string.h>
#include <lwiot/lwiot.h>

#if defined(HAVE_DEBUG) || defined(DOXYGEN)
CDECL
/**
 * @brief Print debug output.
 * @param fmt Format string.
 * @param ... Arguments for the format string.
 */
extern DLL_EXPORT void print_dbg(const char *fmt, ...);
CDECL_END
#else
#define print_dbg(...)
#endif

CDECL
/**
 * @brief Initialize the logging subsystem.
 * @param file File to write to.
 */
extern DLL_EXPORT void log_init(const FILE *file);

/**
 * @brief Perform a system panic.
 * @param fmt Format string.
 * @param ... Arguments to the format string.
 */
extern DLL_EXPORT void panic(const char *fmt, ...);
CDECL_END

#ifdef __cplusplus
#include <lwiot/stl/string.h>

namespace lwiot
{
	/**
	 * @ingroup util
	 * @brief Logging stream.
	 */
	class Logger {
	public:
		/**
		 * @brief Logging visibility.
		 */
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

		/**
		 * @brief Create a new logger instance.
		 */
		explicit Logger();

		/**
		 * @brief Create a new logger instance.
		 * @param output Output file.
		 */
		explicit Logger(FILE *output);

		/**
		 * @brief Create a new logger instance.
		 * @param subsys Subsystem name.
		 * @param output Output file.
		 */
		explicit Logger(const String& subsys, FILE *output = stdout);

		/**
		 * @brief Set the stream visibility.
		 * @param visibility The visibility to set.
		 */
		void setStreamVisibility(Visibility visibility);

		/**
		 * @brief Stream operator.
		 * @param nl New line feed.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(NewLine nl);

		/**
		 * @brief Stream operator.
		 * @param txt Logging text.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(const char *txt);

		/**
		 * @brief Stream operator.
		 * @param txt Logging text.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(const String& txt);

		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(int num);
		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(long num);
		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(unsigned int num);
		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(unsigned long num);
		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(long long num);
		/**
		 * @brief Stream operator.
		 * @param num Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(unsigned long long num);
		/**
		 * @brief Stream operator.
		 * @param fp Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(float fp);
		/**
		 * @brief Stream operator.
		 * @param fp Number to output to the logging stream.
		 * @return A reference to \p *this.
		 */
		Logger& operator <<(double fp);

		/**
		 * @brief Print debugging output.
		 * @param str String to output.
		 * @return A reference to \p *this.
		 */
		Logger& debug(const String& str);

		/**
		 * @brief Print critical output.
		 * @param str String to output.
		 * @return A reference to \p *this.
		 */
		Logger& critical(const String& str);

		/**
		 * @brief Print informational output.
		 * @param str String to output.
		 * @return A reference to \p *this.
		 */
		Logger& info(const String& str);

		/**
		 * @brief Set the visibility.
		 * @param v Visibility to set.
		 */
		void setVisibility(Visibility v);

		/**
		 * @brief Get the visibility.
		 * @return The current visibility.
		 */
		Visibility visibility() const;

		static NewLine newline; //!< New line feed.

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
