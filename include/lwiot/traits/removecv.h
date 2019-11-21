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
		template<typename T>
		struct RemoveConst {
			typedef T type;
		};

		template<typename T>
		struct RemoveConst<T const> {
			typedef T type;
		};

		template<typename T>
		struct RemoveVolatile {
			typedef T type;
		};

		template<typename T>
		struct RemoveVolatile<T volatile> {
			typedef T type;
		};

		template<typename T>
		struct RemoveCv {
			typedef typename RemoveConst<typename RemoveVolatile<T>::type>::type type;
		};

		template<typename _Tp>
		using __remove_cvref_t = typename RemoveCv<typename RemoveReference<_Tp>::type>::type;

		template<typename _Tp>
		struct AddConst {
			typedef _Tp const type;
		};

		template<typename _Tp>
		struct AddVolatile {
			typedef _Tp volatile type;
		};

		template<typename _Tp>
		struct AddCv {
			typedef typename AddConst<typename AddVolatile<_Tp>::type>::type type;
		};
	}
}
