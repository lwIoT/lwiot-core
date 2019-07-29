/*
 * DispatchQueueBase definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/traits/enableif.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/iscallable.h>
#include <lwiot/traits/isconvirtible.h>

#include <lwiot/function.h>
#include <lwiot/types.h>
#include <lwiot/error.h>

#include <lwiot/stl/tuple.h>
#include <lwiot/stl/array.h>

#include <lwiot/kernel/uniquelock.h>

namespace lwiot
{
	namespace detail
	{
		template <typename T>
		struct IsBool {
			static constexpr bool value = false;
		};

		template <>
		struct IsBool<bool> {
			static constexpr bool value = true;
		};

		template <typename T>
		struct IsVoid {
			static constexpr bool value = false;
		};

		template <>
		struct IsVoid<void> {
			static constexpr bool value = true;
		};

		template <size_t... Indices>
		struct IndexSequence {
			using type = IndexSequence<Indices...>;

			constexpr size_t size() const noexcept
			{
				return sizeof...(Indices);
			}
		};

		template <size_t idx, size_t ...Indices>
		struct MakeIndexSequence : MakeIndexSequence<idx - 1, idx - 1, Indices...> { };

		template <size_t ... Indices>
		struct MakeIndexSequence<0, Indices...> {
			using type = IndexSequence<Indices...>;
		};

		template <typename ReturnType>
		struct Dispatcher {
			template <typename QueueType, typename Iterator, typename Worker>
			static constexpr ReturnType dispatch(QueueType& queue, Iterator& iter, Worker&& work)
			{
				auto result = stl::move(work());

				if(likely(result)) {
					queue.erase(iter);
					work.clear();
				} else {
					queue.replace(iter, stl::move(work));
				}

				return result;
			}
		};

		template <>
		struct Dispatcher<void> {
			template <typename QueueType, typename Iterator, typename Worker>
			static constexpr void dispatch(QueueType& queue, Iterator& iter, Worker&& work)
			{
				work();
				queue.erase(iter);
				work.clear();
			}
		};

		template <typename Proto, typename... Args>
		struct DispatchQueueBase_helper {
			static constexpr bool IsCallableWith = traits::IsCallable<Proto, Args...>::value;
			static_assert(IsCallableWith, "Proto type not callable!");

			typedef lwiot::Function<Proto> Handler;
		};

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

		template <typename Proto, template<typename> typename P>
		class DispatchQueueBase;

		template <template <typename> typename P, typename R, typename ... Args>
		class DispatchQueueBase<R(Args...), P> : public DispatchQueueBase_helper<R(Args...), Args...> {
			template <typename T>
			static constexpr bool is_convirtible_to_bool_or_void()
			{
				constexpr auto is_void = IsVoid<T>::value;
				constexpr auto is_bool = IsBool<T>::value;
				constexpr auto is_convirtible = traits::IsConvirtable<T, bool>::value;

				return is_bool || is_convirtible || is_void;
			}

		public:
			struct Work;

			typedef R ReturnType;
			typedef P<Work> PolicyType;
			typedef typename PolicyType::Threading Threading;

			typedef DispatchQueueBase_helper<ReturnType(Args...), Args...> Base;
			typedef DispatchQueueBase<ReturnType(Args...), P> ThisClass;
			typedef UniqueLock<typename Threading::Lock> LockGuard;
			typedef typename PolicyType::Queue Queue;

			static_assert(is_convirtible_to_bool_or_void<ReturnType>(), "Unable to convert return value to bool!");

			using ArgumentWrapper = stl::Tuple< typename traits::RemoveCv<typename traits::RemoveCv<Args>::type>::type...>;

			struct Work {
			public:
				constexpr Work() : _buffer(), _is_allocated(false)
				{ }

				Work(typename Base::Handler handler) : _handler(handler), _buffer(), _is_allocated(false)
				{
				}

				Work(Work&& rhs) noexcept
				{
					this->_is_allocated = rhs._is_allocated;
					this->_buffer = stl::move(rhs._buffer);
					this->_handler = stl::move(rhs._handler);

					rhs._is_allocated = false;
				}

				Work(const Work& other)
				{
					this->_is_allocated = other._is_allocated;
					this->_buffer = other._buffer;
					this->_handler = other._handler;
				}

				template <typename T>
				Work(T&& handler) : _handler(handler), _buffer(), _is_allocated(false)
				{ }

				~Work()
				{
					if(this->_is_allocated)
						this->clear();
				}

				Work& operator=(Work&& rhs)
				{
					if(this->_is_allocated)
						this->clear();

					this->_is_allocated = rhs._is_allocated;
					this->_buffer = stl::move(rhs._buffer);
					this->_handler = stl::move(rhs._handler);

					rhs._is_allocated = false;

					return *this;
				}

				Work& operator=(const Work& rhs)
				{
					if(this->_is_allocated)
						this->clear();

					this->_is_allocated = rhs._is_allocated;
					this->_buffer = rhs._buffer;
					this->_handler = rhs._handler;

					return *this;
				}

				void set(ArgumentWrapper&& args)
				{
					new(this->_buffer.data()) ArgumentWrapper(stl::move(args));
					this->_is_allocated = true;
				}

				constexpr ArgumentWrapper get() const
				{
					assert(this->_is_allocated);
					return this->raw();
				}

				constexpr void clear()
				{
					assert(this->_is_allocated);
					auto& packed = this->raw();

					packed.~ArgumentWrapper();
					this->_is_allocated = false;
				}

				ReturnType invoke(Args&&... args)
				{
					return this->_handler(args...);
				}

				template <size_t ... Indices>
				ReturnType dispatch(IndexSequence<Indices...> seq)
				{
					(void)seq;
					assert(this->_is_allocated);
					return this->invoke(stl::get<Indices>(stl::move<ArgumentWrapper >( this->get() )) ... );
				}

				ReturnType operator()()
				{
					return this->dispatch(typename MakeIndexSequence<sizeof...(Args)>::type());
				}

			private:
				typename Base::Handler _handler;
				stl::Array<char, sizeof(ArgumentWrapper)> _buffer;
				bool _is_allocated;

				constexpr ArgumentWrapper& raw() const
				{
					return *reinterpret_cast<ArgumentWrapper *>((char*)this->_buffer.data());
				}
			};

			constexpr DispatchQueueBase() :
				_lock(),  _queue(), _notfull_event(),
				_empty_event(), _capacity(ThisClass::DefaultCapacity)
			{ }

			constexpr DispatchQueueBase(size_t capacity) :
				_lock(), _queue(), _notfull_event(), _empty_event(), _capacity(capacity)
			{ }

			template <typename Func>
			inline void enqueue(Func&& handler, Args&&... args)
			{
				Work w(handler);
				LockGuard g(this->_lock);

				if(this->_queue.size() >= this->capacity()) {
					auto result = this->_notfull_event.wait(g, ThisClass::Timeout);

					if(!result)
						return;
				}

				w.set(ThisClass::MakeArgumentWrapper(stl::forward<Args>(args)...));
				this->_queue.push_back(lwiot::stl::move(w));
				this->_empty_event.signal();
			}

			template <typename Func>
			inline int enqueue_irq(Func&& handler, Args&&... args)
			{
				Work w(handler);

				if(this->_queue.size() >= this->capacity())
					return -EINUSE;

				w.set(ThisClass::MakeArgumentWrapper(stl::forward<Args>(args)...));
				this->_queue.push_back(lwiot::stl::move(w));
				return -EOK;
			}

			[[stack_protect]]
			inline ssize_t process() noexcept
			{
				LockGuard g(this->_lock);
				Queue copy = Queue();
				auto empty = this->_queue.empty();

				if(unlikely(empty))
					return -ETRYAGAIN;

				stl::swap(this->_queue, copy);
				this->_notfull_event.signal();
				g.unlock();

				stl::foreach(copy, [&](typename Queue::iterator& iter) {
					Work work = stl::move(*iter);
					Dispatcher<ReturnType>::dispatch(copy, iter, stl::move(work));
				});

				g.lock();
				if(!copy.empty())
					this->_queue.append(stl::move(copy));

				return this->_queue.size();
			}

		protected:
			typename Threading::Lock _lock;

			[[stack_protect]]
			inline void wait_for()
			{
				LockGuard g(this->_lock);

				if(!this->empty())
					return;

				this->_empty_event.wait(g, FOREVER);
			}


			constexpr size_t capacity() const
			{
				return this->_capacity;
			}

			constexpr size_t size() const
			{
				return this->_queue.size();
			}

			constexpr bool empty() const
			{
				return this->size() == 0UL;
			}

			inline void clear_events() noexcept
			{
				for(int idx = 0; idx < ThisClass::DefaultQueueSize; idx++) {
					this->_notfull_event.signal();
					this->_empty_event.signal();
				}
			}

		private:
			typename PolicyType::Queue _queue;
			typename Threading::Event _notfull_event;
			typename Threading::Event _empty_event;
			size_t _capacity;

			static constexpr int Timeout = 500;
			static constexpr int DefaultCapacity = 20;
			static constexpr int DefaultQueueSize = 4;

			/* Methods */
			static constexpr auto MakeArgumentWrapper(Args&&... args)
			{
				return stl::MakeTuple(stl::forward<Args>(args)...);
			}
		};

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif
	}
}
