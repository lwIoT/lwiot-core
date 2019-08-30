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

	void operator()() const
	{
		print_dbg("Const operator called!\n");
	}

protected:
	void run() override
	{
		print_dbg("Functor called!");
	}

};

int main(int argc, char **argv)
{
	lwiot::Function<int(int)> fn;

	lwiot_init();
	print_dbg("Testing lambda wrapping..\n");

	FunctorTest functor;
	lwiot::Function<void(void)> functor_wrapper(functor);

	functor_wrapper();

	fn = [&](int a) -> int {
		return a + 1;
	};

	print_dbg("Fn of 15 = %i\n", fn(15));

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
