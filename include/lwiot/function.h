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
#include <lwiot/functor.h>

#include <lwiot/traits/isfunction.h>
#include <lwiot/traits/enableif.h>

namespace lwiot
{
	template<class ReturnType, class...Xs>
	struct SFConcept {
		virtual ReturnType operator()(Xs...) const = 0;
		virtual ReturnType operator()(Xs...) = 0;

		virtual ~SFConcept() = default;
	};

	template<class F, typename ReturnType, typename...Xs>
	struct SFModel final : SFConcept<ReturnType, Xs...> {
        typename traits::RemoveCv<F>::type f;

		explicit SFModel(F const &f) : f(f)
		{
		}

		virtual ReturnType operator()(Xs...xs) const
		{
			return f(xs...);
		}

		virtual ReturnType operator()(Xs...xs)
		{
			return f(xs...);
		}

		virtual ~SFModel() = default;
	};


	template<typename Func>
	class Function ;

	template<typename ReturnType, typename ...Xs>
	class Function<ReturnType(Xs...)> {
		typedef size_t size_type;
		static constexpr size_type SIZE = sizeof(SFModel<ReturnType(*)(Xs...), ReturnType, Xs...>);

	public:
		Function() : _buffer(SIZE), allocated(false)
		{
		}

		template <typename Func>
		explicit Function(const Func &f) : _buffer(SIZE),  allocated(true)
		{
			new(_buffer.data()) SFModel<Func, ReturnType, Xs...>(f);
		}

		Function(const Function &sf) : _buffer(SIZE), allocated(sf.allocated)
		{
			this->_buffer = sf._buffer;
		}

		Function(Function &&sf) noexcept : _buffer(stl::move(sf._buffer)), allocated(sf.allocated)
		{
			sf.allocated = false;
		}

		template <typename Func>
		Function(Func&& func) noexcept : _buffer(SIZE), allocated(true)
		{
			static_assert(sizeof(SFModel<Func, ReturnType, Xs...>) <= SIZE, "Expression too big!");
			new(_buffer.data()) SFModel<Func, ReturnType, Xs...>(func);
		}

		Function &operator=(Function const &sf)
		{
			clean();
			allocated = sf.allocated;
			this->_buffer = sf._buffer;
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
			if(allocated && this->_buffer.data() != nullptr) {
				auto c = this->as_concept();
				this->allocated = false;
				c->~concept();
			}
		}

		template <typename Func>
		Function& operator=(Func&& func) noexcept
		{
			this->clean();
			this->allocated = true;
			this->_buffer.reset();
			new(this->_buffer.data()) SFModel<Func, ReturnType, Xs...>(stl::forward<Func>(func));
			return *this;
		}

		Function& operator=(Function&& func) noexcept
		{
			this->clean();
			this->allocated = func.allocated;
			this->_buffer = stl::move(func._buffer);

			func.allocated = false;
			return *this;
		}

		template <typename Func>
		Function& operator=(Func& f)
		{
			this->allocated = sizeof(SFModel<Func, ReturnType, Xs...>) != 0;
			new(_buffer.data()) SFModel<Func, ReturnType, Xs...>(f);
			return *this;
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
				((concept *) _buffer)->copy(data);
			}
		}

	private:
		ByteBuffer _buffer;
		bool allocated;
		using concept = SFConcept<ReturnType, Xs...>;

		constexpr concept* as_concept() const
		{
			return (concept*) this->_buffer.data();
		}
	};
}
