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

namespace lwiot
{
	namespace stl
	{
		template<typename T>
		class LinkedList;

		namespace list
		{
			class Iterator;

			template<typename T>
			struct Node {
			public:
				explicit Node(const T &data) : _data(data), next(nullptr), prev(nullptr)
				{
				}

				explicit Node(T &&data) : _data(stl::forward<T>(data)), next(nullptr), prev(nullptr)
				{
				}

				constexpr bool operator==(const Node<T> &other)
				{
					return this->_data == other._data && this->next == other.next && this->prev == other.prev;
				}

				T _data;
				friend class LinkedList<T>;

			private:
				friend class Iterator;

				Node<T> *next;
				Node<T> *prev;
			};
		}

		template<typename T>
		class LinkedList {
		public:
			typedef T value_type;
			typedef list::Node<value_type> node_type;

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
						this->_next = node->next;
				}

				constexpr Iterator &operator=(typename LinkedList<T>::node_type *node)
				{
					this->_current = node;
					this->_start = node;

					if(node)
						this->_next = node->next;

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
					this->_next = this->_next->next;
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
				friend class LinkedList<T>;
				node_type *_current;
				node_type *_start;
				node_type *_next;
			};

			typedef Iterator<false> iterator;
			typedef Iterator<true> const_iterator;

			constexpr explicit LinkedList() : _head(nullptr), _size(0UL)
			{
			}

			LinkedList(const LinkedList<T> &other) : _head(nullptr), _size(0UL)
			{
				this->copy(other);
			}

			constexpr LinkedList(LinkedList<T> &&other) noexcept : _head(nullptr), _size(0)
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

			void remove(const node_type *node)
			{
				if(!removeAndKeepNode(node))
					return;

				delete node;
			}

			void erase(const_iterator&  iter)
			{
				this->remove(iter.node());

				if(this->_size == 0UL)
					iter.clear();
			}

			void erase(iterator& iter)
			{
				this->remove(iter.node());

				if(this->_size == 0UL)
					iter.clear();
			}

			void copy(const LinkedList<T> &list)
			{
				for(const value_type &value : list) {
					this->push_back(value);
				}
			}

			constexpr void append(LinkedList&& list)
			{
				stl::foreach(list, [&](iterator& iter) {
					auto node = iter.node();
					list.removeAndKeepNode(node);
					node->next = nullptr;
					node->prev = nullptr;
					this->add_back(node);
				});
			}

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

				if(this->_head->prev == nullptr)
					return this->front();

				return this->_head->prev->_data;
			}

			constexpr const value_type& back() const
			{
				if(this->_head == nullptr)
					return this->front();

				if(this->_head->prev == nullptr)
					return this->front();

				return this->_head->prev->_data;
			}

			void push_back(const value_type &data)
			{
				node_type *node = new node_type(data);
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
					delete this->_head;
					this->_size = 0UL;

					return;
				}

				size_t cached = this->_size;

				for(size_t idx = 0; idx < cached; idx++) {
					this->remove(this->_head);
				}
			}

			constexpr size_t size() const
			{
				return this->_size;
			}

			constexpr bool empty() const
			{
				return this->size() == 0UL;
			}

			constexpr friend void swap(LinkedList<value_type>& l1, LinkedList<value_type>& l2)
			{
				using stl::swap;

				swap(l1._head, l2._head);
				swap(l1._size, l2._size);
			}


		private:

			friend struct list::Node<T>;
			node_type *_head;
			size_t _size;

			constexpr bool removeAndKeepNode(const node_type *node)
			{
				if(this->_head == nullptr || !(node->next || node->prev))
					return false;

				this->_size--;

				if(this->_size == 0UL) {
					this->_head = nullptr;
					return true;
				}

				if(node == this->_head) {
					this->_head = node->next;
				}

				if(node->next)
					node->next->prev = node->prev;

				if(node->prev)
					node->prev->next = node->next;

				return true;
			}

			constexpr void add(node_type *lnew, node_type *prev, node_type *next)
			{
				next->prev = lnew;
				lnew->next = next;
				lnew->prev = prev;
				prev->next = lnew;
			}

			constexpr void add_front(node_type *node)
			{
				node->prev = node->next = node;
				this->_size++;

				if(this->_head == nullptr) {
					this->_head = node;
					return;
				}

				this->add(node, _head, _head->next);
				this->_head = node;
			}

			constexpr void add_back(node_type *node)
			{
				assert(node);

				node->prev = node->next = node;
				this->_size++;

				if(this->_head == nullptr) {
					this->_head = node;
					return;
				}

				this->add(node, _head->prev, _head);
			}
		};
	}
}
