/*
 * GUID generation.
 *
 * @author Michel Megens
 * @email dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/log.h>
#include <lwiot/util/guid.h>

namespace lwiot
{
	Guid::Guid()
	{
		for(int idx = 0; idx < Guid::GUID_SIZE; idx++) {
			this->_bytes[idx] = this->random();
		}
	}

	Guid::Guid(const lwiot::Guid &other)
	{
		memcpy(this->_bytes, other._bytes, Guid::GUID_SIZE);
	}

	Guid::Guid(lwiot::Guid &&other) noexcept
	{
		memcpy(this->_bytes, other._bytes, Guid::GUID_SIZE);
	}

	Guid &Guid::operator=(lwiot::Guid &&rhs) noexcept
	{
		memcpy(this->_bytes, rhs._bytes, Guid::GUID_SIZE);
		return *this;
	}

	Guid &Guid::operator=(const lwiot::Guid &rhs)
	{
		memcpy(this->_bytes, rhs._bytes, Guid::GUID_SIZE);
		return *this;
	}

	bool Guid::operator==(const lwiot::Guid &other)
	{
		return memcmp(this->_bytes, other._bytes, Guid::GUID_SIZE) == 0;
	}

	bool Guid::operator==(const lwiot::String &str)
	{
		return this->toString() == str;
	}

	bool Guid::operator!=(const lwiot::Guid &other)
	{
		return !(*this == other);
	}

	bool Guid::operator!=(const lwiot::String &other)
	{
		return this->toString() != other;
	}

	uint8_t Guid::randomBit() const
	{
		return rand() & 0x1;
	}

	uint8_t Guid::improveRandomBit() const
	{
		int x;

		while(true) {
			x = this->randomBit();
			x |= this->randomBit() << 1;

			if(x == 1)
				return 0;

			if(x == 2)
				return 1;
		}
	}

	uint8_t Guid::random() const
	{
		uint8_t result;

		result = 0;
		for(uint8_t idx = 8; idx--;) {
			result += result + randomBit();
		}

		return result;
	}

	String Guid::toString() const
	{
		String string = "";
		const char *hex = "0123456789abcdef";

		for(int i = 0; i < Guid::GUID_SIZE; i++) {
			if(i == 4)
				string += "-";
			if(i == 6)
				string += "-";
			if(i == 8)
				string += "-";
			if(i == 10)
				string += "-";

			auto top = this->_bytes[i] >> 4;
			auto bottom = this->_bytes[i] & 0x0f;

			string += hex[top];
			string += hex[bottom];
		}

		return string;
	}
}
