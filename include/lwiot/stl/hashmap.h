/*
 * Hashmap implementation based on a skiplist.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file hashmap.h Hahsmap header
 */

#pragma once

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/hash.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @ingroup stl
		 * @brief A key value storage container.
		 * @tparam K Key type.
		 * @tparam V Value type.
		 */
		template <typename K, typename V>
		class HashMap {
		public:
			typedef hash_type mapped_key_type; //!< Hash type.
			typedef V value_type; //!< Value type.
			typedef K key_type; //!< Key type.

			typedef typename SkipList<mapped_key_type, value_type>::const_iterator const_iterator; //!< Const iterator type.
			typedef typename SkipList<mapped_key_type, value_type>::iterator iterator; //!< Iterator type.
			typedef typename SkipList<mapped_key_type, value_type>::size_type size_type; //!< Size type.

			/* ELEMENT ACCESS */
			/**
			 * @brief Access the specified element
			 * @param k Key to access.
			 * @return Element at \p k.
			 */
			value_type& operator[](const key_type& k)
			{
				return this->_list[stl::hash(k)];
			}

			/**
			 * @brief Access the specified element
			 * @param k Key to access.
			 * @return Element at \p k.
			 */
			value_type& operator[](key_type&& k)
			{
				auto hash = stl::hash(k);
				return this->_list[stl::move(hash)];
			}

			/**
			 * @brief Access the specified element
			 * @param key Key to access.
			 * @return Element at \p key.
			 */
			value_type& at(const key_type& key)
			{
				return this->_list.at(stl::hash(key));
			}

			/**
			 * @brief Access the specified element
			 * @param key Key to access.
			 * @return Element at \p key.
			 */
			const value_type& at(const key_type& key) const
			{
				return this->_list.at(stl::hash(key));
			}

			/* MODIFIERS */
			/**
			 * @brief Insert an element in using a key-value pair.
			 * @param kv Key-value pair.
			 * @return An iterator to the inserted item.
			 */
			iterator insert(const stl::Pair<key_type, value_type>& kv)
			{
				stl::Pair<mapped_key_type, value_type> mapped_kv(
						stl::move(stl::hash(kv.first)), stl::move(kv.second)
				);

				return this->_list.insert(stl::move(mapped_kv));
			}

			/**
			 * @brief Erase an element from the hashmap.
			 * @param pos Position to erase.
			 * @return True or false based on whether or not an element was removed.
			 */
			bool erase(const_iterator pos)
			{
				return this->_list.erase();
			}

			/**
			 * @brief Erase an element from the hashmap.
			 * @param key Key to remove.
			 * @return True or false based on whether or not an element was removed.
			 */
			bool erase(const key_type& key)
			{
				return this->_list.erase(key);
			}

			/**
			 * @brief Swap \p *this with another map.
			 * @param map Map to swap with \p *this.
			 */
			void swap(HashMap<key_type, value_type>& map)
			{
				using stl::swap;
				swap(this->_list, map);
			}

			/**
			 * @brief Clear all keys from the hashmap.
			 */
			void clear()
			{
				return this->_list.clear();
			}

			/**
			 * @brief Construct an element in place.
			 * @tparam Args Argument types.
			 * @param args Arguments to forward to the element constructor.
			 * @return Returns a pair consiting of an iterator to the element inserted and a bool indicating whether or
			 *         not an insert operation took place.
			 */
			template <typename... Args>
			stl::Pair<iterator, bool> emplace(Args&&... args)
			{
				return this->emplace_impl(stl::forward<Args>(args)...);
			}

			/* CAPACITY METHODS */

			/**
			 * @brief Get the number of keys stored.
			 * @return The number of keys stored by \p *this.
			 */
			size_type size() const
			{
				return this->_list.size();
			}

			/**
			 * @brief Check if \p *this is empty.
			 * @return True or false based on whether or not \p *this is empty.
			 */
			bool empty() const
			{
				return this->_list.empty();
			}

			/* OPERATION METHODS */
			/**
			 * @brief Find an element with \p x as key value.
			 * @param key Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			iterator find(const key_type& key)
			{
				auto hash = stl::hash(key);
				return this->_list.find(key);
			}

			/**
			 * @brief Find an element with \p x as key value.
			 * @param key Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			const_iterator find(const key_type& key) const
			{
				auto hash = stl::hash(key);
				return this->_list.find(hash);
			}

			/**
			 * @brief Find an element with \p x as key value.
			 * @tparam T Equivalent key type.
			 * @param x Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			template <typename T>
			iterator find(const T& x)
			{
				auto hash = stl::hash(x);
				return this->_list.find(hash);
			}

			/**
			 * @brief Find an element with \p x as key value.
			 * @tparam T Equivalent key type.
			 * @param x Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			template <typename T>
			const_iterator find(const T& x) const
			{
				auto hash = stl::hash(x);
				return this->_list.find(hash);
			}

			/**
			 * @brief Check if \p *this contains \p k.
			 * @param k Key to check.
			 * @return True or false based on whether or not \p *this holds \p k.
			 */
			bool contains(const key_type& k) const
			{
				return this->find(k) != this->end();
			}

			/**
			 * @brief Check if \p *this contains \p k.
			 * @tparam T Key type.
			 * @param k Key to check.
			 * @return True or false based on whether or not \p *this holds \p k.
			 */
			template <typename T>
			bool contains(const T& k) const
			{
				return this->find(k) != this->end();
			}

			/*
			 *  Iterator methods
			 */

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			iterator begin()
			{
				return this->_list.begin();
			}

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			const_iterator begin() const
			{
				return this->_list.begin();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			iterator end()
			{
				return this->_list.end();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			const_iterator end() const
			{
				return this->_list.end();
			}

#ifndef NDEBUG
			void dump()
			{
				this->_list.dump("Level %i\n\t", "%i ");
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
