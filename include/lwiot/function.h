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
		Function()
		{
		}

		template<typename F, traits::EnableIf_t<(sizeof(SFModel<F, ReturnType, Xs...>) <= size), bool> = false>
		Function(F const &f) : allocated(sizeof(SFModel<F, ReturnType, Xs...>))
		{
			new(memory) SFModel<F, ReturnType, Xs...>(f);
		}

		template<unsigned s, traits::EnableIf_t<(s <= size), bool> = 0>
		Function(Function<ReturnType(Xs...), s> const &sf)
				: allocated(sf.allocated)
		{
			sf.copy(memory);
		}


		template<unsigned s, traits::EnableIf_t<(s <= size), bool> = 0>
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
				((concept *) memory)->~concept();
				allocated = 0;
			}
		}

		~Function()
		{
			if(allocated) {
				((concept *) memory)->~concept();
			}
		}

		template<class...Ys>
		ReturnType operator()(Ys &&...ys)
		{
			return (*(concept *) memory)(stl::forward<Ys>(ys)...);
		}

		template<class...Ys>
		ReturnType operator()(Ys &&...ys) const
		{
			return (*(concept *) memory)(stl::forward<Ys>(ys)...);
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
		bool allocated = false;
		using concept = SFConcept<ReturnType, Xs...>;
	};
}
