/*
 * Application class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/thread.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/application.h>
#include <lwiot/functor.h>

namespace lwiot
{
	Application::Application(Functor& func) : _functor(func)
	{
		lwiot_init();
		print_dbg("Application started...\n");
	}

	Application::~Application()
	{
		print_dbg("Stopping application...\n");
		lwiot_destroy();
	}

	void Application::start() const
	{
		this->_functor();
	}
}
