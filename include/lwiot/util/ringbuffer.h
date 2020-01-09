/*
 * Ring buffer implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/detail/ringbuffer.h>

namespace lwiot
{
	template <typename T, stl::size_t S>
	using RingBuffer = detail_rb::RingBuffer<T, S>;

	constexpr stl::size_t DefaultRingBufferSize = 256;
	using DefaultRingBuffer = RingBuffer<uint8_t, DefaultRingBufferSize>;
}
