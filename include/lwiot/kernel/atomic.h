/*
 * Global atomic header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>
#include <lwiot.h>

#ifdef HAVE_SYNC_FETCH
#include <lwiot/detail/atomic_sync.h>
#else
#include <lwiot/detail/atomic_crit.h>
#endif

namespace lwiot
{
	typedef lwiot::Atomic<uint8_t> atomic_uint8_t;
	typedef lwiot::Atomic<uint16_t> atomic_uint16_t;
	typedef lwiot::Atomic<uint32_t> atomic_uint32_t;
	typedef lwiot::Atomic<uint64_t> atomic_uint64_t;

	typedef lwiot::Atomic<short> atomic_short_t;
	typedef lwiot::Atomic<int> atomic_int_t;
	typedef lwiot::Atomic<long> atomic_long_t;
	typedef lwiot::Atomic<long long> atomic_long_long_t;

	class AtomicBool : public lwiot::detail::Atomic<uint8_t> {
	public:
		explicit AtomicBool(bool value = false) : detail::Atomic<uint8_t>((uint8_t)value)
		{
		}

		AtomicBool& operator=(bool v) noexcept
		{
			this->store(uint8_t(v));

			return *this;
		}

		AtomicBool& operator=(const AtomicBool& ab)
		{
			bool v = ab.load() != 0;
			*this = v;

			return *this;
		}

		explicit operator bool() noexcept
		{
			bool value = this->load();
			return value != 0;
		}
	};

	typedef AtomicBool atomic_bool_t;
}
