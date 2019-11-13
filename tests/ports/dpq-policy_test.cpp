/*
 * Dispatch queue test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/util/application.h>

#include <lwiot/detail/dpq_policy.h>

using Policy = lwiot::detail::MultiThreadingPolicy<int>;

typedef typename lwiot::detail::SelectThreadingPolicy< Policy , lwiot::detail::HasThreading<Policy >::Value >::Type Threading;
static_assert(lwiot::detail::HasThreading<Policy>::Value, "Multi-threading policy has no threading!");

class Application : public lwiot::Functor {
	void run() override
	{
		print_dbg("Inside init test..\n");

		static_assert(!lwiot::detail::HasThreading<lwiot::detail::SingleThreadingPolicy<int>>::Value, "Unexpected support for threading!");
		static_assert(lwiot::detail::HasThreading<lwiot::detail::MultiThreadingPolicy<int>>::Value, "Threading not supported!");
	}
} app;

int main(int argc, char **argv)
{
	printf("Running lwIoT init test!\n");

	lwiot::Application testapp(app);
	testapp.start();

	wait_close();
	return -EXIT_SUCCESS;
}
