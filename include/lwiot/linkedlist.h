/*
 * Linked list header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/container_of.h>
#include <lwiot/traits/typechoice.h>

namespace lwiot
{
	template <typename T> class LinkedList;

	namespace list
	{
		class Iterator;

		template <typename T>
		struct Node {
		public:
			explicit Node(const T& data) : _data(data)
			{ }

			explicit Node(T&& data) : _data(stl::forward<T>(data))
			{ }

			CONSTEXPR bool operator==(const Node<T>& other)
			{
				return this->_data == other._data && this->next == other.next && this->prev == other.prev;
			}

		private:
			friend class LinkedList<T>;
			friend class Iterator;

			T _data;
			Node<T>* next;
			Node<T>* prev;
		};
	}

	template <typename T>
	class LinkedList {
	public:
		typedef T value_type;
		typedef list::Node<T> node_type;

		template <bool is_const>
		class Iterator {
		public:
			typedef typename traits::TypeChoice<is_const, const value_type&, value_type&>::type reference;
			typedef typename traits::TypeChoice<is_const, const value_type*, value_type*>::type pointer;

			CONSTEXPR explicit Iterator() : Iterator(nullptr)
			{ }

			CONSTEXPR explicit Iterator(node_type* node) : _current(node), _start(node), _next(nullptr)
			{
				if(node)
					this->_next = node->next;
			}

			CONSTEXPR Iterator& operator=(typename LinkedList<T>::node_type* node)
			{
				this->_current = node;
				this->_start = node;

				if(node)
					this->_next = node->next;

				return *this;
			}

			explicit CONSTEXPR operator bool() const
			{
				return this->_current != nullptr;
			}

			CONSTEXPR Iterator& operator++()
			{
				if(this->_current == nullptr)
					return *this;

				if(this->_next == this->_start) {
					this->_current = nullptr;
					return *this;
				}

				this->_current = this->_next;
				this->_next = this->_next->next;
				return *this;
			}

			CONSTEXPR Iterator operator++(int)
			{
				Iterator iter = *this;

				++*this;
				return iter;
			}

			CONSTEXPR bool operator ==(const Iterator& iter) const
			{
				return this->_current == iter._current ;
			}

			CONSTEXPR bool operator !=(const Iterator& iter) const
			{
				return !(*this == iter);
			}

			CONSTEXPR reference operator*() const
			{
				assert(this->_current);
				return this->_current->_data;
			}

			CONSTEXPR pointer operator->() const
			{
				assert(this->_current);
				return &this->_current->_data;
			}

			CONSTEXPR node_type* node() const
			{
				return this->_current;
			}

		private:
			node_type* _current;
			node_type* _start;
			node_type* _next;
		};

		typedef Iterator<false> iterator;
		typedef Iterator<true> const_iterator;

		CONSTEXPR explicit LinkedList() : _head(nullptr), _size(0UL)
		{ }

		explicit LinkedList(const LinkedList<T>& other) : _head(nullptr), _size(0UL)
		{
			this->copy(other);
		}

		CONSTEXPR explicit LinkedList(LinkedList<T>&& other) noexcept
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

		LinkedList& operator=(const LinkedList<T>& rhs)
		{
			this->copy(rhs);
			return *this;
		}

		LinkedList& operator=(LinkedList<T>&& rhs)
		{
			if(this->_size > 0UL)
				this->clear();

			this->_size = rhs._size;
			this->_head = rhs._head;

			rhs._head = nullptr;
			rhs._size = 0UL;
			return *this;
		}

		CONSTEXPR void remove(const node_type* node)
		{
			if(this->_head == nullptr || !(node->next || node->prev))
				return;

			this->_size--;

			if(this->_size == 0UL) {
				this->_head = nullptr;
				delete node;
				return;
			}

			if(node == this->_head) {
				this->_head = node->next;
			}

			if(node->next)
				node->next->prev = node->prev;

			if(node->prev)
				node->prev->next = node->next;

			delete node;
		}

		CONSTEXPR void remove(iterator& iter)
		{
			this->remove(iter.node());
		}

		CONSTEXPR void copy(const LinkedList<T>& list)
		{
			for(const value_type& value : list) {
				this->push_back(value);
			}
		}

		CONSTEXPR iterator begin()
		{
			return iterator(this->_head);
		}

		CONSTEXPR const_iterator begin() const
		{
			return const_iterator(this->_head);
		}

		CONSTEXPR iterator end()
		{
			return iterator(nullptr);
		}

		CONSTEXPR const_iterator end() const
		{
			return const_iterator(nullptr);
		}

		CONSTEXPR value_type& front()
		{
			return *this->begin();
		}

		CONSTEXPR const value_type& front() const
		{
			return *this->begin();
		}

		CONSTEXPR void push_back(const value_type & data)
		{
			node_type* node = new node_type(data);
			this->add_back(node);
		}

		CONSTEXPR void push_back(value_type&& data)
		{
			node_type* node = new node_type(stl::forward<value_type>(data));
			this->add_back(node);
		}

		CONSTEXPR void push_front(const value_type& data)
		{
			node_type* node = new node_type(data);
			this->add_front(node);
		}

		CONSTEXPR void push_front(value_type&& data)
		{
			node_type* node = new node_type(stl::forward<value_type>(data));
			this->add_front(node);
		}

		CONSTEXPR void clear()
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

	private:
		friend class list::Node<T>;
		node_type* _head;
		size_t _size;

		constexpr void add(node_type* lnew, node_type* prev, node_type* next)
		{
			next->prev = lnew;
			lnew->next = next;
			lnew->prev = prev;
			prev->next = lnew;
		}

		CONSTEXPR void add_front(node_type* node)
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

		CONSTEXPR void add_back(node_type* node)
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

