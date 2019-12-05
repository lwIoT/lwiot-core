/*
 * 23K256 SRAM chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sram23k256.h SRAM device driver.

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/scopedlock.h>
#include <lwiot/bytebuffer.h>

#include <lwiot/stl/referencewrapper.h>

#include <lwiot/io/spimessage.h>
#include <lwiot/io/spibus.h>

#include <lwiot/device/sram23k256.h>

namespace lwiot
{
	/**
	 * @brief SRAM device driver.
	 * @ingroup device
	 */
	class SRAM23K256 {
	public:
		/**
		 * @brief Construct a new 23K256 object.
		 * @param cs Chip select pin.
		 */
		explicit SRAM23K256(int cs = 1);

		/**
		 * @brief Construct a new 23K256 object.
		 * @param bus The SPI bus.
		 * @param cs Chip select pin.
		 */
		explicit SRAM23K256(SpiBus& bus, int cs = 1);

		/**
		 * @brief Start the 23K256 driver.
		 */
		void begin();

		/**
		 * @brief Start the 23K256 driver.
		 * @param bus SPI bus.
		 * @param cs Chip select pin.
		 */
		void begin(SpiBus& bus, int cs = 1);

		/**
		 * @brief Read from the SRAM chip.
		 * @param addr Address to start reading.
		 * @param length Number of bytes to read.
		 * @return Buffer object with read data.
		 */
		ByteBuffer read(size_t addr, size_t length);

		/**
		 * @brief Write data to the SRAM chip.
		 * @param addr Address to write to.
		 * @param buffer Data to write.
		 */
		void write(size_t addr, const ByteBuffer& buffer);

	private:
		stl::ReferenceWrapper<SpiBus> _bus;
		GpioPin _cs;

		/* Methods */
		void mode(uint16_t mode);
	};
}
