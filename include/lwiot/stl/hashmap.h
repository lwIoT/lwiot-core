/*
 * Hashmap implementation based on a skiplist.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/hash.h>
#include <map>

namespace lwiot
{
	namespace stl
	{
		template <typename K, typename V>
		class HashMap {
		public:
			typedef hash_type mapped_key_type;
			typedef V value_type;
			typedef K key_type;

			typedef typename SkipList<mapped_key_type, value_type>::const_iterator const_iterator;
			typedef typename SkipList<mapped_key_type, value_type>::iterator iterator;
			typedef typename SkipList<mapped_key_type, value_type>::size_type size_type;

			/* ELEMENT ACCESS */
			value_type& operator[](const key_type& k)
			{
				return this->_list[stl::hash(k)];
			}

			value_type& operator[](key_type&& k)
			{
				auto hash = stl::hash(k);
				return this->_list[stl::move(hash)];
			}

			value_type& at(const key_type& key)
			{
				return this->_list.at(stl::hash(key));
			}

			value_type& at(const key_type& key) const
			{
				return this->_list.at(stl::hash(key));
			}

			/* MODIFIERS */
			iterator insert(const stl::Pair<key_type, value_type>& kv)
			{
				stl::Pair<mapped_key_type, value_type> mapped_kv(
						stl::move(stl::hash(kv.first)), stl::move(kv.second)
				);

				return this->_list.insert(stl::move(mapped_kv));
			}

			bool erase(const_iterator pos)
			{
				return this->_list.erase();
			}

			bool erase(const key_type& key)
			{
				return this->_list.erase(key);
			}

			void swap(HashMap<key_type, value_type>& map)
			{
				using stl::swap;
				swap(this->_list, map);
			}

			void clear()
			{
				return this->_list.clear();
			}

			template <typename... Args>
			stl::Pair<iterator, bool> emplace(Args&&... args)
			{
				return this->emplace_impl(stl::forward<Args>(args)...);
			}

			/* CAPACITY METHODS */

			size_type size() const
			{
				return this->_list.size();
			}

			bool empty() const
			{
				return this->_list.empty();
			}

			/* OPERATION METHODS */
			iterator find(const key_type& key)
			{
				auto hash = stl::hash(key);
				return this->_list.find(key);
			}

			const_iterator find(const key_type& key) const
			{
				auto hash = stl::hash(key);
				return this->_list.find(hash);
			}

			template <typename T>
			iterator find(const T& x)
			{
				auto hash = stl::hash(x);
				return this->_list.find(hash);
			}

			template <typename T>
			const_iterator find(const T& x) const
			{
				auto hash = stl::hash(x);
				return this->_list.find(hash);
			}

			bool contains(const key_type& k) const
			{
				return this->find(k) != this->end();
			}

			template <typename T>
			bool contains(const T& k) const
			{
				return this->find(k) != this->end();
			}

			/*
			 *  Iterator methods
			 */

			iterator begin()
			{
				return this->_list.begin();
			}

			const_iterator begin() const
			{
				return this->_list.begin();
			}

			iterator end()
			{
				return this->_list.end();
			}

			const_iterator end() const
			{
				return this->_list.end();
			}

#ifndef NDEBUG
			void dump()
			{
				this->_list.dump();
			}
#endif

		private:
			SkipList<mapped_key_type, value_type> _list;

			stl::Pair<iterator, bool> emplace_impl(key_type&& k, value_type&& v)
			{
				auto hash = lwiot::stl::hash(k);
				return this->_list.emplace(stl::move(hash), stl::forward<value_type>(v));
			}

			stl::Pair<iterator, bool> emplace_impl(const char* k, value_type&& v)
			{
				auto hash = lwiot::stl::hash(k);
				return this->_list.emplace(stl::move(hash), stl::forward<value_type>(v));
			}
		};
	}
}
