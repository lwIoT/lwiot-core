/*
 * Legacy header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/stl/sharedpointer.h>

namespace lwiot
{
	template <typename T>
	using SharedPointer = lwiot::stl::SharedPointer<T>;
}