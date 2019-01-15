/*
 * GUID generation.
 *
 * @author Michel Megens
 * @email dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/log.h>

namespace lwiot
{
	class Guid {
	public:
		explicit Guid();
		Guid(const Guid& other);
		explicit Guid(Guid&& other) noexcept;

		Guid& operator=(const Guid& rhs);
		Guid& operator=(Guid&& rhs) noexcept;

		bool operator==(const Guid& other);
		bool operator==(const String& str);
		bool operator!=(const Guid& other);
		bool operator!=(const String& other);

		String toString() const;

		static constexpr int GUID_SIZE = 16;

	private:
		uint8_t _bytes[GUID_SIZE];

		/* Methods */
		uint8_t random() const;
		uint8_t randomBit() const;
		uint8_t improveRandomBit() const;
	};
}
