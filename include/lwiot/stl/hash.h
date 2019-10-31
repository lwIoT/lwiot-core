/*
 * Hash functions.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <lwiot/stl/string.h>
#include <lwiot/traits/isscalar.h>

namespace lwiot
{
	namespace stl
	{
		typedef uint64_t hash_type;

		namespace detail
		{
			static constexpr uint64_t FNV_offset = 14695981039346656037ULL;
			static constexpr uint64_t FNV_prime  = 1099511628211ULL;

			inline hash_type hash(const uint8_t* data, size_t length)
			{
				auto hash = FNV_offset;

				for(size_t idx = 0U; idx < length; idx++) {
					hash  ^= data[idx];
					hash  *= FNV_prime;
				}

				return hash;
			}
		}

		template <typename T>
		inline hash_type hash(T value)
		{
			auto data = reinterpret_cast<const uint8_t *>(&value);
			constexpr auto size = sizeof(T);

			return detail::hash(data, size);
		}

		inline hash_type hash(const char* value)
		{
			auto data = reinterpret_cast<const uint8_t *>(value);
			return detail::hash(data, strlen(value));
		}

		inline hash_type hash(const stl::String& value)
		{
			return detail::hash(reinterpret_cast<const uint8_t *>(value.c_str()), value.length());
		}
	}
}
