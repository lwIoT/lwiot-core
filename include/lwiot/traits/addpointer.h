/*
 * AddPointer type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/isreference.h>
#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace traits
	{
#ifdef WIN32
		template<class _Ty>
		using remove_reference_t = typename RemoveReference<_Ty>::type;

		template<class... _Types>
		using void_t = void;

		template<class _Ty,
		class = void>
		struct _Add_pointer
		{	// add pointer
		using type = _Ty;
		};

		template<class _Ty>
		struct _Add_pointer<_Ty, void_t<remove_reference_t<_Ty> *>>
		{	// add pointer
		using type = remove_reference_t<_Ty> *;
		};


		template<class _Ty>
		using add_pointer_t = typename _Add_pointer<_Ty>::type;

		template<class _Ty>
		struct AddPointer 
		{
			using type = typename _Add_pointer<_Ty>::type;
		};
#else
		template<typename _Tp, bool = traits::Or < IsReferenceable<_Tp>, traits::IsSame <_Tp, void>> ::value>
		struct AddPointer_helper {
			typedef _Tp type;
		};

		template<typename _Tp>
		struct AddPointer_helper<_Tp, true> {
			typedef typename RemoveReference<_Tp>::type *type;
		};

		template<typename _Tp>
		struct AddPointer : public AddPointer_helper<_Tp> {
		};
#endif
	}
}
