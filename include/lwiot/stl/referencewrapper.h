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

			constexpr ReferenceWrapper() noexcept : _value(nullptr)
			{ }

			explicit constexpr ReferenceWrapper(type& value) noexcept : _value(&value)
			{ }

			constexpr ReferenceWrapper(const ReferenceWrapper& other) noexcept : _value(other._value)
			{ }

			constexpr ReferenceWrapper(ReferenceWrapper&& other) noexcept : _value(other._value)
			{
				other._value = nullptr;
			}

			constexpr ReferenceWrapper& operator=(type& other)
			{
				this->_value = &other;
				return *this;
			}

			constexpr ReferenceWrapper& operator=(const ReferenceWrapper& other)
			{
				this->_value = other._value;
				return *this;
			}

			constexpr ReferenceWrapper& operator=(ReferenceWrapper&& other) noexcept
			{
				this->_value = other._value;
				other._value = nullptr;

				return *this;
			}

			constexpr operator bool() const
			{
				return this->_value != nullptr;
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

			constexpr type& operator*()
			{
				return *this->_value;
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
