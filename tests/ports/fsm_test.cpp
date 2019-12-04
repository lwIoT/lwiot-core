/*
 * FSM unit test.
 *
 * @author Michel Megens
 * @author michel@michelmegens.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <stdlib.h>

#include <lwiot/util/application.h>
#include <lwiot/functor.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/stl/tuple.h>

#include <lwiot/kernel/fsm.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

static uint8_t expected[] = {
		2,3,3,4,1,2,3,3,4,5
};

static uint8_t actual[] = {
		0,0,0,0,0,0,0,0,0
};

using FsmType = lwiot::FSM;
using HandlerType = FsmType::HandlerType;
using FsmStateType = FsmType::StateType;

class MySignal : public FsmType::SignalType {
public:
	MySignal() : FsmType::SignalType(), done(false), y(21), value(9)
	{ }

	MySignal(int x, int y) : FsmType::SignalType(), done(false), y(y), value(x)
	{ }

	void log()
	{
		print_dbg("Signal value: [%i:%i]\n", y, value);
	}

	MySignal(MySignal&& other) noexcept : done(other.done), y(other.y), value(other.value)
	{
		other.y = other.value = 0;
	}

	MySignal& operator=(MySignal&& rhs) noexcept
	{
		this->y = rhs.y;
		this->value = rhs.value;

		rhs.y = rhs.value = 0;
		return *this;
	}

	bool done;

private:
	int y;
	int value;
};

class Application : public lwiot::Functor {
	void test_fsm()
	{
		FsmType fsm;
		FsmStateType state1;
		FsmStateType state2;
		FsmStateType state3;
		FsmStateType state4;
		FsmStateType state5;
		FsmStateType super;
		int s1_executed = 0;
		bool s4_executed = false;
		int idx = 0;

		using namespace lwiot;
		print_dbg("FSM size: %u\n", sizeof(FsmStateType));

		state5.setAction([&](const FsmType::SignalPointer& ptr) {
			auto signal = SignalAs<MySignal>(ptr);

			print_dbg("State 5: ");
			signal->log();
			actual[idx++] = 5;

			return true;
		});

		state1.setAction([&](const stl::SharedPointer<FsmType::SignalType>& signal) {
			auto value = SignalAs<MySignal>(signal);

			assert(value);
			assert(s1_executed <= 2);
			s1_executed = true;

			MySignal s(1, 3);
			s1_executed++;

			print_dbg("State 1: ");
			fsm.transition(1, stl::move(s));
			value->log();
			actual[idx] = 1;
			idx += 1;

			return true;
		});

		print_dbg("State size: %u\n", sizeof(state1));

		state2.setAction([&](const stl::SharedPointer<FsmType::SignalType>& signal) {
			auto value = SignalAs<MySignal>(signal);
			assert(value);
			MySignal s(12, 31);

			fsm.raise(1, stl::move(s));

			print_dbg("State 2: ");
			value->log();
			actual[idx++] = 2;

			return true;
		});

		state3.setAction([&](const stl::SharedPointer<FsmType::SignalType>& signal) {
			auto value = SignalAs<MySignal>(signal);
			assert(value);
			MySignal s(1, 3);

			s.done = true;

			if(value->done)
				fsm.transition(3, stl::move(s));
			else
				fsm.transition(2, stl::move(s));

			print_dbg("State 3: ");
			value->log();
			actual[idx++] = 3;

			return true;
		});

		state4.setAction([&](const stl::SharedPointer<FsmType::SignalType>& signal) {
			auto value = SignalAs<MySignal>(signal);
			auto rv = !s4_executed;

			print_dbg("State 4: ");
			value->log();

			s4_executed = true;

			actual[idx++] = 4;
			return rv;
		});

		fsm.addTransition(state5, 4, state5);
		fsm.addTransition(state5, 3, state5);
		fsm.addTransition(state5, 2, state5);
		fsm.addTransition(state5, 1, state5);

		fsm.addTransition(super, 4, state1);
		fsm.addTransition(super, 1, state1);

		fsm.addTransition(state1, 1, state2, [](const stl::SharedPointer<FsmType::SignalType>& signal) { return true; });
		fsm.addTransition(state1, 2, state1);
		fsm.addTransition(state1, 3, state1);
		fsm.addTransition(state1, 4, state1);

		fsm.addTransition(state2, 1, state3, [](const stl::SharedPointer<FsmType::SignalType>& signal) { return true; });
		fsm.addTransition(state2, 2, state1);
		fsm.addTransition(state2, 3, state1);
		fsm.addTransition(state2, 4, state1);

		fsm.addTransition(state3, 2, state3, [](const stl::SharedPointer<FsmType::SignalType>& signal) { return true; });
		fsm.addTransition(state3, 3, state4, [](const stl::SharedPointer<FsmType::SignalType>& signal) { return true; });

		fsm.addTransition(state4, 2, state4);
		fsm.addTransition(state4, 3, state1, [](const stl::SharedPointer<FsmType::SignalType>& signal) { return true; });

		state4.setParent(super);
		state3.setParent(super);

		auto s_super = fsm.addState(super);
		auto s1 = fsm.addState(state1);
		auto s2 = fsm.addState(state2);
		auto s3 = fsm.addState(state3);
		auto s4 = fsm.addState(state4);
		auto s5 = fsm.addState(state5);

		fsm.addStopState(s4.first);
		fsm.setStartState(s1.first);
		fsm.setErrorState(s5.first);

		assert(s1.second);
		assert(s2.second);
		assert(s3.second);
		assert(s4.second);
		assert(s5.second);
		assert(s_super.second);

		assert(fsm.valid());
		assert(fsm.deterministic());

		fsm.start();

		MySignal signal1;
		MySignal signal2;

		fsm.raise(1, stl::move(signal1));
		auto now = lwiot_tick_ms();
		auto done = false;

		fsm.block(false);

#define RAISE_TMO 100
#define DONE_TMO  1000

		for(;;) {
			if(lwiot_tick_ms() > now+RAISE_TMO && !done) {
				fsm.raise(4, stl::move(signal2));
				done = true;
			}

			if(lwiot_tick_ms() > now+DONE_TMO && done)
				break;

			fsm.run();
		}

		fsm.stop();

		assert(s1_executed <= 2);

		for(auto i = 0UL; i < sizeof(actual); i++) {
			assert(actual[i] == expected[i]);
		}
	}

	void run() override
	{
		print_dbg("Running FSM unit tests!\n");

		this->test_fsm();
	}
} app;

static void start_app()
{
	lwiot::Application testapp(app);
	testapp.start();
}

static void main_thread(void *arg)
{
	start_app();
	wait_close();
#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t* tp = lwiot_thread_create(main_thread, "main" , nullptr);

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif
	lwiot_thread_destroy(tp);

	return -EXIT_SUCCESS;
}
