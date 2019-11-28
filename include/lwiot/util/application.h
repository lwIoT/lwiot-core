/*
 * Application class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file application.h

#pragma once

#include <lwiot/functor.h>
#include <lwiot/lwiot.h>
#include <lwiot/types.h>

namespace lwiot
{
	/**
	 * @brief Application class.
	 * @ingroup util
	 */
	class Application {
	public:
		/**
		 * @brief Construct a new application object.
		 * @param func Callable object.
		 */
		explicit Application(Functor& func);

		/**
		 * @brief Destruct an application.
		 */
		virtual ~Application();

		/**
		 * @brief Start the application.
		 */
		void start() const;

	private:
		Functor& _functor;
	};
}
