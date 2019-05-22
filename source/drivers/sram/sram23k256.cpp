/*
 * 23K256 SRAM chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/scopedlock.h>
#include <lwiot/bytebuffer.h>

#include <lwiot/device/sram23k256.h>

#include <lwiot/stl/move.h>

#define WRSR 0x1
#define RDSR 0x5

#define RDDA 0x3
#define WRDA 0x2

#define HOLD 0x1

#define SRAM_SYNC 15

#define SPI_BYTE_MODE HOLD
#define SPI_PAGE_MODE HOLD | 0x80
#define SPI_SEQ_MODE  HOLD | 0x40


namespace lwiot
{
	SRAM23K256::SRAM23K256(int cs) : _cs(cs)
	{
	}

	SRAM23K256::SRAM23K256(lwiot::SpiBus &bus, int cs) : _bus(bus), _cs(cs)
	{
	}

	void SRAM23K256::mode(uint16_t mode)
	{
		SpiMessage msg(2, this->_cs);

		auto& buf = msg.txdata();
		buf[0] = WRSR;
		buf[1] = mode & 0xFF;

		if(!this->_bus->transfer(msg)) {
			print_dbg("Unable to set SPI mode!\n");
		}
	}

	void SRAM23K256::write(size_t addr, const lwiot::ByteBuffer &buffer)
	{
		SpiMessage msg(buffer.count() + 3, this->_cs);

		auto& tx = msg.txdata();
		tx.write(WRDA);
		tx.write((addr >> 8) & 0xFF);
		tx.write(addr & 0xFF);

		buffer.foreach([&tx](uint8_t value) {
			tx.write(value);
		});

		if(!this->_bus->transfer(msg)) {
			print_dbg("Unable to write data to SRAM chip!\n");
		}
	}

	ByteBuffer SRAM23K256::read(size_t addr, size_t length)
	{
		SpiMessage msg(length+3, this->_cs);
		ByteBuffer rv(length);

		auto& tx = msg.txdata();
		tx[0] = RDDA;
		tx[1] = (addr >> 8) & 0xFF;
		tx[2] = addr & 0xFF;

		if(!this->_bus->transfer(msg)) {
			print_dbg("Unable to read from SRAM chip!\n");
			return stl::move(rv);
		}

		auto data = msg.rxdata().data();
		rv.write(data + 3, length);
		return stl::move(rv);
	}

	void SRAM23K256::begin()
	{
		this->_cs.output();
		this->_cs.write(true);
		this->mode(SPI_SEQ_MODE);
	}

	void SRAM23K256::begin(lwiot::SpiBus &bus, int cs)
	{
		this->_cs = cs;
		this->_bus = bus;
		this->begin();
	}
}

