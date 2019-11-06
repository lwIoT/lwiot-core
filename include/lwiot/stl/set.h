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
		template <typename T>
		class Set {
		public:
			typedef T key_type;
			typedef T value_type;
			typedef typename SkipList<T,bool>::iterator iterator;
			typedef typename SkipList<T,bool>::iterator const_iterator;

			stl::Pair<iterator, bool> insert(const key_type& value)
			{
				if(this->_set.at(value))
					return stl::Pair<iterator, bool>(this->_set.end(), false);

				stl::Pair<iterator, bool> p(this->_set.insert(value, true), true);
				return p;
			}

			stl::Pair<const_iterator, bool> insert(const key_type& value) const
			{
				if(this->_set.at(value))
					return stl::Pair<const_iterator, bool>(this->_set.end(), false);

				stl::Pair<const_iterator, bool> p(this->_set.insert(value, true), true);
				return p;
			}

			bool contains(const key_type& value) const
			{
				auto rv = this->_set.find(value);
				return !(rv == this->_set.end());
			}

			template <typename... Args>
			stl::Pair<iterator, bool> emplace(Args&&... args)
			{
				return this->_set.emplace(stl::forward<Args>(args)..., true);
			}

		private:
			stl::SkipList<key_type, bool> _set;
		};
	}
}
