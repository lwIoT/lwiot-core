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
#include <lwiot/functor.h>
#include <lwiot/uniquepointer.h>

#include <lwiot/stl/forward.h>
#include <lwiot/stl/memfn.h>

#include <lwiot/traits/enableif.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/decay.h>

namespace lwiot
{
	template<class ReturnType, class...Xs>
	struct SFConcept {
		virtual ReturnType invoke(Xs&&...) = 0;
		virtual SFConcept* clone() const = 0;

		virtual ~SFConcept() = default;
	};

	template<typename Signature, typename Functor>
	struct SFModel;

	/* Specialization for member functions */
	template <typename Class, typename Member, typename ReturnType, typename... Args>
	struct SFModel<ReturnType(Args...), Member Class::*> : public SFConcept<ReturnType, Args...> {
	private:
		using CallableType = Member (Class::*);
		CallableType _callable;

	public:
		explicit SFModel(CallableType callable) : _callable(stl::move(callable))
		{
		}

		SFModel(const SFModel &) = default;
		SFModel(SFModel &&) noexcept = default;

		SFModel& operator=(const SFModel &) = default;
		SFModel& operator=(SFModel &&) noexcept = default;

		ReturnType invoke(Args &&... args)
		{
			return this->call(stl::forward<Args>(args)...);
		}

		template<typename ClassType, typename... Params>
		ReturnType call(ClassType obj, Params &&... args)
		{
			return stl::mem_fn(this->_callable)(obj, stl::forward<Params>(args)...);
		}

		SFConcept<ReturnType, Args...>* clone() const override
		{
			return new SFModel(*this);
		}
	};

	/* Specialization for function pointers */
	template<typename ReturnType, typename... Args>
	struct SFModel<ReturnType(Args...), ReturnType(*)(Args...)> : public SFConcept<ReturnType, Args...> {
	private:
		using CallableType = ReturnType(*)(Args...);
		CallableType _callable;

	public:
		explicit SFModel(CallableType callable) : _callable(stl::move(callable))
		{
		}

		SFModel(const SFModel &) = default;
		SFModel(SFModel &&) noexcept = default;

		SFModel& operator=(const SFModel &) = default;
		SFModel& operator=(SFModel &&) noexcept = default;

		ReturnType invoke(Args&&... args)
		{
			return this->_callable(stl::forward<Args>(args)...);
		}

		SFConcept<ReturnType, Args...>* clone() const override
		{
			return new SFModel(*this);
		}
	};

	/* Specialization for Functors */
	template<typename F, typename ReturnType, typename...Xs>
	struct SFModel<ReturnType(Xs...), F> : public SFConcept<ReturnType, Xs...> {
		SFModel(F&& f) : f(stl::forward<F>(f))
		{
		}

		SFModel(const SFModel &) = default;
		SFModel(SFModel &&) noexcept = default;

		SFModel& operator=(const SFModel &) = default;
		SFModel& operator=(SFModel &&) noexcept = default;

		SFConcept<ReturnType, Xs...>* clone() const override
		{
			return new SFModel(*this);
		}

		ReturnType invoke(Xs&&... xs)
		{
			return f(stl::forward<Xs>(xs)...);
		}

		virtual ~SFModel() = default;

	private:
		F f;
	};


	template<typename Func>
	class Function ;

	template<typename ReturnType, typename ...Xs>
	class Function<ReturnType(Xs...)> {
		using Signature = ReturnType(Xs...);
		using Concept = SFConcept<ReturnType, Xs...>;

	public:
		Function() : _concept(nullptr)
		{ }

		Function(const Function& other) : _concept()
		{
			if(other.valid())
				this->_concept.reset(other._concept->clone());
		}

		Function(Function&& other) noexcept : _concept(stl::move(other._concept))
		{
		}

		Function& operator=(const Function& other)
		{
			if(other.valid())
				this->_concept.reset(other._concept->clone());

			return *this;
		}

		Function& operator=(Function&& other) noexcept
		{
			this->_concept.reset(other._concept.release());
			return *this;
		}

		template <typename F, typename traits::EnableIf<
					!traits::IsSame<typename traits::Decay<F>::type, Function<ReturnType(Xs...)>>::value, bool>::type = true>
		Function& operator=(F&& f)
		{
			this->_concept.reset(new SFModel<Signature, F>(stl::forward<F>(f)));
			return *this;
		}

		template <typename F>
		Function(F f) : _concept(MakeUnique<SFModel<Signature, F>>(stl::move(f)))
		{
		}

		ReturnType operator()(Xs... args) const
		{
			auto c = this->as_concept();
			return c->invoke(stl::forward<Xs>(args)...);
		}

		bool valid() const
		{
			return static_cast<bool>(this->_concept);
		}

		explicit operator bool() const
		{
			return this->valid();
		}

	private:
		UniquePointer<Concept> _concept;

		constexpr Concept* as_concept() const
		{
			return this->_concept.get();
		}
	};
}
