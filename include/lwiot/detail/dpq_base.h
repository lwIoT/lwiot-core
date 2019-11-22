/*
 * Dispatch queue base implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/**
 * @file dpq_base.h Dispatch queue base header.
 */

#pragma once

#include <lwiot/log.h>
#include <lwiot/function.h>

#include <lwiot/io/watchdog.h>

#include <lwiot/traits/iscallable.h>
#include <lwiot/traits/ismoveconstructible.h>
#include <lwiot/traits/issame.h>
#include <lwiot/traits/isconstructible.h>
#include <lwiot/traits/isconvirtible.h>

#include <lwiot/stl/array.h>
#include <lwiot/stl/forward.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/foreach.h>
#include <lwiot/stl/tuple.h>

#include "dpq_policy.h"

namespace lwiot
{
	namespace detail
	{
		template <typename T>
		struct IsVoid : public traits::IsSame<T, void> { };

		template <typename T>
		struct IsBool : public traits::IsSame<T, bool> {};

		template <size_t... Indices>
		struct IndexSequence {
			using Type = IndexSequence<Indices...>;

			constexpr size_t size() const noexcept
			{
				return sizeof...(Indices);
			}
		};

		template <size_t idx, size_t... indices>
		struct MakeIndexSequence : MakeIndexSequence<idx - 1, idx - 1, indices...> {
		};

		template <size_t... indices>
		struct MakeIndexSequence<0, indices...> {
			using Type = IndexSequence<indices...>;
		};

		/*
		 * Dispatch work using the SFINAE principle. One implementation for `void'
		 * return types and one implementation * for boolean and convirtable to boolean.
		 */
		template <typename T, typename ... Args>
		struct Dispatcher {
			template <typename Worker, typename ReturnType = T>
			static inline
			traits::EnableIf_t<traits::Not<IsVoid<ReturnType>>::value, bool>
			dispatch(Worker& work)
			{
				/* Convert to bool and return */
				bool retval = work.dispatch(typename MakeIndexSequence<sizeof...(Args)>::Type());
				return retval;
			}

			template <typename Worker, typename ReturnType = T>
			static inline
			traits::EnableIf_t<IsVoid<ReturnType>::value, bool>
			dispatch(Worker& work)
			{
				work.dispatch(typename MakeIndexSequence<sizeof...(Args)>::Type());
				return true;
			}
		};

		template <typename...T>
		struct TemplateAll;

		template <>
		struct TemplateAll<> : public traits::TrueType {
		};

		template <typename... T>
		struct TemplateAll<traits::FalseType, T...> : public traits:: FalseType {
		};

		template <typename ... T>
		struct TemplateAll<traits::TrueType, T...> : public TemplateAll<T...>::type {
		};

		template <typename Proto, typename ... Args>
		class DispatchQueueBase_helper {
		private:
			static constexpr bool IsMoveConstructible()
			{
				return TemplateAll<typename traits::IsMoveConstructible<Args>::type...>::value;
			}

			static constexpr bool IsCallableUsingArgs()
			{
				return traits::IsCallable<Proto, Args...>::value;
			}

			static_assert(IsMoveConstructible(), "Arguments must be move constructible!");
			static_assert(IsCallableUsingArgs(), "Prototype is not callable with the given arguments!");

		protected:
			stl::ReferenceWrapper<Watchdog> _watchdog;

		public:
			typedef Function<Proto> HandlerType;

			constexpr DispatchQueueBase_helper() : _watchdog(wdt)
			{ }
		};

#ifdef DOXYGEN
		/**
		 * @brief Dispatch queue base class implementation.
		 *
		 * A dispatch queue is a threaded work queue.
		 *
		 * @tparam Handler Work handler type.
		 * @tparam Policy  Polciy type.
		 */
		template <typename Handler, template <typename> typename Policy>
		class DispatchQueueBase : public DispatchQueueBase_helper<Handler> {
#else

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
		template <typename, template <typename> typename>
		class DispatchQueueBase;

		template <template <typename> typename P, typename R, typename... Args>
		class DispatchQueueBase<R(Args...), P> : public DispatchQueueBase_helper<R(Args...), Args...> {
#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif
#endif
			struct Work;
		public:
			typedef R ReturnType; //!< Handler return type.
			typedef P<Work> PolicyType; //!< DPQ polity type.

		private:
			static constexpr bool IsConvirtableToBool()
			{
				constexpr auto is_void = IsVoid<ReturnType>::value;
				constexpr auto is_bool = IsBool<ReturnType>::value;
				constexpr auto is_conv = traits::IsConvirtable<ReturnType, bool>::value;

				return is_void || is_bool || is_conv;
			}

			static_assert(IsConvirtableToBool(), "Return type must be void, bool or convirtable to bool!");
			using ArgumentWrapper = stl::Tuple<typename traits::RemoveCv<typename traits::RemoveCv<Args>::type>::type...>;

		public:
			static constexpr bool Threaded = detail::HasThreading<PolicyType>::Value; //!< Threading indicator.

			typedef typename detail::SelectThreadingPolicy<PolicyType, Threaded>::Type Threading; //!< Threading policy.
			typedef DispatchQueueBase_helper<ReturnType(Args...), Args...> Base; //!< Base class definition.
			typedef DispatchQueueBase<ReturnType(Args...), P> ThisClass; //!< Definition of *this type.
			typedef UniqueLock<typename Threading::Lock> LockGuard; //!< Lock type.
			typedef typename PolicyType::Queue Queue; //!< Queue type.
			typedef size_t size_type; //!< Size type.

		private:
			struct Work {
			public:
				constexpr Work() : _handler(), _buffer(), _is_allocated(false)
				{ }

				Work(Work&& rhs) noexcept : _is_allocated(false)
				{
					this->_is_allocated = rhs._is_allocated;
					this->_buffer = stl::move(rhs._buffer);
					this->_handler = stl::move(rhs._handler);

					rhs._is_allocated = false;
				}

				Work(const Work& other) : _is_allocated(false)
				{
					this->_is_allocated = other._is_allocated;
					this->_buffer = other._buffer;
					this->_handler = other._handler;
				}

				~Work()
				{
					if(this->_is_allocated)
						this->clear();
				}

				Work& operator=(Work&& rhs) noexcept
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

				void setHandler(const typename  Base::HandlerType& handler)
				{
					this->_handler = handler;
				}

				template <typename Func>
				void setHandler(Func&& handler)
				{
					this->_handler = stl::forward<Func>(handler);
				}

				void setArguments(ArgumentWrapper&& args)
				{
					new(this->_buffer.data()) ArgumentWrapper(stl::move(args));
					this->_is_allocated = true;
				}

				constexpr ArgumentWrapper get()
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
					UNUSED(seq);
					assert(this->_is_allocated);

					return this->invoke(stl::get<Indices>(stl::move<ArgumentWrapper >( this->get() )) ... );
				}

			private:
				typename Base::HandlerType _handler;
				typename stl::Array<uint8_t, sizeof(ArgumentWrapper)> _buffer;
				bool _is_allocated;

				/* private methods */
				constexpr ArgumentWrapper& raw()
				{
					return *reinterpret_cast<ArgumentWrapper *>(this->_buffer.data());
				}
			};

		public:
			/**
			 * @brief Create a new dispatch queue object.
			 */
			explicit DispatchQueueBase() : Base(), _lock(), _empty_event(), _not_full_event(),
				_queue(), _capacity(DefaultCapacity), _timeout(DefaultTimeout)
			{
			}

			/**
			 * @brief Create a new dispatch queue object.
			 * @param capacity DPQ capacity.
			 * @param tmo Timout. Default to DefaultTimeout.
			 */
			explicit DispatchQueueBase(size_type capacity, int tmo = DefaultTimeout) : Base(), _lock(),
				_empty_event(), _not_full_event(), _queue(), _capacity(capacity), _timeout(tmo)
			{
			}

			/**
			 * @brief Copy a dispatch queue.
			 * @param other DPQ to copy.
			 */
			DispatchQueueBase(DispatchQueueBase&& other) noexcept : Base(), _lock(),
				_empty_event(stl::move(other._empty_event)), _not_full_event(stl::move(other._not_full_event)),
				_queue(stl::move(other._queue)), _capacity(other._capacity), _timeout(other._timeout)
			{
			}

			/**
			 * @brief Dispatch queue destructor.
			 */
			virtual ~DispatchQueueBase()
			{
				LockGuard g(this->_lock);

				this->clear_waiters();

				if(this->_queue.size() <= 0)
					return;

				this->_queue.clear();
			}

			/**
			 * @brief Copy assignment operator.
			 * @param rhs DPQ to copy into *this.
			 * @return A reference to *this.
			 */
			DispatchQueueBase& operator=(DispatchQueueBase&& rhs) noexcept
			{
				this->move(rhs);
				return *this;
			}

			DispatchQueueBase(const DispatchQueueBase&) = delete;
			DispatchQueueBase& operator=(const DispatchQueueBase&) = delete;

			/**
			 * @brief Enqueue work for future execution.
			 * @param handler Handler to enqueue.
			 * @param args Arguments to pass to \p handler when \p handler is executed.
			 */
			inline void enqueue(const typename Base::HandlerType& handler, Args&&... args)
			{
				LockGuard g(this->_lock);

				if(this->_queue.size() >= this->_capacity) {
					do {
						this->_not_full_event.wait(g, this->_timeout);
						this->_watchdog->reset();
					} while(this->_queue.size() >= this->_capacity);
				}

				this->__enqueue(handler, stl::forward<Args>(args)...);
				this->_empty_event.signal();
			}

			/**
			 * @brief Enqueue work for future execution.
			 * @tparam Func Function object pointer.
			 * @param handler Handler to enqueue.
			 * @param args Arguments to pass to \p handler when \p handler is executed.
			 */
			template <typename Func>
			inline void enqueue(Func&& handler, Args&&... args)
			{
				LockGuard g(this->_lock);

				if(this->_queue.size() >= this->_capacity) {
					do {
						this->_not_full_event.wait(g, this->_timeout);
						this->_watchdog->reset();
					} while(this->_queue.size() >= this->_capacity);
				}

				this->__enqueue(stl::forward<Func>(handler), stl::forward<Args>(args)...);
				this->_empty_event.signal();
			}

			/**
			 * @brief Enqueue work for future execution from IRQ context.
			 * @tparam Func Function object pointer.
			 * @param handler Handler to enqueue.
			 * @param args Arguments to pass to \p handler when \p handler is executed.
			 */
			template <typename Func>
			inline void enqueue_irq(Func&& handler, Args&&... args)
			{
				this->__enqueue(stl::forward<Func>(handler), stl::forward<Args>(args)...);
				this->_empty_event.signalFromIrq();
			}

			/**
			 * @brief Enqueue work for future execution from IRQ context.
			 * @param handler Handler to enqueue.
			 * @param args Arguments to pass to \p handler when \p handler is executed.
			 */
			inline void enqueue_irq(const typename Base::HandlerType& handler, Args&&... args)
			{
				this->__enqueue(stl::forward(handler), stl::forward<Args>(args)...);
				this->_empty_event.signalFromIrq();
			}

			/**
			 * @brief Swap two dispatch queue objects.
			 * @param a Dispatch queue 1.
			 * @param b Dispatch queue 2.
			 */
			friend void swap(ThisClass& a, ThisClass& b) noexcept
			{
				using lwiot::stl::swap;

				if(&a < &b) {
					a._lock.lock();
					b._lock.lock();
				} else {
					b._lock.lock();
					a._lock.lock();
				}

				swap(a._queue, b._queue);
				swap(a._capacity, b._capacity);
				swap(a._timeout, b._timeout);

				if(&a < &b) {
					b._lock.unlock();
					a._lock.unlock();
				} else {
					a._lock.unlock();
					b._lock.unlock();
				}
			}

			/**
			 * @brief Process the current state of the dispatch queue.
			 * @return The number of work items remaining on the queue due to execution errors.
			 *
			 * This member function will execute the available work on the work queue.
			 */
			[[cxx_optimize]]
			inline ssize_t process() noexcept
			{
				Queue copy;
				LockGuard g(this->_lock);

				if(unlikely(this->_queue.empty()))
					return -EOK;

				using lwiot::stl::swap;
				swap(this->_queue, copy);
				g.unlock();

				stl::foreach(copy, [&](typename Queue::iterator& iter) {
					auto work = stl::move(*iter);
					auto result = Dispatcher<ReturnType, Args...>::dispatch(work);

					if(likely(result)) {
						copy.erase(iter);
						work.clear();
					} else {
						*iter = stl::move(work);
					}

					this->_watchdog->reset();
				});

				g.lock();

				for(auto& work : copy) {
					this->_queue.push_back(stl::move(work));
				}

				if(this->_queue.size() < this->_capacity)
					this->_not_full_event.signal();

				return this->_queue.size();
			}

			/**
			 * @brief Get the capacity of the DPQ.
			 * @return The capacity of the dispatch queue.
			 */
			constexpr size_type capacity() const
			{
				return this->_capacity;
			}

			/**
			 * @brief Get the size of the DPQ.
			 * @return The size of the dispatch queue.
			 */
			inline size_type size() const
			{
				LockGuard g(this->_lock);
				return this->_queue.size();
			}

			/**
			 * @brief Get the DPQ timeout value.
			 * @return The DPQ timeout value.
			 */
			constexpr int timeout() const
			{
				return this->_timeout;
			}

		protected:
			mutable typename Threading::Lock _lock; //!< Dispatch queue lock.
			static constexpr size_type DefaultCapacity = 20; //!< Default capacity.
			static constexpr int DefaultTimeout = 100; //!< Default timeout value.

			/**
			 * @brief Move \p other into \p *this.
			 * @param other DPQ to move into \p *this.
			 */
			constexpr void move(ThisClass& other)
			{
				using stl::swap;
				swap(*this, other);
			}

			/**
			 * @brief Lock the dispatch queue.
			 */
			void lock()
			{
				this->_lock.lock();
			}

			/**
			 * @brief Unlock the dispatch queue.
			 */
			void unlock()
			{
				this->_lock.unlock();
			}

			/**
			 * @brief Wait for the queue to become non-empty.
			 */
			inline void wait_for()
			{
				bool success;
				LockGuard g(this->_lock);

				if(!this->_queue.empty())
					return;

				do {
					success = this->_empty_event.wait(g, this->timeout());
					this->_watchdog->reset();
				} while(!success);
			}

			/**
			 * @brief Signal threads waiting on a dispatch queue event.
			 * @see _not_full_event
			 * @see _empty_event
			 */
			inline void clear_waiters()
			{
				for(size_type idx = 0; idx < this->_capacity; idx++) {
					this->_not_full_event.signal();
					this->_empty_event.signal();
				}
			}

		private:
			typename Threading::Event _empty_event;
			typename Threading::Event _not_full_event;
			Queue _queue;
			size_type _capacity;
			int _timeout;

			static constexpr auto MakeArgumentWrapper(Args&&... args)
			{
				return stl::MakeTuple(stl::forward<Args>(args)...);
			}

			template <typename Func>
			inline void __enqueue(Func&& handler, Args&&... args)
			{
				Work work;

				if(this->_queue.size() >= this->capacity())
					return;

				work.setHandler(stl::forward<Func>(handler));
				work.setArguments(stl::move(ThisClass::MakeArgumentWrapper(stl::forward<Args>(args)...)));
				this->_queue.push_back(stl::move(work));
			}
		};
	}
}
