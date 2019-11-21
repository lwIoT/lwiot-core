
/*
 * Scalar type traits.
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
		struct __is_member_pointer_helper : public FalseType { };

		template<typename _Tp, typename _Cp>
		struct __is_member_pointer_helper<_Tp _Cp::*> : public TrueType { };

		template<typename _Tp>
		struct IsMemberPointer : public __is_member_pointer_helper<typename RemoveCv<_Tp>::type>::type
		{ };

		template<typename>
		struct __is_member_object_pointer_helper
				: public FalseType { };

		template<typename _Tp, typename _Cp>
		struct __is_member_object_pointer_helper<_Tp _Cp::*>
				: public IntegralConstant<bool, !IsFunction<_Tp>::value> { };

		template<typename _Tp>
		struct IsMemberObjectPointer : public __is_member_object_pointer_helper< typename RemoveCv<_Tp>::type>::type
		{ };

		template<typename>
		struct __is_member_function_pointer_helper : public FalseType { };

		template<typename _Tp, typename _Cp>
		struct __is_member_function_pointer_helper<_Tp _Cp::*> : public IntegralConstant<bool, traits::IsFunction<_Tp>::value> { };

		template<typename _Tp>
		struct IsMemberFunctionPointer : public __is_member_function_pointer_helper< typename RemoveCv<_Tp>::type>::type
		{ };
	}
}

