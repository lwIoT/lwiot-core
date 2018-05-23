/*
 * Countable object interface.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#ifdef CXX
namespace lwiot
{
	template <typename T> class Countable {
	public:
		virtual ~Countable() = default;

		virtual size_t count()
		{
			return this->_count;
		}

		virtual inline const size_t& count() const
		{
			return this->_count;
		}

		virtual const T& operator[] (const size_t& idx) const = 0;
		virtual T& operator[] (const size_t& idx) = 0;

		virtual const T& at(const size_t& idx) const
		{
			return operator[](idx);
		}

	protected:
		explicit Countable() : Countable(0) { }

		explicit Countable(const size_t& count) : _count(count)
		{ }

		virtual void grow(const size_t& num)
		{
			this->_count += num;
		}

		size_t _count;
	};
}
#endif
