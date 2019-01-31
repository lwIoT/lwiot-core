/*
 * Global atomic header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#ifdef HAVE_SYNC_FETCH
#include "detail/atomic_sync.h"
#else
#include "detail/atomic_crit.h"
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
}
