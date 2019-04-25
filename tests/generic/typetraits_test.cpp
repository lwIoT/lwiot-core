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
#include <lwiot/sharedpointer.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/forward.h>
#include <lwiot/stl/tuple.h>

#include <lwiot/traits/issame.h>
#include <lwiot/traits/isfloatingpoint.h>
#include <lwiot/traits/enableif.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/isintegral.h>

#include <lwiot/traits/addpointer.h>
#include <lwiot/traits/isarray.h>
#include <lwiot/traits/isfunction.h>
#include <lwiot/traits/remove_extent.h>
#include <lwiot/traits/addpointer.h>
#include <lwiot/traits/decay.h>
#include <lwiot/traits/isconvirtible.h>

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

	static_assert(lwiot::traits::IsSame<lwiot::traits::AddPointer<A>::type, A*>::value, "Add pointer not working!");

	static_assert(lwiot::traits::IsArray<A[]>::value, "Type A[] is not an array!");
	static_assert(lwiot::traits::IsArray<A[1234]>::value, "Type A[] is not an array!");

	static_assert(lwiot::traits::IsObject<A>::value, "Type A is not an object!");
	static_assert(lwiot::traits::IsSame<A,A>::value, "Type's A and A are not the same!");
	static_assert(lwiot::traits::IsFloatingPoint<float>::value, "Float is not a floating point!");
	static_assert(lwiot::traits::IsFloatingPoint<double >::value, "Float is not a floating point!");
	static_assert(lwiot::traits::IsFloatingPoint<long double>::value, "Float is not a floating point!");
	static_assert(!lwiot::traits::IsFloatingPoint<long>::value, "Long is not a floating point!");
	static_assert(lwiot::traits::IsConvirtable<int, bool>::value, "Int not convirtable to bool!");
	static_assert(!lwiot::traits::IsConvirtable<A, bool>::value, "Int not convirtable to bool!");


	tst = lwiot::stl::move(4);
	assert(tst.x == 4);

	auto tpl = lwiot::stl::MakeTuple(1, 22.3f, "Abc");

	assert(lwiot::stl::get<1>(tpl) == 22.3f);
	assert(strcmp(lwiot::stl::get<2>(tpl), "Abc") == 0);

	UNUSED(argc);
	UNUSED(argv);

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
