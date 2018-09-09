/*
 * Wrapper class for lambda expressions and functors.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 * @authos joas <StackOverflow>
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/sharedpointer.h>

namespace lwiot
{
	template <typename T> struct IsVoid {
		static const bool value = false;
	};

	template <> struct IsVoid<void> {
		static const bool value = true;
	};

	template <typename _ReturnType, typename... Args> struct Callable {
		typedef _ReturnType ReturnType;
		typedef _ReturnType (*SignatureType)(Args...);
		virtual ~Callable() = default;

		virtual _ReturnType operator ()(Args...args) = 0;
	};

	template <typename _ClosureType, typename _ReturnType, typename ...Args>
	struct Closure : public Callable<_ReturnType, Args...> {
		typedef _ClosureType ClosureType;
		typedef Closure<_ClosureType, _ReturnType, Args...> SelfType;

		_ClosureType handler;

		Closure(const _ClosureType& handler) : handler(handler)
		{ }

		Closure(const Callable<_ReturnType, Args...> *callable)
		{
			const Closure<ClosureType , _ReturnType, Args...> *closure = (const SelfType *)callable;
			this->handler = closure->handler;
		}

		virtual ~Closure() = default;

		virtual _ReturnType operator ()(Args...args) override
		{
			if(IsVoid<_ReturnType>::value) {
				this->handler(args...);
			} else {
				return this->handler(args...);
			}
		}
	};

	template <typename _FunctionType> class Function : public Function<decltype(&_FunctionType::operator())> {
	};

	/*
	 * Wrapper for lambda's, functors and functions.
	 */
	template <typename _ReturnType, typename... Args> class Function<_ReturnType(*)(Args...)> {
	public:
		typedef Function<_ReturnType(*)(Args...)> SelfType;
		typedef _ReturnType(*SignatureType)(Args...);

		template <typename _ClosureType> Function(const _ClosureType& func) :
			_callable(new Closure<decltype(func), _ReturnType, Args...>(func))
		{
		}

		Function(_ReturnType(*func)(Args...)) :
			_callable(new Closure<SignatureType, _ReturnType, Args...>(func))
		{
		}

		Function() : _callable(nullptr)
		{ }

		virtual ~Function()
		{
		}

//		Function(const SelfType& func)
//		{
//			if(func._callable == nullptr) {
//				this->_callable = nullptr;
//				return;
//			}
//
//			this->_callable = new Closure<SignatureType , _ReturnType, Args...>(func._callable);
//
//			this->_callable = func._callable;
//		}

//		Function& operator =(const SelfType& func)
//		{
//			if(func._callable == nullptr) {
//				this->_callable = nullptr;
//				return *this;
//			}
//
//			this->_callable = new Closure<SignatureType , _ReturnType, Args...>(func._callable);
//			return *this;
//		}

		Function& operator=(_ReturnType(*func)(Args...))
		{
			if(this->_callable)
				delete this->_callable;

			this->_callable.reset( new Closure<SignatureType, _ReturnType, Args...>(func));
			return *this;
		}

		template <typename _ClosureType>
		Function& operator=(const _ClosureType& func)
		{
			this->_callable.reset( new Closure<decltype(func), _ReturnType, Args...>(func));
			return *this;
		}

		_ReturnType operator() (Args... args)
		{
			if(IsVoid<_ReturnType>::value) {
				(*_callable)(args...);
			} else {
				return (*_callable)(args...);
			}
		}

		operator bool() const
		{
			return this->valid();
		}

		bool operator !() const
		{
			return !this->valid();
		}

		bool valid() const
		{
			return this->_callable.get() != nullptr;
		}

	private:
//		Callable<_ReturnType, Args...> *_callable;
//		std::shared_ptr<Callable<_ReturnType, Args...>> _callable;
		lwiot::SharedPointer<Callable<_ReturnType, Args...>> _callable;
	};

	template <typename _ClassType, typename _ReturnType, typename... Args>
	class Function<_ReturnType(_ClassType::*)(Args...)> {
	public:
		typedef Function<_ReturnType(_ClassType::*)(Args...)> SelfType;
		typedef _ReturnType(_ClassType::*SignatureType)(Args...);

		Function(_ReturnType(_ClassType::*method)(Args...)) : _method(method)
		{ }

		Function() : _method(nullptr)
		{ }

		Function& operator=(_ReturnType(_ClassType::*method)(Args...))
		{
			this->_method = method;
			return *this;
		}

		_ReturnType operator ()(_ClassType *object, Args... args)
		{
			if(IsVoid<_ReturnType>::value) {
				(object->*_method)(args...);
				return;
			}

			return (object->*_method)(args...);
		}

		bool valid() const
		{
			return this->_callable != nullptr;
		}

	private:
		SignatureType _method;
	};

	template <typename C, typename R, typename... Args> class Function<R(C::*)(Args...) const> {
	public:
		typedef Function<R(C::*)(Args...) const> SelfType;
		typedef R(C::*SignatureType)(Args...) const;

		Function(R(C::*method)(Args...) const) : _method(method)
		{ }

		Function() : _method(nullptr)
		{ }

		Function& operator=(R(C::*method)(Args...) const)
		{
			this->_method = method;
			return *this;
		}

		R operator ()(C *object, Args... args) const
		{
			if(IsVoid<R>::value) {
				(object->*_method)(args...);
				return;
			}

			return (object->*_method)(args...);
		}

		bool valid() const
		{
			return this->_callable != nullptr;
		}

	private:
		SignatureType _method;
	};
}
