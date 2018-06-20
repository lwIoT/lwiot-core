/*
 * C++ pair value object. Types of both objects may
 * differ. Individual members can be accessed through
 * `first()' and `second()'.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once


namespace lwiot
{
	template<typename A, typename B> class Pair {
	public:
		explicit Pair();
		Pair(const A& first, const B& seocnd) : _first(first), _second(seocnd)
		{ }

		Pair(const Pair& other) : _first(other._first), _second(other._second)
		{ }

		Pair<A, B>& operator =(const Pair<A, B>& rhs)
		{
			this->_firt = rhs._firt;
			this->_second = rhs._second;
		}

		const A& first() const
		{
			return this->_firt;
		}

		const B& second() const
		{
			return this->_second;
		}

	private:
		A _firt;
		B _second;
	};
}

