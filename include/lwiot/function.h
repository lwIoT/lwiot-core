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

#include <lwiot/traits/isfunction.h>
#include <lwiot/traits/enableif.h>

namespace lwiot
{
	template<class ReturnType, class...Xs>
	struct SFConcept {
		virtual ReturnType operator()(Xs...) const = 0;
		virtual ReturnType operator()(Xs...) = 0;
		virtual void copy(void *) const = 0;

		virtual ~SFConcept()
		{
		};
	};

	template<class F, typename ReturnType, typename...Xs>
	struct SFModel final : SFConcept<ReturnType, Xs...> {
        F f;

		SFModel(F const &f) : f(f)
		{
		}

		virtual void copy(void *memory) const
		{
			new(memory) SFModel<F, ReturnType, Xs...>(f);
		}

		virtual ReturnType operator()(Xs...xs) const
		{
			return f(xs...);
		}

		virtual ReturnType operator()(Xs...xs)
		{
			return f(xs...);
		}

		virtual ~SFModel()
		{
		}
	};


	template<typename Func, size_t size = 128>
	class Function ;

	template<typename ReturnType, typename ...Xs, size_t size>
	class Function<ReturnType(Xs...), size> {
	public:
		Function() : allocated(false)
		{
		}

		template <typename Func>
		Function(Func const &f) : allocated(sizeof(SFModel<Func, ReturnType, Xs...>) != 0)
		{
			static_assert(sizeof(SFModel<Func, ReturnType, Xs...>) <= size, "Expression too big!");
			new(memory) SFModel<Func, ReturnType, Xs...>(f);
		}

		template<unsigned s, traits::EnableIf_t<(s <= size), bool> = false>
		Function(Function<ReturnType(Xs...), s> const &sf) : allocated(sf.allocated)
		{
			sf.copy(memory);
		}


		template<unsigned s, traits::EnableIf_t<(s <= size), bool> = false>
		Function &operator=(Function<ReturnType(Xs...), s> const &sf)
		{
			clean();
			allocated = sf.allocated;
			sf.copy(memory);
			return *this;
		}

		void clean()
		{
			if(allocated) {
				auto c = this->as_concept();
				c->~concept();
				allocated = false;
			}
		}

		virtual ~Function()
		{
			if(allocated) {
				auto c = this->as_concept();
				c->~concept();
			}
		}

		template<class...Ys>
		ReturnType operator()(Ys &&...ys)
		{
			auto c = this->as_concept();
			return (*c)(stl::forward<Ys>(ys)...);
		}

		template<class...Ys>
		ReturnType operator()(Ys &&...ys) const
		{
			auto c = this->as_concept();
			return (*c)(stl::forward<Ys>(ys)...);
		}

		bool valid() const
		{
			return this->allocated;
		}

		explicit operator bool() const
		{
			return this->valid();
		}

		void copy(void *data) const
		{
			if(allocated) {
				((concept *) memory)->copy(data);
			}
		}

	private:
		char memory[size];
		bool allocated;
		using concept = SFConcept<ReturnType, Xs...>;

		constexpr concept* as_concept() const
		{
			return (concept*) &memory[0];
		}
	};
}
