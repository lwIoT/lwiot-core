/*
 * STL atomic wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <atomic>

namespace lwiot
{

	template <typename T>
	using Atomic = std::atomic<T>;

	typedef std::memory_order memory_order;
}
