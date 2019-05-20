/*
 * SPI message implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/types.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/io/spimessage.h>
#include <lwiot/io/spibus.h>

namespace lwiot
{
	SpiBus::SpiBus(int mosi, int miso, int clk, uint32_t freq) :
		_miso(miso), _mosi(mosi), _clk(clk), _freq(freq)
	{
	}

	const uint32_t& SpiBus::frequency() const
	{
		return this->_freq;
	}

	const GpioPin& SpiBus::miso() const
	{
		return this->_miso;
	}

	const GpioPin& SpiBus::mosi() const
	{
		return this->_mosi;
	}

	const GpioPin& SpiBus::clk() const
	{
		return this->_clk;
	}

	void SpiBus::setFrequency(uint32_t freq)
	{
		this->_freq = freq;
	}

	bool SpiBus::transfer(stl::Vector<SpiMessage>& msgs)
	{
		for(SpiMessage& msg : msgs) {
			if(!this->transfer(msg))
				return false;
		}

		return true;
	}

	bool SpiBus::transfer(SpiMessage& msg)
	{
		const uint8_t *txbytes;
		uint8_t *rxbytes;

		txbytes = msg.txdata().data();
		rxbytes = msg.rxdata().data();
		auto retval = this->transfer(txbytes, rxbytes, msg.size());
		msg.rxdata().setIndex(msg.size());

		return retval;
	}
}
