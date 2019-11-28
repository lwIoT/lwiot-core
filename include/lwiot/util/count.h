/*
 * Lambda loop utilities.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

/// @file count.h

#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/function.h>

namespace lwiot
{
	/**
	 * @brief Count from \p start to \p end.
	 * @param start Starting integer.
	 * @param end Final integer.
	 * @param func Handler function.
	 * @return Return value of \p func.
	 * @ingroup util
	 */
	extern int count(ssize_t start, ssize_t end, Function<int(int)>& func);
	/**
	 * @brief Count from \p start to \p end.
	 * @param start Starting integer.
	 * @param end Final integer.
	 * @param func Handler function.
	 * @return Return value of \p func.
	 * @ingroup util
	 */
	extern int count_up(ssize_t start, ssize_t end, Function<int(int)>& func);
	/**
	 * @brief Down from \p start to \p end.
	 * @param start Starting integer.
	 * @param end Final integer.
	 * @param func Handler function.
	 * @return Return value of \p func.
	 * @ingroup util
	 */
	extern int count_down(ssize_t start, ssize_t end, Function<int(int)>& func);
}
