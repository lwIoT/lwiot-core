/*
 * lwIoT - Compiler header
 *
 * Author: Michel Megens
 * Date: 12/12/2017
 * Email: dev@bietje.net
 */

#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <lwiot_opts.h>

#ifdef WIN32
#include <lwiot/compiler-vc.h>
#else
#include <lwiot/compiler-gcc.h>
#endif

#define likely(x) __compiler_likely(x)
#define unlikely(x) __compiler_unlikely(x)

#define container_of(ptr, type, entry) __compiler_co(ptr, type, entry)

#ifndef __cplusplus

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
typedef char bool;

#define false 0
#define true !false
#endif

#endif

#ifdef __cplusplus
#ifdef NULL
#undef NULL
#define NULL nullptr
#endif

#define CDECL extern "C" {
#define CDECL_END }
#else
#define CDECL
#define CDECL_END
#endif

#if __compiler_barrier && CONFIG_HW_BARRIER
#define barrier() __compiler_barrier()
#else
#define barrier() __asm__ __volatile__ ("" ::: "memory")
#endif

#ifdef __cplusplus
#ifdef AVR
template<typename T>
constexpr void *operator new(unsigned int s, T *v)
{
	return v;
}
#else

#include <stddef.h>
template<typename T>
constexpr void *operator new(size_t s, T *v)
{
	return v;
}
#endif
#endif

#endif
