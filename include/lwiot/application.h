/*
 * Application class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/functor.h>
#include <lwiot/lwiot.h>
#include <lwiot/types.h>

namespace lwiot
{
	class Application {
	public:
		explicit Application(Functor& func);
		virtual ~Application();

		void start() const;

	private:
		Functor& _functor;
	};
}
