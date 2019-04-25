/*
 * Reference Wrapper header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <assert.h>
#include <lwiot.h>

namespace lwiot
{
	namespace stl
	{

		template <typename T>
		class ReferenceWrapper {
		public:
			typedef T type;

			CONSTEXPR ReferenceWrapper() noexcept : _value(nullptr)
			{ }

			CONSTEXPR ReferenceWrapper(type& value) noexcept : _value(&value)
			{ }

			CONSTEXPR ReferenceWrapper(const ReferenceWrapper& other) noexcept : _value(other._value)
			{ }

			CONSTEXPR ReferenceWrapper& operator=(const ReferenceWrapper& other)
			{
				this->_value = other._value;
				return *this;
			}

			constexpr type& get() const noexcept
			{
				assert(this->_value != nullptr);
				return *this->_value;
			}

			constexpr operator type&() const noexcept
			{
				assert(this->_value != nullptr);
				return this->get();
			}

			constexpr type* operator->() const noexcept
			{
				assert(this->_value != nullptr);
				return this->_value;
			}

		private:
			type* _value;
		};
	}
}
