/*
 * Scalar type traits.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isintegral.h>

namespace lwiot
{
	namespace traits
	{
		template<typename _Tp>
		struct __is_member_pointer_helper : public FalseType { };

		template<typename _Tp, typename _Cp>
		struct __is_member_pointer_helper<_Tp _Cp::*> : public TrueType { };

		/// is_member_pointer
		template<typename _Tp>
		struct IsMemberPointer : public __is_member_pointer_helper<typename RemoveCv<_Tp>::type>::type
		{ };

		template<typename>
		struct __is_null_pointer_helper : public FalseType { };

		template<>
		struct __is_null_pointer_helper<decltype(nullptr)> : public TrueType { };

		template<typename _Tp>
		struct is_null_pointer :
				public __is_null_pointer_helper<typename RemoveCv<_Tp>::type>::type
		{ };

		template <typename _Tp>
		struct IsEnum : public IntegralConstant<bool, __is_enum(_Tp)>
		{ };

		template<typename>
		struct __is_floating_point_helper : public FalseType { };

		template<>
		struct __is_floating_point_helper<float> : public TrueType { };

		template<>
		struct __is_floating_point_helper<double> : public TrueType { };

		template<>
		struct __is_floating_point_helper<long double> : public TrueType { };

		template<typename _Tp>
		struct IsFloatingPoint : public __is_floating_point_helper<typename RemoveCv<_Tp>::type>::type
		{ };

		template<typename _Tp>
		struct IsArithmetic : public Or<IsIntegral<_Tp>, IsFloatingPoint<_Tp>>::type
		{ };


		template<typename>
		struct __is_pointer_helper : public FalseType { };

		template<typename _Tp>
		struct __is_pointer_helper<_Tp*> : public TrueType { };

		template<typename _Tp>
		struct IsPointer : public __is_pointer_helper<typename RemoveCv<_Tp>::type>::type
		{ };

		template<typename _Tp>
		struct IsScalar
				: public Or<IsArithmetic<_Tp>, IsEnum<_Tp>, IsPointer<_Tp>,
				  IsMemberPointer<_Tp>, is_null_pointer<_Tp>>::type
		{ };
}
}
