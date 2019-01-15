/*
 * Lambda loop utilities.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/function.h>

/*template<typename Func>
static inline int count_down(ssize_t start, ssize_t end, Func functor)
{
	int rv = 0;

	for(ssize_t i = start; i >= end && rv >= 0; i--)
		rv = functor(i);

	return rv;
}

template<typename Func>
static inline int count_up(ssize_t start, ssize_t end, Func functor)
{
	int rv = 0;

	for(ssize_t i = start; i < end && rv >= 0; i++)
		rv = functor(i);

	return rv;
}

template<typename Func>
static inline int count(ssize_t start, ssize_t end, Func functor)
{
	if(start < end)
		return count_up(start, end, functor);
	else
		return count_down(start, end, functor);
}*/

namespace lwiot
{
	extern int count(ssize_t start, ssize_t end, Function<int(*)(int)>& func);
	extern int count_up(ssize_t start, ssize_t end, Function<int(*)(int)>& func);
	extern int count_down(ssize_t start, ssize_t end, Function<int(*)(int)>& func);
}
