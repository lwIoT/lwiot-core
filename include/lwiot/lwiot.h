/*
 * lwIoT project header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot_opts.h>

#include <lwiot/compiler.h>
#include <lwiot/types.h>
#include <lwiot/port.h>

#ifdef UNUSED
#undef UNUSED
#endif

#define UNUSED(x) (void)x

#define CXX __cplusplus

#if !defined(BIT) && !defined(ESP32)
#define BIT(__x) (1U << __x)
#endif

#define BITS_PER_BYTE 8U

#ifdef __cplusplus
namespace lwiot
{
	namespace lib
	{
		template <typename T>
		static inline T& move(T& obj)
		{
			return obj;
		}

		template <typename T>
		static inline void swap(T& a, T& b)
		{
			T tmp = move(a);

			a = b;
			b = tmp;
		}
	}
}
#endif
