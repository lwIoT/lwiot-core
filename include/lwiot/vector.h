/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	template<class T> class Vector {
	public:
		explicit Vector() : Vector(8) { }

		Vector(size_t size) : _size(size)
		{
			this->_length = 0;
			this->_data = new T[size];
		}

		virtual ~Vector()
		{
			delete[] this->_data;
		}

		void add(const T& obj)
		{
			if(this->_length == this->_size) {
				this->_data = (T*) grow(this->_size * 2U);
				this->_size = this->_size * 2U;
			}

			this->_data[this->_length++] = obj;
		}

		const T* begin() const
		{
			return &this->_data[0];
		}

		const T* end() const
		{
			return &this->_data[this->_length];
		}

		T get(size_t idx)
		{
			return this->_data[idx];
		}

		T& operator[] (size_t idx)
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

		friend void *grow(size_t newsize);
	};
}
