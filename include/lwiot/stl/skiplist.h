/*
 * Skiplist header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/util/application.h>

#include <lwiot/stl/vector.h>
#include <lwiot/stl/foreach.h>
#include <lwiot/stl/pair.h>

namespace lwiot
{
	namespace stl
	{
		namespace detail
		{
			template<typename K, typename T>
			struct SkipListNode {
				typedef K key_type;
				typedef T value_type;
				typedef size_t size_type;

				SkipListNode(const SkipListNode &other) = default;

				SkipListNode(SkipListNode &&other) noexcept = default;

				SkipListNode &operator=(const SkipListNode &rhs) = default;

				SkipListNode &operator=(SkipListNode &&rhs) noexcept = default;

				key_type _key;
				value_type _value;
				size_type _levels;
				SkipListNode *_next[1];
			};
		}

		template<typename K, typename V, typename A = DefaultAllocator<detail::SkipListNode<K,V>>>
		class SkipList {
		private:
			template<typename, typename >
			class HashMap;

			friend class HashMap<K,V>;

		public:
			typedef K key_type;
			typedef V value_type;
			typedef size_t size_type;
			typedef A allocator_type;
			typedef detail::SkipListNode<K, V> node_type;

			template<bool is_const>
			class IteratorBase {
			public:
				typedef typename traits::TypeChoice<is_const, const SkipList::value_type,
						SkipList::value_type>::type value_type;
				typedef typename traits::TypeChoice<is_const, const SkipList::key_type, SkipList::key_type>::type key_type;
				typedef typename traits::TypeChoice<is_const,
					const SkipList::node_type*, SkipList::node_type*>::type pointer_type;

				constexpr explicit IteratorBase() : _current(nullptr)
				{
				}

				constexpr explicit IteratorBase(pointer_type pos) : _current(pos)
				{
				}

				constexpr IteratorBase(const IteratorBase &other) : _current(other._current)
				{
				}

				constexpr IteratorBase(IteratorBase &&other) noexcept : _current(other._current)
				{
					other._current = nullptr;
				}

				constexpr IteratorBase &operator=(const IteratorBase &rhs)
				{
					this->_current = rhs._current;
					return *this;
				}

				constexpr IteratorBase &operator=(IteratorBase &&rhs) noexcept
				{
					this->_current = rhs._current;
					rhs._current = nullptr;
					return *this;
				}

				constexpr bool operator==(const IteratorBase &b) const
				{
					return this->_current == b._current;
				}

				constexpr bool operator!=(const IteratorBase &b) const
				{
					return this->_current != b._current;
				}

				constexpr bool operator>(const IteratorBase &b) const
				{
					if(this->_current != nullptr && b._current != nullptr)
						return this->_current->_key > b._current->_key;
					else
						return _current == nullptr && b._current != nullptr;
				}

				constexpr bool operator<(const IteratorBase &b) const
				{
					return (!(this->_current < b.curr) && (this->_current != b.curr));
				}

				constexpr bool operator>=(const IteratorBase &b) const
				{
					return ((this->_current == b.curr) || (b._current > this->_current));
				}

				constexpr bool operator<=(const IteratorBase &b) const
				{
					return ((this->_current == b.curr) || (b._current < this->_current));
				}

				constexpr IteratorBase &operator++()
				{
					if(this->_current == nullptr)
						return *this;

					this->_current = this->_current->_next[0];
					return *this;
				}

				constexpr IteratorBase &operator+=(int offset)
				{
					if(offset <= 0)
						return *this;

					for(int i = 0; i < offset; ++i) {
						++*this;
					}

					return *this;
				}

				IteratorBase operator+(int offset)
				{
					IteratorBase it = *this;
					it += offset;
					return it;
				}

				constexpr value_type& operator*()
				{
					return this->_current->_value;
				}

				constexpr value_type *operator->() const
				{
					return &this->_current->_value;
				}

				constexpr value_type *operator->()
				{
					return &this->_current->_value;
				}

			private:
				pointer_type _current;

				friend class SkipList<K, V>;
			};

			typedef IteratorBase<true> const_iterator;
			typedef IteratorBase<false> iterator;

			explicit SkipList() : _head(1), _size(0UL), _prob(DefaultProbability), _max(DefaultLevel), _alloc()
			{
			}

			SkipList(const SkipList &other) :
				_head(other._head.size()), _size(other._size), _prob(other._prob), _max(DefaultLevel), _alloc(other._alloc)
			{
				this->copy(other);
			}

			SkipList(SkipList &&other) noexcept :
				_head(stl::move(other._head)), _size(other._size), _prob(other._prob), _max(DefaultLevel),
				_alloc(stl::move(other._alloc))
			{
				other._head.assign(1, nullptr);
			}

			virtual ~SkipList()
			{
				this->freeAllNodes(this->_head[0]);
			}

			constexpr SkipList &operator=(SkipList &&rhs) noexcept
			{
				using stl::swap;
				swap(*this, rhs);
				return *this;
			}

			SkipList &operator=(const SkipList &rhs)
			{
				this->copy(rhs);
				return *this;
			}

			inline void clear()
			{
				this->freeAllNodes(this->_head[0]);
				this->_head.assign(1, nullptr);
			}

			constexpr bool empty() const noexcept
			{
				return this->_head[0] == nullptr;
			}

			iterator begin() noexcept
			{
				return iterator{this->_head[0]};
			}

			iterator end() noexcept
			{
				return iterator{nullptr};
			}

			const_iterator begin() const noexcept
			{
				return const_iterator{this->_head[0]};
			}

			const_iterator end() const noexcept
			{
				return const_iterator{nullptr};
			}

			const_iterator cbegin() const noexcept
			{
				return begin();
			}

			const_iterator cend() const noexcept
			{
				return end();
			}

			value_type & operator[] (const key_type& key)
			{
				auto iter = this->find(key);

				if(iter == this->end()) {
					V defaultvalue;
					return *(this->insert(key, defaultvalue));
				}

				return *iter;
			}

			value_type & operator[] (key_type &&key)
			{
				auto iter = this->find(key);

				if(iter == this->end()) {
					V value;
					return *(this->insert(stl::forward<key_type>(key), stl::move(value)));
				}

				return *iter;
			}

			bool operator==(const SkipList& other) const
			{
				if(this->_size != other._size)
					return false;

				const_iterator a = this->begin();
				const_iterator b = other.begin();

				for(; a != this->end(); ++a, ++b) {
					if(*a == *b)
						continue;

					return false;
				}

				return true;
			}

			bool operator!=(const SkipList& other) const
			{
				if(this->_size != other._size)
					return true;

				const_iterator a = this->begin();
				const_iterator b = other.begin();

				for(; a != this->end(); ++a, ++b) {
					if(*a == *b)
						continue;

					return true;
				}

				return false;
			}

			iterator insert(const Pair<key_type, value_type>& p)
			{
				return this->insert(p.first, p.second);
			}

			iterator insert(Pair<key_type, value_type>&& p)
			{
				return this->insert(stl::move(p.first), stl::move(p.second));
			}

			iterator insert(key_type &&key, value_type &&value)
			{
				auto node_level = this->generateLevel();
				auto new_node = this->allocateNode(
						stl::forward<key_type>(key), stl::forward<value_type>(value), node_level);

				return this->insert(new_node, node_level);
			}

			iterator insert(const key_type &key, const value_type &value)
			{
				auto node_level = this->generateLevel();
				auto new_node = this->allocateNode(key, value, node_level);

				return this->insert(new_node, node_level);
			}

			template <typename... Args>
			stl::Pair<iterator, bool> emplace(Args&&... args)
			{
				auto level = this->generateLevel();
				auto node = this->allocateNode(stl::forward<Args>(args)..., level);
				auto rv = this->insert(node, level, true);
				auto ok = rv != this->end();

				if(rv == this->end())
					this->destroyNode(node);

				return stl::Pair<iterator, bool>(rv, ok);
			}

			bool erase(const key_type &key)
			{
				node_type *node = nullptr;

				auto level = this->_head.size();
				auto next = this->_head.data();

				while(level > 0) {
					const auto link_index = level - 1;

					if(!next[link_index] || next[link_index]->_key > key) {
						--level;
					} else if(next[link_index]->_key == key) {
						node = next[link_index];
						next[link_index] = node->_next[link_index];
						--level;
					} else {
						next = next[link_index]->_next;
					}
				}

				while(this->_head.size() > 1 && this->_head.back() == nullptr)
					this->_head.popback();

				if(node) {
					this->destroyNode(node);
					this->_size -= 1;
					return true;
				} else {
					return false;
				}
			}

			value_type& at(const key_type& key)
			{
				return *this->find(key);
			}

			const value_type& at(const key_type& key) const
			{
				return *this->find(key);
			}

			iterator find(const key_type &key)
			{
				auto level = this->_head.size();
				auto next = this->_head.data();

				while(level > 0) {
					const auto index = level - 1;

					if(!next[index] || next[index]->_key > key) {
						--level;
					} else if(next[index]->_key == key) {
						return iterator{next[index]};
					} else {
						next = next[index]->_next;
					}
				}

				return this->end();
			}

			const_iterator find(const key_type &key) const
			{
				auto level = this->_head.size();
				auto next = this->_head.data();

				while(level > 0) {
					const auto index = level - 1;

					if(!next[index] || next[index]->_key > key) {
						--level;
					} else if(next[index]->_key == key) {
						return const_iterator(next[index]);
					} else {
						next = next[index]->_next;
					}
				}

				return this->end();
			}

			size_type size() const
			{
				return this->_size;
			}

#ifndef NDEBUG
			void dump(const String& formatlvl, const String& formatnode)
			{
				auto level = this->_head.size();

				for(int idx = 0; idx < level; idx++) {
					auto node = this->_head[idx];

					printf(formatlvl.c_str(), idx);

					while(node != nullptr) {
						printf(formatnode.c_str(), node->_key);
						node = node->_next[idx];
					}

					printf("\n");
				}
			}
#endif

		private:
			stl::Vector<node_type *> _head;
			size_type _size;
			float _prob;
			size_type _max;
			allocator_type _alloc;

			static constexpr double DefaultProbability = 0.5;
			static constexpr size_type DefaultLevel = 5;

			size_type generateLevel()
			{
				auto new_level = size_type{};
				bool nxtlvl;

				do {
					++new_level;
					nxtlvl = this->shouldProgress();
				} while(new_level <= this->_head.size() && nxtlvl && new_level < this->_max);

				return new_level;
			}

			bool shouldProgress()
			{
				double p = lwiot::random();

				p /= static_cast<double>(RAND_MAX);
				return p >= this->_prob;
			}

			node_type *allocateNode(key_type &&key, value_type &&value, size_type levels)
			{
				const auto node = this->_alloc.allocate(levels);

				new(node) node_type{stl::forward<key_type>(key), stl::forward<value_type>(value), levels, {nullptr}};
				return reinterpret_cast<node_type *>(node);
			}

			node_type *allocateNode(const key_type &key, const value_type &value, size_type levels)
			{
				const auto node = this->_alloc.allocate(levels);

				new(node) node_type{key, value, levels, {nullptr}};
				return reinterpret_cast<node_type *>(node);
			}

			void destroyNode(node_type *node)
			{
				if(node == nullptr)
					return;

				this->_alloc.destroy(node);
				this->_alloc.deallocate(node, 1);
			}

			void freeAllNodes(node_type *head)
			{
				for(auto index = head; index != nullptr;) {
					const auto temp = index;
					index = index->_next[0];
					this->destroyNode(temp);
				}

				this->_size = 0;
			}

			friend auto swap(SkipList &a, SkipList &b) noexcept
			{
				using stl::swap;
				swap(a._head, b._head);
				swap(a._size, b._size);
				swap(a._prob, b._prob);
				swap(a._max,  b._max);
				swap(a._alloc, b._alloc);
			}

			iterator insert(node_type *new_node, size_t node_level, bool once = false)
			{
				node_type *old = nullptr;

				while(this->_head.size() < node_level)
					this->_head.push_back(nullptr);

				auto level = this->_head.size();
				auto next = this->_head.data();

				while(level > 0) {
					const auto index = level - 1;
					auto node = next[index];

					if(node == nullptr || node->_key > new_node->_key) {
						if(level <= node_level) {
							new_node->_next[index] = next[index];
							next[index] = new_node;

						}

						--level;
					} else if(node->_key == new_node->_key) {
						if(once)
							return this->end();

						if(node->_levels >= node_level) {
							node->_value = stl::move(new_node->_value);
							this->destroyNode(node);

							return iterator(node);
						}

						old = node;

						new_node->_next[index] = node->_next[index];
						next[index] = new_node;
						--level;
					} else {
						next = node->_next;
					}
				}

				this->_size += 1;

				if(old != nullptr) {
					this->destroyNode(old);
					this->_size -= 1;
				}

				return iterator(new_node);
			}

			void copy(const SkipList &other)
			{
				this->_alloc = other._alloc;
				this->_head.assign(other._head.size(), nullptr);

				auto tail = stl::Vector<node_type **>{};

				tail.reserve(this->_head.size());
				stl::foreach(this->_head, [&](auto &iter) {
					tail.push_back(&(*iter));
				});

				for(node_type *node = other._head[0]; node != nullptr; node = node->_next[0]) {
					const auto copy_node = this->allocateNode(node->_key, node->_value, node->_levels);

					for(auto i = 0U; i < copy_node->_levels; ++i) {
						*tail[i] = copy_node;
						tail[i] = &copy_node->_next[i];
					}
				}

				stl::foreach(tail, [](auto &iter) {
					*iter = nullptr;
				});

				this->_size = other._size;
				this->_prob = other._prob;
				this->_max  = other._max;
			}
		};
	}
}
