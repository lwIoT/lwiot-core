/*
 * Map implementation using a linked list.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/linkedlist.h>

namespace lwiot
{
	template <typename K, typename V>
	class Map {
	public:
		typedef K MapKey;
		typedef V MapValue;

		struct Entry {
		public:
			Entry(const MapKey& key, const MapValue& value) : key(key), value(value)
			{ }

			Entry(MapKey&& key, MapValue&& value) noexcept :
				key(stl::forward<MapKey>(key)), value(stl::forward<MapValue>(value))
			{ }

			bool operator==(const Entry& rhs) const
			{
				return this->key == rhs.key;
			}

			bool operator!=(const Entry& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator!=(const MapKey & rhs) const
			{
				return !(*this == rhs);
			}

			bool operator==(const MapKey & rhs) const
			{
				return this->key == rhs;
			}

			MapKey key;
			MapValue value;
		};

		typedef typename LinkedList<Entry>::Iterator Iterator;
		typedef const typename LinkedList<Entry>::Iterator const_iterator;

		constexpr explicit Map() : _data()
		{ }

		constexpr explicit Map(const Map& map) : _data(map._data)
		{ }

		constexpr explicit Map(Map&& map) : _data(stl::move(map._data))
		{ }

		virtual ~Map()
		{
			this->_data.clear();
		}

		Map<K, V>& operator=(const Map<K,V>& rhs)
		{
			return *this;
		}

		constexpr Map<K, V>& operator=(Map<K,V>&& rhs)
		{
			this->_data = stl::move(rhs._data);
			return *this;
		}

		constexpr void add(const MapKey& key, const MapValue& value)
		{
			Entry e(key, value);
			this->_data.push_front(e);
		}

		constexpr void add(MapKey&& key, MapValue &&value)
		{
			Entry e(stl::forward<MapKey>(key), stl::forward<MapValue>(value));
			this->_data.push_front(stl::move(e));
		}

		constexpr MapValue at(const MapKey&& key) const
		{
			for(const auto& e : this->_data) {
				if(e == key) {
					return e.value;
				}
			}

			return MapValue();
		}

		constexpr MapValue operator[](MapKey idx) const
		{
			return this->at(idx);
		}

		constexpr Iterator find(const MapKey& key)
		{
			for(auto& e : this->_data) {
				if(e == key) {
					return Iterator(reinterpret_cast<typename lwiot::LinkedList<Entry>::node_type*>(&e));
				}
			}

			return Iterator(nullptr);
		}

		constexpr const_iterator find(const MapKey& key) const
		{
			for(const auto& e : this->_data) {
				if(e == key) {
					return Iterator(reinterpret_cast<typename lwiot::LinkedList<Entry>::node_type*>(&e));
				}
			}

			return Iterator(nullptr);
		}

		constexpr Iterator begin() const
		{
			return this->_data.begin();
		}

		constexpr Iterator end() const
		{
			return this->_data.end();
		}

		constexpr bool contains(const MapKey& k) const
		{
			return this->contains(stl::move(k));
		}

		constexpr bool contains(const MapKey&& k) const
		{
			for(Entry& e : this->_data) {
				if(e == k) {
					return true;
				}
			}

			return false;
		}

		constexpr void remove(const MapKey& key)
		{
			this->remove(stl::move(key));
		}

		constexpr void remove(const MapKey&& key)
		{
			for(auto idx = this->begin(); idx != this->end(); ++idx) {
				if(idx->key == key) {
					this->_data.remove(idx.node());
					break;
				}
			}
		}

		MapValue& operator[](MapKey idx)
		{
			for(Entry& e : this->_data) {
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
