/*
 * Type traits unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/test.h>
#include <lwiot/vector.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/forward.h>

#include <lwiot/traits/issame.h>
#include <lwiot/traits/isfloatingpoint.h>
#include <lwiot/traits/enableif.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/isintegral.h>

class A {
public:
	A() : x(-1)
	{ }

	A& operator =(int&& x)
	{
		print_dbg("Rvalue move result: %i\n", x);
		this->x = x;
		return *this;
	}

	int x;
};

int main(int argc, char**argv)
{
	A tst;

	lwiot_init();

	static_assert(lwiot::traits::IsSame<A,A>::value, "Type's A and A are not the same!");
	static_assert(lwiot::traits::IsFloatingPoint<float>::value, "Float is not a floating point!");
	static_assert(lwiot::traits::IsFloatingPoint<double >::value, "Float is not a floating point!");
	static_assert(lwiot::traits::IsFloatingPoint<long double>::value, "Float is not a floating point!");
	static_assert(!lwiot::traits::IsFloatingPoint<long>::value, "Long is not a floating point!");
	tst = lwiot::stl::move(4);
	assert(tst.x == 4);

	UNUSED(argc);
	UNUSED(argv);

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
