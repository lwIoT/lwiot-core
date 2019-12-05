/*
 * Global atomic header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file atomic.h

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
	/**
	 * @brief Atomic boolean type.
	 * @ingroup kernel
	 */
	class AtomicBool : public lwiot::detail::Atomic<uint8_t> {
	public:
		/**
		 * @brief Construct a new atomic boolean type.
		 * @param value Initial value.
		 */
		explicit AtomicBool(bool value = false) : detail::Atomic<uint8_t>((uint8_t)value)
		{
		}

		/**
		 * @brief Atomically assign a new value.
		 * @param v Value to assign.
		 * @return A reference to \p *this.
		 */
		AtomicBool& operator=(bool v) noexcept
		{
			this->store(uint8_t(v));

			return *this;
		}

		/**
		 * @brief Atomically assign a new value.
		 * @param ab Value to assign.
		 * @return A reference to \p *this.
		 */
		AtomicBool& operator=(const AtomicBool& ab)
		{
			bool v = ab.load() != 0;
			*this = v;

			return *this;
		}

		/**
		 * @brief Atomically load the boolean value.
		 * @return The current value of \p *this.
		 */
		operator bool() noexcept
		{
			bool value = this->load();
			return value != 0;
		}
	};

	/**
	 * @addtogroup kernel
	 * @{
	 */
	typedef lwiot::Atomic<uint8_t> atomic_uint8_t; //!< 8-bit atomic type.
	typedef lwiot::Atomic<uint16_t> atomic_uint16_t; //!< 16-bit atomic type.
	typedef lwiot::Atomic<uint32_t> atomic_uint32_t; //!< 32-bit atomic type.
	typedef lwiot::Atomic<uint64_t> atomic_uint64_t; //!< 64-bit atomic type.

	typedef lwiot::Atomic<short> atomic_short_t; //!< Short atomic type.
	typedef lwiot::Atomic<int> atomic_int_t; //!< Integer atomic type.
	typedef lwiot::Atomic<long> atomic_long_t; //!< Long atomic type.
	typedef lwiot::Atomic<long long> atomic_long_long_t; //!< Long long atomic type.
	typedef AtomicBool atomic_bool_t; //!< Atomic boolean type.
	/** @} */
}
