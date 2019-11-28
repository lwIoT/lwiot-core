/*
 * Countable object interface.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file countable.h

#pragma once

#include <lwiot.h>

#ifdef __cplusplus
namespace lwiot
{
	/**
	 * @brief Countable interface.
	 * @ingroup util
	 * @tparam T Element type.
	 */
	template <typename T>
	class Countable {
	public:
		virtual ~Countable() = default;

		/**
		 * @brief Get the count.
		 * @return The count.
		 */
		virtual size_t count()
		{
			return this->_count;
		}

		/**
		 * @brief Get the count.
		 * @return The count.
		 */
		virtual inline const size_t& count() const
		{
			return this->_count;
		}

		/**
		 * @brief Access operator.
		 * @return A reference to the object at \p idx.
		 */
		virtual const T& operator[] (const size_t& idx) const = 0;

		/**
		 * @brief Access operator.
		 * @return A reference to the object at \p idx.
		 */
		virtual T& operator[] (const size_t& idx) = 0;

		/**
		 * @brief Access operator.
		 * @return A reference to the object at \p idx.
		 */
		virtual const T& at(const size_t& idx) const
		{
			return operator[](idx);
		}

	protected:
		/**
		 * @brief Construct a new object.
		 */
		explicit Countable() : Countable(0) { }

		/**
		 * @brief Construct a new object.
		 * @param count Initial value.
		 */
		explicit Countable(const size_t& count) : _count(count)
		{ }

		/**
		 * @brief Increment the count.
		 * @param num Number of increments.
		 */
		virtual void grow(const size_t& num)
		{
			this->_count += num;
		}

		size_t _count; //!< Internal counter.
	};
}
#endif
