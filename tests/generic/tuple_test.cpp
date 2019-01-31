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
#include <lwiot/kernel/event.h>
#include <lwiot/stl/vector.h>

template<typename T>
struct identity {
	typedef T type;
};

template<typename ReturnType>
struct Dispatcher {
	template<typename QueueType, typename Iterator, typename QueuedItemType>
	static constexpr ReturnType dispatch(QueueType &queue, Iterator& iter, QueuedItemType&& item)
	{
		auto result = item();

		if(likely(result)) {
			queue.erase(iter);
			item.clear();
		} else {
			queue.replace(iter, lwiot::stl::forward<QueuedItemType&&>(item));
		}

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

	//return false;
	return true;
}

template<size_t... Indexes>
struct IndexSequence {

};

template<size_t N, size_t ...Indexes>
struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Indexes...> {
};

template<size_t ...Indexes>
struct MakeIndexSequence<0, Indexes...> {
	using Type = IndexSequence<Indexes...>;
};

template<typename Proto, typename P>
class TupleTest;

template<typename P, typename R, typename ...Args>
class TupleTest<R(Args...), P> {
public:
	typedef P PolicyType;
	typedef R ReturnType;
	typedef lwiot::Function<ReturnType(Args...)> HandlerType;
	typedef TupleTest<P, R(Args...)> Base;

	using QueuedEvent = lwiot::stl::Tuple<
			typename lwiot::traits::RemoveCv<typename lwiot::traits::RemoveCv<Args>::type>::type...
	>;

	static_assert(lwiot::IsVoid<ReturnType>::value || IsBool<ReturnType>::value, "Handler return value should be of type bool or void!");

	class QueuedItem {
	public:
		QueuedItem() : buffer(), allocated(false)
		{
		}

		~QueuedItem()
		{
			if(this->allocated) {
				this->clear();
			}
		}

		ReturnType invoke(Args... args)
		{
			return this->handler(args...);
		}

		template<size_t ...Indexes>
		constexpr R dispatch(IndexSequence<Indexes...>)
		{
			return this->invoke(lwiot::stl::get<Indexes>(
					lwiot::stl::forward<QueuedEvent>( this->get() ))...
			);
		}

		constexpr R operator()()
		{
			return this->dispatch(typename MakeIndexSequence<sizeof...(Args)>::Type());
		}

		QueuedItem(QueuedItem &&) = default;
		QueuedItem& operator=(QueuedItem&&) = default;

		QueuedItem(const QueuedItem &item) noexcept : handler(item.handler), buffer(item.buffer)
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
			assert(this->allocated);

			return *reinterpret_cast<QueuedEvent *>(buffer.data());
		}

		void clear()
		{
			assert(this->allocated);

			get().~QueuedEvent();
			this->allocated = false;
		}

		HandlerType handler;

	private:
		lwiot::stl::Array<char, sizeof(QueuedEvent)> buffer;
		bool allocated;
	};

	typedef lwiot::stl::LinkedList<QueuedItem> Queue;

	constexpr TupleTest()
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

		q.push_back(this->q);
		auto value = lwiot::stl::move(q.front());
		auto iter = q.begin();
		Dispatcher<ReturnType>::dispatch(q, iter, lwiot::stl::move(value));
		assert(q.size() != 1);
	}

	constexpr void print()
	{
		print_dbg("Value of 1: %i\n", lwiot::stl::get<0>(this->q.get()));
		print_dbg("Value of 2: %i\n", lwiot::stl::get<1>(this->q.get()));
		print_dbg("Value of 3: %f\n", lwiot::stl::get<2>(this->q.get()));
	}

private:
	QueuedItem q;
	lwiot::Event _notfull_event;
	lwiot::stl::LinkedList<QueuedItem> _items;
};

int main(int argc, char **argv)
{
	lwiot_init();
	TupleTest<bool(int, int, double), int> eq;

	eq.enqueue(1, 4, 1.1234);
	eq.dispatch();

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
