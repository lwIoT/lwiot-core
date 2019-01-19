/*
 * SpiBus / SpiMessage unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/test.h>
#include <lwiot/io/spimessage.h>
#include <lwiot/io/spibus.h>
#include <lwiot/types.h>

#include <stack>

static void reverse(const uint8_t *tx, uint8_t *rx, size_t length)
{
	std::stack<uint8_t> data;

	for(auto idx = 0UL; idx < length; idx++) {
		data.push(tx[idx]);
	}

	auto size = data.size();
	for(auto idx = 0U; idx < size; idx++) {
		rx[idx] = data.top();
		data.pop();
	}
}

class TestSpiBus : public lwiot::SpiBus {
public:
	explicit TestSpiBus(int miso, int mosi, int clk) : lwiot::SpiBus(miso, mosi, clk)
	{
	}

	virtual bool transfer(const uint8_t *tx, uint8_t *rx, size_t length)
	{
		reverse(tx, rx, length);
		return true;
	}
	using SpiBus::transfer;

	virtual void setFrequency(uint32_t frequency) override
	{
	}
};

int main(int argc, char **argv)
{
	unsigned char data[4];

	lwiot_init();

	lwiot::Logger log;
	lwiot::SpiMessage msg(5);
	TestSpiBus bus(1,2,3);

	log << "Running SPI test.." << lwiot::Logger::newline;

	msg << 0xCA;
	msg << 0xCB;

	data[0] = 0xAA;
	data[1] = 0xBA;
	data[2] = 'a';
	data[3] = 0;

	msg << data;

	bus.transfer(msg);
	for(auto byte : msg.rxdata()) {
		msg >> byte;
		print_dbg("Data: 0x%X\n", byte);
	}

	wait_close();
	lwiot_destroy();
	return -EXIT_SUCCESS;
}
