/*
 * Test if lwIoT properly initialises.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <stdlib.h>

#include <lwiot/application.h>
#include <lwiot/functor.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

class Application : public lwiot::Functor {
	virtual void operator()() override
	{
		print_dbg("Inside init test..\n");
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
