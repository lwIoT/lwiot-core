/*
 * Functor interface definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file functor.h

#pragma once

namespace lwiot
{
	/**
	 * @ingroup util
	 * @brief Functor wrapper.
	 */
	class Functor {
	public:
		/**
		 * @brief Create a new functor.
		 */
		explicit Functor() = default;

		/**
		 * @brief Destroy a functor.
		 */
		virtual ~Functor() = default;

		/**
		 * @brief Invoke the functor.
		 */
		void operator()()
		{
			this->run();
		}

	protected:
		virtual void run() = 0; //!< Invokation method.
	};
}
