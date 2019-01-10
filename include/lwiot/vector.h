/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#ifdef CXX
#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/defaultallocator.h>
#include <lwiot/stl/move.h>

namespace lwiot
{
	template<class T, class A = DefaultAllocator<T>>
	class Vector {
	public:
		typedef A Allocator;
		typedef T ObjectType;
		typedef T *iterator;
		typedef const T *const_iterator;


		CONSTEXPR explicit Vector() : _index(0), _objects(nullptr), _space(0)
		{
		}

		explicit Vector(const size_t &&s) : _index(0), _objects(nullptr), _space(0)
		{
			this->reserve(s);
		}

		Vector(const Vector<T, A> &other) : _alloc(other._alloc), _index(0), _objects(nullptr), _space(0)
		{
			this->clear();
			this->reserve(other.capacity());

			for(auto &idx : other) {
				this->pushback(idx);
			}
		}

		explicit constexpr Vector(Vector<T, A> &&other) :
				_index(other._index), _objects(other._objects), _space(other._space)
		{
			this->_alloc = other._alloc;

			other._objects = nullptr;
			other._space = 0;
			other._index = 0;
		}

		Vector<T, A> &operator=(const Vector &a);

		Vector<T,A>& operator=(Vector<T,A>&& rhs)
		{
			this->release();

			this->_alloc = stl::move(rhs._alloc);
			this->_space = rhs._space;
			this->_objects = rhs._objects;
			this->_index = rhs._index;

			rhs._objects = nullptr;
			rhs._space = 0;
			rhs._index = 0;

			return *this;
		}

		virtual ~Vector()
		{
			this->release();
		}

		CONSTEXPR void clear()
		{
			if(this->length() == 0UL)
				return;

			for(auto idx = 0UL; idx < this->_index; idx++) {
				this->_alloc.destroy(&_objects[idx]);
			}

			this->_index = 0;
		}

		iterator begin()
		{
			return &this->_objects[0];
		}

		const_iterator begin() const
		{
			return &this->_objects[0];
		}

		iterator end()
		{
			return &this->_objects[this->_index];
		}

		const_iterator end() const
		{
			return &this->_objects[this->_index];
		}

		constexpr const T &get(int n) const
		{
			return this->_objects[n];
		}

		constexpr T &operator[](int n)
		{
			return this->_objects[n];
		}

		constexpr const T &operator[](int n) const
		{
			return this->_objects[n];
		}

		constexpr size_t size() const
		{
			return _index;
		}

		constexpr size_t capacity() const
		{
			return this->_space;
		}

		constexpr size_t length() const
		{
			return this->size();
		}

		void reserve(size_t newalloc);

		CONSTEXPR void pushback(const ObjectType &val);

		CONSTEXPR void add(const ObjectType &val)
		{
			this->pushback(val);
		}

		template<typename Func>
		CONSTEXPR void foreach(Func functor)
		{
			for(size_t idx = 0U; idx < this->_index; idx++) {
				functor(this->_objects[idx]);
			}
		}

	private:
		Allocator _alloc;

		size_t _index;
		ObjectType *_objects;
		size_t _space;

		/* Methods */
		inline void release()
		{
			for(size_t i = 0; i < _index; ++i) {
				_alloc.destroy(&_objects[i]);
			}

			if(this->_objects != nullptr)
				this->_alloc.deallocate(_objects, this->_space);
		}
	};

	template<class T, class A>
	Vector<T, A> &Vector<T, A>::operator=(const Vector &a)
	{
		if(this == &a)
			return *this;

		if(a.size() <=  this->_space) {
			for(size_t i = 0; i < a.size(); ++i) {
				this->_objects[i] = a[i];
			}

			this->_index = a.size();
			return *this;
		}

		T *p = a._alloc.allocate(a.size());

		for(size_t i = 0; i < a.size(); ++i) {
			a._alloc.construct(&p[i], a[i]);
		}

		for(size_t i = 0; i < this->size(); ++i) {
			a._alloc.destroy(&_objects[i]);
		}

		if(this->_objects != nullptr)
			this->_alloc.deallocate(this->_objects, this->_space);

		this->_space = this->_index = a.size();
		this->_objects = p;
		this->_alloc = a._alloc;
		return *this;
	}

	template<class T, class A>
	void Vector<T, A>::reserve(size_t newalloc)
	{
		if(newalloc <= this->_space)
			return;

		T *p = _alloc.allocate(newalloc);

		for(size_t i = 0; i < this->_index; i++) {
			_alloc.construct(&p[i], _objects[i]);
		}
		for(size_t i = 0; i < this->_index; i++) {
			_alloc.destroy(&_objects[i]);
		}

		if(this->_objects)
			_alloc.deallocate(_objects, this->_space);

		this->_objects = p;
		this->_space = newalloc;
	}

	template<class T, class A>
	CONSTEXPR void Vector<T, A>::pushback(const T &val)
	{
		if(this->_space == 0UL)
			reserve(4);
		else if(this->_index == this->_space)
			reserve(2 * this->_space);

		_alloc.construct(&_objects[this->_index], val);
		this->_index++;
	}
}

#endif
