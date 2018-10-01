/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/types.h>

#ifdef __cplusplus
template <typename T>
void *operator new(size_t s, T *v)
{
	return v;
}

namespace lwiot
{
	template<typename T> struct Simple_alloc {

		Simple_alloc() = default;

		T* allocate(int n)
		{
			auto data = lwiot_mem_alloc(n * sizeof(T));
			return reinterpret_cast<T*>(data);
		}

		void deallocate(T* p, int n)
		{
			if(!p || n == 0)
				return;

			lwiot_mem_free(p);
			//delete[] reinterpret_cast<char*>(p);
		}

		void construct(T* p, const T& t) { new(p) T(t); }
		void destroy(T* p) { p->~T(); }
	};

	template<class T, class A = Simple_alloc<T> >
	class Vector {
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		Vector() : sz(0), elem(nullptr), space(0)
		{
		}

		explicit Vector(const size_t& s) : sz(0), elem(nullptr), space(0)
		{
			this->reserve(s);
		}

		Vector<T, A>& operator=(const Vector& a);

		Vector(const Vector<T, A>& other) : sz(0), elem(nullptr), space(0)
		{
			this->reserve(other.capacity());

			for(auto& idx : other) {
				this->pushback(idx);
			}
		}

		virtual ~Vector()
		{
			for(int i = 0; i<sz; ++i) alloc.destroy(&elem[i]);
		}

		iterator begin()
		{
			return &this->elem[0];
		}

		const_iterator begin() const
		{
			return &this->elem[0];
		}

		iterator end()
		{
			return &this->elem[this->sz];
		}

		const_iterator end() const
		{
			return &this->elem[this->sz];
		}

		const T& get(int n) const
		{
			return this->elem[n];
		}

		T& operator[](int n) { return elem[n]; }
		const T& operator[](int n) const { return elem[n]; }

		size_t size() const { return sz; }
		size_t capacity() const { return space; }
		size_t length() const { return this->size(); }

		void reserve(int newalloc);
		void pushback(const T& val);

		void add(const T& val)
		{
			this->pushback(val);
		}

		template <typename Func>
		void foreach(Func functor)
		{
			for(size_t idx = 0U; idx < this->sz; idx++) {
				functor(this->elem[idx]);
			}
		}

	private:
		A alloc;

		int sz;
		T* elem;
		int space;
	};

	template<class T, class A>
	Vector<T, A>& Vector<T, A>::operator=(const Vector& a)
	{
		if(this == &a) return *this;

		if(a.size() <= (size_t)space) {
			for(int i = 0; (size_t)i<a.size(); ++i) elem[i] = a[i];
			sz = a.size();
			return *this;
		}

		T* p = alloc.allocate(a.size());

		for(size_t i = 0; i < a.size(); ++i) {
			alloc.construct(&p[i], a[i]);
		}

		for(size_t i = 0; i < this->size(); ++i)
			alloc.destroy(&elem[i]);

		space = sz = a.size();
		elem = p;
		return *this;
	}

	template<class T, class A> void Vector<T, A>::reserve(int newalloc)
	{
		if(newalloc <= space)
			return;

		T* p = alloc.allocate(newalloc);

		for(int i = 0; i<sz; ++i)
			alloc.construct(&p[i], elem[i]);
		for(int i = 0; i<sz; ++i)
			alloc.destroy(&elem[i]);

		if(this->elem)
			alloc.deallocate(elem, space);
		this->elem = p;
		this->space = newalloc;
	}

	template<class T, class A>
	void Vector<T, A>::pushback(const T& val)
	{
		if(space == 0) reserve(4);
		else if(sz == space) reserve(2 * space);
		alloc.construct(&elem[sz], val);
		++sz;
	}
}

#endif
