/*
 * Zigbee address implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

namespace lwiot
{
	class ZigbeeAddress {
	public:
		constexpr ZigbeeAddress() : _addr({0ULL}), _is64bit(true)
		{
		}

		constexpr void setAddress16(uint16_t addr)
		{
			this->_is64bit = false;
			this->_addr.addr16 = addr;
		}

		constexpr void setAddress64(uint64_t addr)
		{
			this->_addr.addr64 = addr;
			this->_is64bit = true;
		}

		constexpr uint16_t getAddress16() const
		{
			return this->_is64bit ? 0 : this->_addr.addr16;
		}

		constexpr uint64_t getAddress64() const
		{
			return this->_is64bit ? this->_addr.addr64 : 0ULL;
		}

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
