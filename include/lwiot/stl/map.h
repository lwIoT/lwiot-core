/*
 * Map implementation using a linked list.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/stl/linkedlist.h>

namespace lwiot
{
	namespace stl
	{
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

			typedef typename LinkedList<Entry>::iterator iterator;
			typedef typename LinkedList<Entry>::const_iterator const_iterator;

			CONSTEXPR explicit Map() : _data()
			{
			}

			CONSTEXPR explicit Map(const Map &map) : _data(map._data)
			{
			}

			CONSTEXPR explicit Map(Map &&map) : _data(stl::move(map._data))
			{
			}

			virtual ~Map()
			{
				this->_data.clear();
			}

			Map<K, V> &operator=(const Map<K, V> &rhs)
			{
				return *this;
			}

			CONSTEXPR Map<K, V> &operator=(Map<K, V> &&rhs)
			{
				this->_data = stl::move(rhs._data);
				return *this;
			}

			CONSTEXPR void add(const MapKey &key, const MapValue &value)
			{
				Entry e(key, value);
				this->_data.push_front(e);
			}

			CONSTEXPR void add(MapKey &&key, MapValue &&value)
			{
				Entry e(stl::forward<MapKey>(key), stl::forward<MapValue>(value));
				this->_data.push_front(stl::move(e));
			}

			CONSTEXPR MapValue at(const MapKey &key) const
			{
				for(const auto &e : this->_data) {
					if(e == key) {
						return e.value;
					}
				}

				return MapValue();
			}

			CONSTEXPR MapValue operator[](MapKey idx) const
			{
				return this->at(idx);
			}

			CONSTEXPR iterator find(const MapKey &key)
			{
				for(auto &e : this->_data) {
					if(e == key) {
						return iterator(reinterpret_cast<typename lwiot::stl::LinkedList<Entry>::node_type *>(&e));
					}
				}

				return iterator(nullptr);
			}

			CONSTEXPR const_iterator find(const MapKey &key) const
			{
				for(const auto &e : this->_data) {
					if(e == key) {
						return const_iterator(reinterpret_cast<typename lwiot::stl::LinkedList<Entry>::node_type *>(&e));
					}
				}

				return const_iterator(nullptr);
			}

			CONSTEXPR iterator begin()
			{
				return this->_data.begin();
			}

			CONSTEXPR iterator end()
			{
				return this->_data.end();
			}

			CONSTEXPR const_iterator begin() const
			{
				return this->_data.begin();
			}

			CONSTEXPR const_iterator end() const
			{
				return this->_data.end();
			}

			CONSTEXPR bool contains(const MapKey &k) const
			{
				return this->contains(stl::move(k));
			}

			CONSTEXPR bool contains(const MapKey &&k) const
			{
				for(const Entry &e : this->_data) {
					if(e == k) {
						return true;
					}
				}

				return false;
			}

			CONSTEXPR void remove(const MapKey& key)
			{
				for(auto idx = this->begin(); idx != this->end(); ++idx) {
					if(idx->key == key) {
						this->_data.remove(idx.node());
						break;
					}
				}
			}

			MapValue &operator[](MapKey idx)
			{
				for(Entry &e : this->_data) {
					if(e == idx) {
						return e.value;
					}
				}

				Entry entry(idx, MapValue());
				this->_data.push_front(stl::move(entry));
				return this->_data.front().value;
			}

		private:
			LinkedList<Entry> _data;
		};
	}
}
