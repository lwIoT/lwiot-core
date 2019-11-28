/*
 * GUID generation.
 *
 * @author Michel Megens
 * @email dev@bietje.net
 */

/// @file guid.h

#pragma once

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/log.h>

namespace lwiot
{
	/**
	 * @brief GUID wrapper.
	 * @ingroup util
	 */
	class Guid {
	public:
		/**
		 * @brief Construct a new guid.
		 */
		explicit Guid();

		/**
		 * @brief Copy construct a guid.
		 * @param other Guid to copy.
		 */
		Guid(const Guid& other);

		/**
		 * @brief Move construct a guid.
		 * @param other Guid to move.
		 */
		Guid(Guid&& other) noexcept;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs Object to copy.
		 */
		Guid& operator=(const Guid& rhs);

		/**
		 * @brief Move assignment operator.
		 * @param rhs Object to move.
		 */
		Guid& operator=(Guid&& rhs) noexcept;

		/**
		 * @brief Equality operator.
		 * @param other Object to check for equality.
		 * @return True or false based on success.
		 */
		bool operator==(const Guid& other);

		/**
		 * @brief Equality operator.
		 * @param str Object to check for equality.
		 * @return True or false based on success.
		 */
		bool operator==(const String& str);

		/**
		 * @brief Equality operator.
		 * @param other Object to check for equality.
		 * @return True or false based on success.
		 */
		bool operator!=(const Guid& other);

		/**
		 * @brief Equality operator.
		 * @param other Object to check for equality.
		 * @return True or false based on success.
		 */
		bool operator!=(const String& other);

		/**
		 * @brief Convert to String.
		 * @return String representation of \p *this.
		 */
		String toString() const;

		/**
		 * @brief Byte size of GUIDs.
		 */
		static constexpr int GUID_SIZE = 16;

	private:
		uint8_t _bytes[GUID_SIZE];

		/* Methods */
		uint8_t random() const;
		uint8_t randomBit() const;
		uint8_t improveRandomBit() const;
	};
}
