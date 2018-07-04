/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	extern void *grow(uint8_t *old, size_t oldsize);

	template<class T> class Vector {
	public:
		explicit Vector() : Vector(8) { }

		Vector(size_t size) : _size(size)
		{
			this->_length = 0;
			this->_data = (T*)malloc(sizeof(T) * size);
		}

		virtual ~Vector()
		{
			delete[] this->_data;
		}

		void add(const T& obj)
		{
			if(this->_length == this->_size) {
				this->_data = (T*) grow((uint8_t*)this->_data, this->_size);
				this->_size = this->_size * 2U;
			}

			this->_data[this->_length++] = obj;
		}

		Vector<T>& operator << (const T& obj)
		{
			this->add(obj);
			return *this;
		}

		const T* begin() const
		{
			return &this->_data[0];
		}

		const T* end() const
		{
			return &this->_data[this->_length];
		}

		T *begin()
		{
			return &this->_data[0];
		}

		T *end()
		{
			return &this->_data[this->_length];
		}

		const T& get(size_t idx) const
		{
			return this->_data[idx];
		}

		T& operator[] (size_t idx)
		{
			return this->_data[idx];
		}

		const T& operator[](const size_t& idx) const
		{
			return this->_data[idx];
		}

		const size_t& length() const
		{
			return this->_length;
		}

		const size_t& size() const
		{
			return this->_size;
		}

	private:
		T *_data;
		size_t _length;
		size_t _size;

		friend void *grow(uint8_t *old, size_t oldsize);
	};
}
