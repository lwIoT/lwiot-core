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
	class XBeeAddress {
	public:
		constexpr XBeeAddress() {};
	};

/**
 * Represents a 64-bit XBee Address
 *
 * Note that avr-gcc as of 4.9 doesn't optimize uint64_t very well, so
 * for the smallest and fastest code, use msb and lsb separately. See
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66511
 */
	class XBeeAddress64 : public XBeeAddress {
	public:
		constexpr XBeeAddress64(uint64_t addr) : _msb(addr >> 32), _lsb(addr) {}
		constexpr XBeeAddress64(uint32_t msb, uint32_t lsb) : _msb(msb), _lsb(lsb) {}
		constexpr XBeeAddress64() : _msb(0), _lsb(0) {}
		uint32_t getMsb() {return _msb;}
		uint32_t getLsb() {return _lsb;}
		uint64_t get() {return (static_cast<uint64_t>(_msb) << 32) | _lsb;}
		operator uint64_t() {return get();}
		void setMsb(uint32_t msb) {_msb = msb;}
		void setLsb(uint32_t lsb) {_lsb = lsb;}
		void set(uint64_t addr) {
			_msb = addr >> 32;
			_lsb = addr;
		}
	private:
		// Once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66511 is
		// fixed, it might make sense to merge these into a uint64_t.
		uint32_t _msb;
		uint32_t _lsb;
	};

}
