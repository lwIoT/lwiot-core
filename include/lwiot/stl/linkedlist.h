/*
 * Linked list header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <assert.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/container_of.h>
#include <lwiot/stl/foreach.h>

#include <lwiot/traits/typechoice.h>

#include <lwiot/util/defaultallocator.h>

namespace lwiot
{
	namespace stl
	{
		template<typename T, typename A>
		class LinkedList;

		namespace list
		{
			class Iterator;

			template<typename T>
			struct Node {
				explicit Node(const T &data) : _data(data), _next(nullptr), _prev(nullptr)
				{
				}

				explicit Node(T &&data) : _data(stl::forward<T>(data)), _next(nullptr), _prev(nullptr)
				{
				}

				constexpr bool operator==(const Node<T> &other)
				{
					return this->_data == other._data && this->_next == other._next && this->_prev == other._prev;
				}

				T _data;

				Node<T> *_next;
				Node<T> *_prev;
			};
		}

		template<typename T, typename A = DefaultAllocator<list::Node<T>>>
		class LinkedList {
		public:
			typedef T value_type;
			typedef list::Node<value_type> node_type;
			typedef A allocator_type;

			template<bool is_const>
			class Iterator {
			public:
				typedef typename traits::TypeChoice<is_const, const value_type &, value_type &>::type reference;
				typedef typename traits::TypeChoice<is_const, const value_type *, value_type *>::type pointer;

				constexpr explicit Iterator() : Iterator(nullptr)
				{
				}

				constexpr explicit Iterator(node_type *node) : _current(node), _start(node), _next(nullptr)
				{
					if(node)
						this->_next = node->_next;
				}

				constexpr Iterator &operator=(LinkedList::node_type *node)
				{
					this->_current = node;
					this->_start = node;

					if(node)
						this->_next = node->_next;

					return *this;
				}

				explicit constexpr operator bool() const
				{
					return this->_current != nullptr;
				}

				constexpr Iterator &operator++()
				{
					if(this->_current == nullptr)
						return *this;

					if((this->_next == this->_start) || (this->_next == this->_current)) {
						this->_current = nullptr;
						return *this;
					}

					this->_current = this->_next;
					this->_next = this->_next->_next;
					return *this;
				}

				constexpr const Iterator operator++(int)
				{
					Iterator iter = *this;

					++*this;
					return iter;
				}

				constexpr bool operator==(const Iterator &iter) const
				{
					return this->_current == iter._current;
				}

				constexpr bool operator!=(const Iterator &iter) const
				{
					return !(*this == iter);
				}

				constexpr reference operator*() const
				{
					assert(this->_current);
					return this->_current->_data;
				}

				constexpr pointer operator->() const
				{
					assert(this->_current);
					return &this->_current->_data;
				}

				constexpr node_type *node() const
				{
					return this->_current;
				}

				constexpr void clear()
				{
					this->_current = nullptr;
					this->_start = nullptr;
					this->_next = nullptr;
				}

			private:
				friend class LinkedList;
				node_type *_current;
				node_type *_start;
				node_type *_next;
			};

			typedef Iterator<false> iterator;
			typedef Iterator<true> const_iterator;

			constexpr explicit LinkedList() : _head(nullptr), _size(0UL), _alloc()
			{
			}

			LinkedList(const LinkedList &other) : _head(nullptr), _size(0UL), _alloc(other._alloc)
			{
				this->copy(other);
			}

			constexpr LinkedList(LinkedList &&other) noexcept : _head(nullptr), _size(0), _alloc(stl::move(other._alloc))
			{
				this->_size = other._size;
				this->_head = other._head;

				other._head = nullptr;
				other._size = 0UL;
			}

			virtual ~LinkedList()
			{
				this->clear();
			}

			LinkedList &operator=(const LinkedList<T> &rhs)
			{
				this->copy(rhs);
				return *this;
			}

			LinkedList &operator=(LinkedList<T> &&rhs) noexcept
			{
				if(this->_size > 0UL)
					this->clear();

				this->_size = rhs._size;
				this->_head = rhs._head;
				this->_alloc = stl::move(rhs._alloc);

				rhs._head = nullptr;
				rhs._size = 0UL;
				return *this;
			}

			constexpr value_type replace(iterator& iter, value_type&& value)
			{
				auto node = iter.node();

				if(node == this->_head) {
					iter._start = node;
				}

				return stl::exchange(node->_data, stl::forward<value_type &&>(value));
			}

			/* ITERATORS */

			constexpr iterator begin()
			{
				return iterator(this->_head);
			}

			constexpr const_iterator begin() const
			{
				return const_iterator(this->_head);
			}

			constexpr iterator end()
			{
				return iterator(nullptr);
			}

			constexpr const_iterator end() const
			{
				return const_iterator(nullptr);
			}

			/* ELEMENT ACCESS */

			constexpr value_type &front()
			{
				return *this->begin();
			}

			constexpr const value_type &front() const
			{
				return *this->begin();
			}

			constexpr value_type& back()
			{
				if(this->_head == nullptr)
					return this->front();

				if(this->_head->_prev == nullptr)
					return this->front();

				return this->_head->_prev->_data;
			}

			constexpr const value_type& back() const
			{
				if(this->_head == nullptr)
					return this->front();

				if(this->_head->_prev == nullptr)
					return this->front();

				return this->_head->_prev->_data;
			}

			/* MODIFIERS */

			void erase(const_iterator  iter)
			{
				this->remove(iter.node());

				if(this->_size == 0UL)
					iter.clear();
			}

			void erase(iterator iter)
			{
				this->remove(iter.node());

				if(this->_size == 0UL)
					iter.clear();
			}

			constexpr void merge(LinkedList&& list)
			{
				if(this == &list)
					return;

				stl::foreach(list, [&](iterator& iter) {
					auto node = iter.node();
					list.removeAndKeepNode(node);
					node->_next = nullptr;
					node->_prev = nullptr;
					this->add_back(node);
				});
			}

			constexpr void merge(LinkedList& list)
			{
				this->append(stl::move(list));
			}

			void copy(const LinkedList &list)
			{
				this->_alloc = list._alloc;

				for(const value_type &value : list) {
					this->push_back(value);
				}
			}

			void push_back(const value_type &data)
			{
				auto node = this->_alloc.allocate(1);
				new(node) node_type(data);
				this->add_back(node);
			}

			void push_back(value_type &&data)
			{
				node_type *node = new node_type(stl::forward<value_type>(data));
				this->add_back(node);
			}

			void push_front(const value_type &data)
			{
				node_type *node = new node_type(data);
				this->add_front(node);
			}

			void push_front(value_type &&data)
			{
				node_type *node = new node_type(stl::forward<value_type>(data));
				this->add_front(node);
			}

			void clear()
			{
				if(this->_size == 0UL)
					return;

				if(this->_size == 1UL) {
					this->_head->~node_type();
					this->_alloc.deallocate(this->_head, 1);
					this->_size = 0UL;

					return;
				}

				size_t cached = this->_size;

				for(size_t idx = 0; idx < cached; idx++) {
					this->remove(this->_head);
				}
			}

			/* CAPACITY */

			constexpr size_t size() const
			{
				return this->_size;
			}

			constexpr bool empty() const
			{
				return this->size() == 0UL;
			}

			/* UTILITY */

			constexpr friend void swap(LinkedList& l1, LinkedList& l2)
			{
				using stl::swap;

				swap(l1._head, l2._head);
				swap(l1._size, l2._size);
			}


		private:
			friend struct list::Node<value_type>;
			node_type *_head;
			size_t _size;
			allocator_type _alloc;

			constexpr bool removeAndKeepNode(const node_type *node)
			{
				if(this->_head == nullptr || !(node->_next || node->_prev))
					return false;

				this->_size--;

				if(this->_size == 0UL) {
					this->_head = nullptr;
					return true;
				}

				if(node == this->_head) {
					this->_head = node->_next;
				}

				if(node->_next)
					node->_next->_prev = node->_prev;

				if(node->_prev)
					node->_prev->_next = node->_next;

				return true;
			}

			void remove(node_type *node)
			{
				if(!removeAndKeepNode(node))
					return;

				this->_alloc.destroy(node);
				this->_alloc.deallocate(node, 1);
			}

			constexpr void add(node_type *lnew, node_type *prev, node_type *next)
			{
				next->_prev = lnew;
				lnew->_next = next;
				lnew->_prev = prev;
				prev->_next = lnew;
			}

			constexpr void add_front(node_type *node)
			{
				node->_prev = node->_next = node;
				this->_size++;

				if(this->_head == nullptr) {
					this->_head = node;
					return;
				}

				this->add(node, _head, _head->_next);
				this->_head = node;
			}

			constexpr void add_back(node_type *node)
			{
				assert(node);

				node->_prev = node->_next = node;
				this->_size++;

				if(this->_head == nullptr) {
					this->_head = node;
					return;
				}

				this->add(node, _head->_prev, _head);
			}
		};
	}
}
