/*
 * Map implementation using a linked list.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

/// @file map.h

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/stl/linkedlist.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @ingroup stl
		 * @brief Ordered map.
		 * @tparam K Key type.
		 * @tparam V Value type.
		 */
		template<typename K, typename V>
		class Map {
		public:
			typedef K MapKey;
			typedef V MapValue;

			struct Entry {
			public:
				Entry(const MapKey &key, const MapValue &value) : key(key), value(value)
				{
				}

				Entry(MapKey &&key, MapValue &&value) noexcept :
						key(stl::forward<MapKey>(key)), value(stl::forward<MapValue>(value))
				{
				}

				bool operator==(const Entry &rhs) const
				{
					return this->key == rhs.key;
				}

				bool operator!=(const Entry &rhs) const
				{
					return !(*this == rhs);
				}

				bool operator!=(const MapKey &rhs) const
				{
					return !(*this == rhs);
				}

				bool operator==(const MapKey &rhs) const
				{
					return this->key == rhs;
				}

				MapKey key;
				MapValue value;
			};

			typedef typename LinkedList<Entry>::iterator iterator; //!< Iterator type.
			typedef typename LinkedList<Entry>::const_iterator const_iterator; //!< Const iterator.

			/**
			 * @brief Construct a new map.
			 */
			CONSTEXPR explicit Map() : _data()
			{
			}

			/**
			 * @brief Construct a new map.
			 * @param map Map to copy into \p *this.
			 */
			CONSTEXPR Map(const Map &map) : _data(map._data)
			{
			}

			/**
			 * @brief Construct a new map.
			 * @param map Map to move into \p *this.
			 */
			CONSTEXPR Map(Map &&map) noexcept : _data(stl::move(map._data))
			{
			}

			/**
			 * @brief Destruct a map.
			 */
			virtual ~Map()
			{
				this->_data.clear();
			}

			/**
			 * @brief Clear a map.
			 */
			void clear()
			{
				this->_data.clear();
			}

			/**
			 * @brief Copy assignment operator.
			 * @param rhs Map to copy into *this.
			 * @return A reference to \p *this.
			 */
			Map<K, V> &operator=(const Map<K, V> &rhs)
			{
				this->_data = rhs._data;
				return *this;
			}

			/**
			 * @brief Copy assignment operator.
			 * @param rhs Map to move into *this.
			 * @return A reference to \p *this.
			 */
			CONSTEXPR Map<K, V> &operator=(Map<K, V> &&rhs) noexcept
			{
				this->_data = stl::move(rhs._data);
				return *this;
			}

			/**
			 * @brief Add a key-value pair to *this.
			 * @param key Key value.
			 * @param value Value to add.
			 */
			CONSTEXPR void add(const MapKey &key, const MapValue &value)
			{
				for(auto& entry : this->_data) {
					if(entry == key) {
						entry.value = value;
						return;
					}
				}

				Entry e(key, value);
				this->_data.push_front(e);
			}

			/**
			 * @brief Add a key-value pair to *this.
			 * @param key Key value.
			 * @param value Value to add.
			 */
			CONSTEXPR void add(MapKey &&key, MapValue &&value)
			{
				for(auto& entry : this->_data) {
					if(entry == key) {
						entry.value = stl::move(value);
						return;
					}
				}

				Entry e(stl::forward<MapKey>(key), stl::forward<MapValue>(value));
				this->_data.push_front(stl::move(e));
			}

			/**
			 * @brief Access the specified element
			 * @param key Key to access.
			 * @return Element at \p key.
			 */
			CONSTEXPR MapValue at(const MapKey &key) const
			{
				for(const auto &e : this->_data) {
					if(e == key) {
						return e.value;
					}
				}

				return MapValue();
			}

			/**
			 * @brief Access the specified element
			 * @param key Key to access.
			 * @return A reference to the element at \p key.
			 */
			const MapValue& operator[](MapKey key) const
			{
				return this->at(key);
			}

			/**
			 * @brief Access the specified element
			 * @param key Key to access.
			 * @return A reference to the element at \p key.
			 */
			MapValue &operator[](MapKey key)
			{
				for(Entry &e : this->_data) {
					if(e == key) {
						return e.value;
					}
				}

				Entry entry(key, MapValue());
				this->_data.push_front(stl::move(entry));
				return this->_data.front().value;
			}

			/**
			 * @brief Find an element with \p x as key value.
			 * @param key Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			CONSTEXPR iterator find(const MapKey &key)
			{
				for(auto &e : this->_data) {
					if(e == key) {
						return iterator(reinterpret_cast<typename lwiot::stl::LinkedList<Entry>::node_type *>(&e));
					}
				}

				return iterator(nullptr);
			}

			/**
			 * @brief Find an element with \p x as key value.
			 * @param key Key value.
			 * @return An iterator to the key value \p x or and iterator to end.
			 */
			CONSTEXPR const_iterator find(const MapKey &key) const
			{
				for(const auto &e : this->_data) {
					if(e == key) {
						return const_iterator(reinterpret_cast<typename lwiot::stl::LinkedList<Entry>::node_type *>(&e));
					}
				}

				return const_iterator(nullptr);
			}

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			CONSTEXPR iterator begin()
			{
				return this->_data.begin();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			CONSTEXPR iterator end()
			{
				return this->_data.end();
			}

			/**
			 * @brief Get an iterator to the beginning of \p *this.
			 * @return An iterator to the beginning of \p *this.
			 */
			CONSTEXPR const_iterator begin() const
			{
				return this->_data.begin();
			}

			/**
			 * @brief Get an iterator to the end of \p *this.
			 * @return An iterator to the end of \p *this.
			 */
			CONSTEXPR const_iterator end() const
			{
				return this->_data.end();
			}

			/**
			 * @brief Check if \p *this contains \p k.
			 * @param k Key to check.
			 * @return True or false based on whether or not \p *this holds \p k.
			 */
			CONSTEXPR bool contains(const MapKey &k) const
			{
				return this->contains(stl::move(k));
			}

			/**
			 * @brief Check if \p *this contains \p k.
			 * @param k Key to check.
			 * @return True or false based on whether or not \p *this holds \p k.
			 */
			CONSTEXPR bool contains(const MapKey &&k) const
			{
				for(const Entry &e : this->_data) {
					if(e == k) {
						return true;
					}
				}

				return false;
			}

			/**
			 * @brief Remove an element from the hashmap.
			 * @param key Key to remove.
			 */
			CONSTEXPR void remove(const MapKey& key)
			{
				for(auto idx = this->begin(); idx != this->end(); ++idx) {
					if(idx->key == key) {
						this->_data.erase(idx);
						break;
					}
				}
			}
		private:
			LinkedList<Entry> _data;
		};
	}
}
