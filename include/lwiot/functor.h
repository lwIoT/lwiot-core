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

		virtual void operator()() = 0;
	};
}
