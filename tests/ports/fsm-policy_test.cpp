/*
 * Finite state machine policy unit test.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#include <stdlib.h>

#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/util/application.h>

#include <lwiot/detail/fsm_policy.h>
#include <lwiot/detail/fsm_base.h>

typedef lwiot::detail_fsm::MultiThreadingPolicy MultiPolicy;
typedef lwiot::detail_fsm::SingleThreadingPolicy SinglePolicy;

static_assert(lwiot::detail_fsm::HasThreading<MultiPolicy>::Value, "Multi-threading policy has no threading!");
static_assert(!lwiot::detail_fsm::HasThreading<SinglePolicy>::Value, "Single-threading policy has threading!");

class Application : public lwiot::Functor {
	void run() override
	{
		print_dbg("Inside FSM policy test..\n");
	}
} app;

int main(int argc, char **argv)
{
	lwiot::Application testapp(app);
	testapp.start();

	wait_close();
	return -EXIT_SUCCESS;
}
