/*
 * Definition and implementation of `stl::foreach()'.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/**
 * @file foreach.h Foreach header.
 */

/**
 * @addtogroup stl
 * @{
 */

#pragma once

namespace lwiot
{
	namespace stl
	{
		/**
		 * @brief Iterate a collection from begin to end.
		 * @tparam Collection Collection type.
		 * @tparam Func Handler type. Must be callable.
		 * @param collection Collection to iterate.
		 * @param handler Iteration handler. Called for each object in \p collection.
		 */
		template <typename Collection, typename Func>
		constexpr void foreach(Collection& collection, Func&& handler)
		{
			for(auto iter = collection.begin(); iter != collection.end(); ++iter) {
				if(collection.size() == 0)
					break;

				handler(iter);
			}
		}
	}
}

/** @} */
