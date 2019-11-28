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
#include <lwiot/kernel/port.h>

#ifdef UNUSED
#undef UNUSED
#endif

#define UNUSED(x) (void)x

#define CXX __cplusplus

#if !defined(BIT) && !defined(ESP32) && !defined(ESP8266)
#define BIT(__x) (1U << __x)
#endif

#ifdef __cplusplus
namespace lwiot
{
	namespace stl
	{
		class String;
	}

	/**
	 * @brief Generate a random string.
	 * @param prefix Prefix.
	 * @param size Number of characters to generate.
	 * @return A randommly generated string.
	 */
	stl::String randstr(const stl::String& prefix, size_t size);

#define RANDOM_MIN = 0x7FFF;

	/**
	 * @brief Generate a random integer.
	 * @return A random integer.
	 */
	inline int random()
	{
		return rand();
	}
}
#endif

#define BITS_PER_BYTE 8U
