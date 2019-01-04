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

namespace lwiot
{
	template <typename T> class LinkedList;

	namespace list
	{
		template <typename T> class Iterator;

		template <typename T>
		struct Node {
		public:
			explicit Node(const T& data) : _data(data)
			{ }

			explicit Node(T&& data) : _data(stl::forward<T>(data))
			{ }

			constexpr bool operator==(const Node<T>& other)
			{
				return this->_data == other._data && this->next == other.next && this->prev == other.prev;
			}

		private:
			friend class LinkedList<T>;
			friend class Iterator<T>;

			T _data;
			Node<T>* next;
			Node<T>* prev;
		};
	}

	template <typename T>
	class LinkedList {
	public:
		class Iterator;
		typedef T value_type;
		typedef list::Node<T> node_type;
		typedef Iterator iterator;

		constexpr explicit LinkedList() : head(nullptr), _size(0UL)
		{ }

		explicit LinkedList(const LinkedList<T>& other) : head(nullptr), _size(0UL)
		{
			this->copy(other);
		}

		constexpr explicit LinkedList(LinkedList<T>&& other) noexcept
		{
			this->_size = other._size;
			this->head = other.head;

			other.head = nullptr;
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
			this->head = rhs.head;

			rhs.head = nullptr;
			rhs._size = 0UL;
			return *this;
		}

		constexpr void remove(const node_type* node)
		{
			if(this->head == nullptr || !(node->next || node->prev))
				return;

			this->_size--;

			if(this->_size == 0UL) {
				this->head = nullptr;
				delete node;
				return;
			}

			if(node == this->head) {
				this->head = node->next;
			}

			if(node->next)
				node->next->prev = node->prev;

			if(node->prev)
				node->prev->next = node->next;

			delete node;
		}

		constexpr void copy(const LinkedList<T>& list)
		{
			for(const value_type& value : list) {
				this->push_back(value);
			}
		}

		constexpr void removeAt(size_t index)
		{
			if(index >= this->_size)
				return;

			size_t idx = 0UL;

			for(iterator iter = iterator(this->head); iter != this->end(); ++iter, idx++) {
				if(idx == index) {
					this->remove(iter.node());
					break;
				}
			}
		}

		constexpr void remove(const value_type& data)
		{
			for(iterator iter = iterator(this->head); iter != this->end(); ++iter) {
				if(*iter == data) {
					this->remove(iter.node());
					break;
				}
			}
		}

		constexpr iterator begin() const
		{
			return iterator(this->head);
		}

		constexpr iterator end() const
		{
			return iterator(nullptr);
		}

		constexpr void push_back(const value_type & data)
		{
			node_type* node = new node_type(data);
			this->add_back(node);
		}

		constexpr void push_back(value_type&& data)
		{
			node_type* node = new node_type(stl::forward<value_type>(data));
			this->add_back(node);
		}

		constexpr void push_front(const value_type& data)
		{
			node_type* node = new node_type(data);
			this->add_front(node);
		}

		constexpr void push_front(value_type&& data)
		{
			node_type* node = new node_type(stl::forward<value_type>(data));
			this->add_front(node);
		}

		constexpr void clear()
		{
			if(this->_size == 0UL)
				return;

			if(this->_size == 1UL) {
				delete this->head;
				this->_size = 0UL;

				return;
			}

			size_t cached = this->_size;
			for(size_t idx = 0; idx < cached; idx++) {
				this->removeAt(0);
			}
		}

		class Iterator {
		public:
			constexpr explicit Iterator() : _current(nullptr), _start(nullptr)
			{ }

			constexpr explicit Iterator(node_type* node) : _current(node), _start(node)
			{ }

			constexpr Iterator& operator=(typename LinkedList<T>::node_type* node)
			{
				this->_current = node;
				this->_start = node;

				return *this;
			}

			constexpr Iterator& operator++()
			{
				if(this->_current) {
					if(this->_current->next == this->_start) {
						this->_current = nullptr;
					} else {
						this->_current = this->_current->next;
					}
				}

				return *this;
			}

			constexpr Iterator operator++(int)
			{
				Iterator iter = *this;

				++*this;
				return iter;
			}

			constexpr bool operator ==(const Iterator& iter) const
			{
				return this->_current == iter._current ;
			}

			constexpr bool operator !=(const Iterator& iter) const
			{
				return !(*this == iter);
			}

			constexpr T& operator*() const
			{
				assert(this->_current);
				return this->_current->_data;
			}

			constexpr T* operator->() const
			{
				assert(this->_current);
				return &this->_current->_data;
			}

			constexpr node_type* node() const
			{
				return this->_current;
			}

		private:
			node_type* _current;
			node_type* _start;
		};

	private:
		friend class list::Node<T>;
		node_type* head;
		size_t _size;

		constexpr void add(node_type* lnew, node_type* prev, node_type* next)
		{
			next->prev = lnew;
			lnew->next = next;
			lnew->prev = prev;
			prev->next = lnew;
		}

		constexpr void add_front(node_type* node)
		{
			node->prev = node->next = node;
			this->_size++;

			if(this->head == nullptr) {
				this->head = node;
				return;
			}

			this->add(node, head, head->next);
		}

		constexpr void add_back(node_type* node)
		{
			assert(node);

			node->prev = node->next = node;
			this->_size++;

			if(this->head == nullptr) {
				this->head = node;
				return;
			}

			this->add(node, head->prev, head);
		}
	};
}

