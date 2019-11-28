/*
 * Set implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <lwiot/stl/skiplist.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @ingroup stl
		 * @brief Associative container contains a set of unique objects of type \p T.
		 * @tparam T Key type.
		 */
		template <typename T>
		class Set {
		public:
			typedef T key_type; //!< Key type.
			typedef typename SkipList<T,bool>::iterator iterator; //!< Iterator type.
			typedef typename SkipList<T,bool>::iterator const_iterator; //!< Const iterator type.

			/**
			 * @brief Insert a new key.
			 * @param value Value to insert.
			 * @return Pair object containing an iterator to the inserted object and a boolean indicating of an insert
			 *         operation has taken place.
			 */
			stl::Pair<iterator, bool> insert(const key_type& value)
			{
				if(this->contains(value))
					return stl::Pair<iterator, bool>(this->_set.end(), false);

				stl::Pair<iterator, bool> p(this->_set.insert(value, true), true);
				return p;
			}

			/**
			 * @brief Check if \p value is a member of \p *this.
			 * @param value Value to check.
			 * @return True or false based on whether or not \p value is a member of \p *this.
			 */
			bool contains(const key_type& value) const
			{
				auto rv = this->_set.find(value);
				return !(rv == this->_set.end());
			}

			/**
			 * @brief Construct an element in-place.
			 * @tparam Args Argument types.
			 * @param args Arguments forwarded to the element constructor.
			 * @return Pair object containing an iterator to the inserted object and a boolean indicating of an insert
			 *         operation has taken place.
			 */
			template <typename... Args>
			stl::Pair<iterator, bool> emplace(Args&&... args)
			{
				return this->_set.emplace(stl::forward<Args>(args)..., true);
			}

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			iterator begin()
			{
				return this->_set.begin();
			}

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			const_iterator begin() const
			{
				return this->_set.begin();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			iterator end()
			{
				return this->_set.end();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			const_iterator end() const
			{
				return this->_set.end();
			}

			/**
			 * @brief Inequality operator.
			 * @param other Set to compare against \p *this.
			 * @return \p *this \f$ \neq \f$ \p other
			 */
			bool operator!=(const Set& other) const
			{
				return !(*this == other);
			}

			/**
			 * @brief Equality operator.
			 * @param other Set to compare against \p *this.
			 * @return \p *this \f$ \equiv \f$ \p other
			 */
			bool operator==(const Set& other) const
			{
				if(this->_set.size() != other._set.size())
					return false;

				for(auto& value : this->_set) {
					auto v = other.contains(value);

					if(!v)
						return false;
				}

				return true;
			}

			/**
			 * @brief Clear all keys from \p *this.
			 */
			void clear()
			{
				this->_set.clear();
			}

		private:
			stl::SkipList<key_type, bool> _set;
		};
	}
}
