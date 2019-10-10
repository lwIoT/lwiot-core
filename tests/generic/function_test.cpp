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

int main(int argc, char **argv)
{
	lwiot::Function<int(int)> fn;
	lwiot::Function<void(void)> fn2;
	int x = 0;

	lwiot_init();
	print_dbg("Testing lambda wrapping..\n");

	FunctorTest functor;
	lwiot::Function<void(void)> functor_wrapper(functor);

	functor_wrapper();

	fn = [&](int a) -> int {
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

	assert(x == 2);

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
