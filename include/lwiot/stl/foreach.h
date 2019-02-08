/*
 * Definition and implementation of `stl::foreach()'.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	namespace stl
	{
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
