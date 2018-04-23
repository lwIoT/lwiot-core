/*
 * lwIoT logging header
 *
 * Author: Michel Megens
 * Date: 04/12/2017
 * Email: dev@bietje.net
 */

#ifndef __LOG_H__
#define __LOG_H__


#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>

#ifdef HAVE_DEBUG
CDECL
extern DLL_EXPORT void print_dbg(const char *fmt, ...);
CDECL_END
#else
#define print_dbg(x, ...)
#endif

CDECL
extern DLL_EXPORT void log_init(const FILE *file);
extern DLL_EXPORT void panic(const char *fmt, ...);
CDECL_END

#endif // !__LOG_H__
