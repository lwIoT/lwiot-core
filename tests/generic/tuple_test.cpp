/*
 * Vector unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/function.h>
#include <lwiot/test.h>
#include <lwiot/stl/linkedlist.h>

#include <lwiot/stl/tuple.h>
#include <lwiot/stl/array.h>

template<typename T>
struct identity {
	typedef T type;
};

template<typename ReturnType>
struct Dispatcher {
	template<typename QueueType, typename QueuedItemType>
	static constexpr ReturnType dispatch(QueueType &queue, QueuedItemType&& item)
	{
		auto result = item();

		if(likely(result))
			item.clear();
		else
			queue.push_front(item);

		return result;
	}
};

template<>
struct Dispatcher<void> {
	template<typename QueueType, typename QueuedItemType>
	static constexpr void dispatch(QueueType &queue, QueuedItemType&& item)
	{
		item();
		item.clear();
	}
};

template<typename T>
struct IsBool {
	static constexpr bool value = false;
};

template<>
struct IsBool<bool> {
	static constexpr bool value = true;
};

static bool func(int x, int y, double z)
{
	print_dbg("Value of x: %i\n", x);
	print_dbg("Value of y: %i\n", y);
	print_dbg("Value of z: %f\n", z);

	return false;
}

template<size_t... Indexes>
struct IndexSequence {

};

template<size_t N, size_t ...Indexes>
struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Indexes...> {
};

template<std::size_t ...Indexes>
struct MakeIndexSequence<0, Indexes...> {
	using Type = IndexSequence<Indexes...>;
};

template<typename Proto, typename P>
class EventQueueBase;

template<typename P, typename R, typename ...Args>
class EventQueueBase<R(Args...), P> {
public:
	typedef P PolicyType;
	typedef R ReturnType;
	typedef lwiot::Function<ReturnType(*)(Args...)> HandlerType;
	typedef EventQueueBase<P, R(Args...)> Base;

	using QueuedEvent = lwiot::stl::Tuple<
			typename lwiot::traits::RemoveCv<typename lwiot::traits::RemoveCv<Args>::type>::type...
	>;

	static_assert(lwiot::IsVoid<ReturnType>::value || IsBool<ReturnType>::value,
	              "Handler return value should be of type bool or void!");

	class QueuedItem {
	public:
		QueuedItem() : buffer(), allocated(false)
		{
		}

		~QueuedItem()
		{
			if(allocated) {
				clear();
			}
		}

		R invoke(Args... args)
		{
			return this->handler(args...);
		}

		template<size_t ...Indexes>
		constexpr R dispatch(IndexSequence<Indexes...>)
		{
			return this->invoke(lwiot::stl::get<Indexes>(
					lwiot::stl::forward<QueuedEvent>(this->get()))...
			);
		}

		constexpr R operator()()
		{
			return this->dispatch(typename MakeIndexSequence<sizeof...(Args)>::Type());
		}

		QueuedItem(QueuedItem &&) = delete;

		QueuedItem(const QueuedItem &item) : handler(item.handler), buffer(item.buffer)
		{
			this->allocated = item.allocated;
		}

		QueuedItem &operator=(const QueuedItem &) = delete;

		void set(QueuedEvent &&item)
		{
			assert(!allocated);

			new(buffer.data()) QueuedEvent(lwiot::stl::move(item));
			allocated = true;
		}

		QueuedEvent &get()
		{
			assert(allocated);

			return *reinterpret_cast<QueuedEvent *>(buffer.data());
		}

		void clear()
		{
			assert(allocated);

			get().~QueuedEvent();
			allocated = false;
		}

		HandlerType handler;

	private:
		lwiot::stl::Array<char, sizeof(QueuedEvent)> buffer;
		bool allocated;
	};

	typedef lwiot::stl::LinkedList<QueuedItem> Queue;

	constexpr EventQueueBase()
	{
		this->q.handler = HandlerType(func);
	}

	template<typename... A>
	constexpr void enqueue(A... args)
	{
		this->q.set(QueuedEvent(args...));
	}

	constexpr void dispatch()
	{
		Queue q;

		Dispatcher<ReturnType>::dispatch(q, lwiot::stl::move(this->q));
		assert(q.size() == 1);
	}

	constexpr void print()
	{
		print_dbg("Value of 1: %i\n", lwiot::stl::get<0>(this->q.get()));
		print_dbg("Value of 2: %i\n", lwiot::stl::get<1>(this->q.get()));
		print_dbg("Value of 3: %f\n", lwiot::stl::get<2>(this->q.get()));
	}

private:
	QueuedItem q;
};

int main(int argc, char **argv)
{
	lwiot_init();
	EventQueueBase<bool(int, int, double), int> eq;

	eq.enqueue(1, 4, 1.1234);
	eq.dispatch();

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
