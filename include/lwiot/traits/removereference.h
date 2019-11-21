/*
 * Remove reference type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot
{
	namespace traits
	{
		template<typename _Tp>
		struct RemoveReference {
			typedef _Tp type;
		};

		template<typename _Tp>
		struct RemoveReference<_Tp &> {
			typedef _Tp type;
		};

		template<typename _Tp>
		struct RemoveReference<_Tp &&> {
			typedef _Tp type;
		};
	}
}
