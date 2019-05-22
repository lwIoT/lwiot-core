/*
 * 23K256 SRAM chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
	class SRAM23K256 {
	public:
		explicit SRAM23K256(int cs = 1);
		explicit SRAM23K256(SpiBus& bus, int cs = 1);

		void begin();
		void begin(SpiBus& bus, int cs = 1);

		ByteBuffer read(size_t addr, size_t length);
		void write(size_t addr, const ByteBuffer& buffer);

	private:
		stl::ReferenceWrapper<SpiBus> _bus;
		GpioPin _cs;

		/* Methods */
		void mode(uint16_t mode);
	};
}
