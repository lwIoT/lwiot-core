/*
 * Zigbee address implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file zigbeeaddress.h

namespace lwiot
{
	/**
	 * @brief Zigbee address wrapper.
	 */
	class ZigbeeAddress {
	public:
		/**
		 * @brief Construct a zigbee address wrapper.
		 */
		constexpr ZigbeeAddress() : _addr({0ULL}), _is64bit(true)
		{
		}

		/**
		 * @brief Construct a zigbee address wrapper.
		 * @param addr Address to wrap.
		 */
		explicit ZigbeeAddress(uint16_t addr) : _addr({addr}), _is64bit(false)
		{ }

		/**
		 * @brief Set the address.
		 * @param addr Address to set.
		 */
		constexpr void setAddress16(uint16_t addr)
		{
			this->_is64bit = false;
			this->_addr.addr16 = addr;
		}

		/**
		 * @brief Set the address.
		 * @param addr Address to set.
		 */
		constexpr void setAddress64(uint64_t addr)
		{
			this->_addr.addr64 = addr;
			this->_is64bit = true;
		}

		/**
		 * @brief Get the 16-bit address.
		 * @return The 16-bit address value.
		 */
		constexpr uint16_t getAddress16() const
		{
			return this->_is64bit ? 0 : this->_addr.addr16;
		}

		/**
		 * @brief Get the 64-bit address.
		 * @return The 64-bit address value.
		 */
		constexpr uint64_t getAddress64() const
		{
			return this->_is64bit ? this->_addr.addr64 : 0ULL;
		}

		/**
		 * @brief Check if the address is 64-bit or not.
		 * @return 64-bit indicator flag.
		 */
		constexpr bool is64Bit() const
		{
			return this->_is64bit;
		}

	private:
		union {
			uint16_t addr16;
			uint64_t addr64;
		} _addr;

		bool _is64bit;
	};
}
