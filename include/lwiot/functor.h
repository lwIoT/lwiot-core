/*
 * Functor interface definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	class Functor {
	public:
		explicit Functor() = default;
		virtual ~Functor() = default;

		void operator()()
		{
			this->run();
		}

	protected:
		virtual void run() = 0;
	};
}
