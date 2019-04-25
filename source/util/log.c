/*
 * lwIoT logging implementation.
 *
 * Author: Michel Megens
 * Date: 04/12/2017
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>

#include <lwiot/types.h>
#include <lwiot/lwiot.h>
#include <lwiot/log.h>

static FILE *dbg_file;

static void vfprint_dbg(const char *prefix, const char *fmt, va_list va)
{
	time_t tick;

	tick = lwiot_tick_ms();
#ifdef AVR
	fprintf(dbg_file, "[%lu]", (unsigned long)tick);
#elif defined(ESP8266)
	fprintf(dbg_file, "[%li]", (long)tick);
#else
	fprintf(dbg_file, "[%llu]", (unsigned long long)tick);
#endif

	fprintf(dbg_file, "%s", prefix);
	vfprintf(dbg_file, fmt, va);
}

#ifdef HAVE_DEBUG
void print_dbg(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vfprint_dbg("[lwIoT]: ", fmt, va);
	va_end(va);
}
#endif

void panic(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vfprint_dbg("[lwIoT PANIC]: ", fmt, va);
	va_end(va);
}

void log_init(const FILE *file)
{
	if(file)
		dbg_file = (FILE*)file;
	else
		dbg_file = stdout;
}
