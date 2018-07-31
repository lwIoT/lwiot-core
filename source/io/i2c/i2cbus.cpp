/*
 * I2C serial bus wrapper.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/vector.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/i2cbus.h>

namespace lwiot
{
	I2CBus::I2CBus(I2CAlgorithm *algo) : _algo(*algo)
	{
	}

	I2CBus::I2CBus(I2CAlgorithm& algo) : _algo(algo)
	{
	}

	void I2CBus::setFrequency(const uint32_t& freq)
	{
		this->_algo.setFrequency(freq);
	}

	bool I2CBus::transfer(Vector<I2CMessage>& msgs)
	{
		return this->_algo.transfer(msgs) > 0;
	}

	bool I2CBus::transfer(I2CMessage& msg)
	{
		return this->_algo.transfer(msg) > 0;
	}

	void I2CBus::setAlgorithm(I2CAlgorithm& algo)
	{
		this->_algo = algo;
	}
}
