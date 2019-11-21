/*
 * Generic unit test for <function.h>.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/function.h>
#include <lwiot/functor.h>
#include <lwiot/test.h>

#include <lwiot/stl/bind.h>

class FunctorTest : public lwiot::Functor {
public:
	FunctorTest() = default;
	FunctorTest(const FunctorTest& ft) = default;
	FunctorTest( FunctorTest&& ft) = default;

	FunctorTest& operator=(FunctorTest&& rhs) = default;
	FunctorTest& operator=(const FunctorTest& rhs) = default;

protected:
	void run() override
	{
		print_dbg("Functor called!\n");
	}

};

struct test {
	bool tst2(int x)
	{
		print_dbg("Test_2 called!\n");
		return x == 5;
	}

	void tst3(int x)
	{
		print_dbg("Test_3 called!\n");
	}
};

static void test_bind()
{
	test t;
	auto f1 = lwiot::stl::bind(&test::tst2, t, lwiot::stl::placeholders::_1);
	auto f2 = lwiot::stl::bind(&test::tst3, t, lwiot::stl::placeholders::_1);
	int x = 5;

	f1(x);
	lwiot::Function<bool(int)> g1;

	g1 = f1;
	lwiot::Function<void(int)> g2 = f2;

	g1(5);
	g2(x);
}

int main(int argc, char **argv)
{
	lwiot::Function<int(int&)> fn;
	lwiot::Function<void(void)> fn2;
	int x = 0;

	lwiot_init();

	print_dbg("Testing stl::bind()\n");
	test_bind();

	lwiot::Function<bool(test, int)> memfn_wrapper(&test::tst2);
	test t;
	memfn_wrapper(t, 5);

	print_dbg("Testing lambda wrapping..\n");

	FunctorTest functor;
	lwiot::Function<void(void)> functor_wrapper(functor);

	functor_wrapper();

	fn = [&](int& a) -> int {
		a += 2;
		return a + 1;
	};

	fn2 = [&]() {
		print_dbg("Test func called..\n");
		x++;
	};

	fn2();
	fn2 = lwiot::stl::move(functor_wrapper);
	fn2();
	fn2 = [&]() {
		print_dbg("Test func called..\n");
		x++;
	};

	fn2();

	int integer = 10;
	fn(integer);
	assert(integer == 12);

	assert(x == 2);

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
