/*
 * XBee address wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>

namespace lwiot
{
	/**
	 * @brief XBee address wrapper base class.
	 */
	class XBeeAddress {
	public:
		constexpr XBeeAddress() = default;;
	};

	/**
	 * @brief Represents a 64-bit XBee Address
	 *
	 * Note that avr-gcc as of 4.9 doesn't optimize uint64_t very well, so
	 * for the smallest and fastest code, use msb and lsb separately. See
	 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66511
	 */
	class XBeeAddress64 : public XBeeAddress {
	public:
		/**
		 * @brief Construct a new XBee address.
		 * @param addr 64-bit initial address.
		 */
		constexpr XBeeAddress64(uint64_t addr) : _msb(addr >> 32), _lsb(addr)
		{
		}

		/**
		 * @brief Construct a new XBee address.
		 * @param msb Most significant byte.
		 * @param lsb Least significant byte.
		 */
		constexpr XBeeAddress64(uint32_t msb, uint32_t lsb) : _msb(msb), _lsb(lsb)
		{
		}

		/**
		 * @brief Construct a new XBee address.
		 */
		constexpr XBeeAddress64() : _msb(0), _lsb(0)
		{
		}

		/**
		 * @brief Get the MSB.
		 * @return The MSB.
		 */
		uint32_t getMsb()
		{
			return this->_msb;
		}

		/**
		 * @brief Get the LSB.
		 * @return The LSB.
		 */
		uint32_t getLsb()
		{
			return this->_lsb;
		}

		/**
		 * @brief Get the interger representation of \p *this.
		 * @return 64-bit address.
		 */
		uint64_t get()
		{
			return (static_cast<uint64_t>(this->_msb) << 32) | this->_lsb;
		}

		operator uint64_t()
		{
			return get();
		}

		/**
		 * @brief Set the MSB.
		 * @param msb MSB to set.
		 */
		void setMsb(uint32_t msb)
		{
			this->_msb = msb;
		}

		/**
		 * @brief Set the LSB.
		 * @param lsb LSB to set.
		 */
		void setLsb(uint32_t lsb)
		{
			this->_lsb = lsb;
		}

		/**
		 * @brief Set the address.
		 * @param addr Address byte.
		 */
		void set(uint64_t addr)
		{
			this->_msb = addr >> 32;
			this->_lsb = addr;
		}

	private:
		// Once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66511 is
		// fixed, it might make sense to merge these into a uint64_t.
		uint32_t _msb;
		uint32_t _lsb;
	};

}
