/*
 * Generic unit test for the DAC.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/dacchip.h>
#include <lwiot/test.h>

class TestDacChip : public lwiot::DacChip {
public:
	explicit TestDacChip() : DacChip(8, 8, 3300)
	{ }

	void begin() override
	{ }

	void write(const size_t& value) override
	{
		auto mapped = this->map(value);
		print_dbg("Writing DAC value [%u : %u]\n", value, mapped);
	}
};

int main(int argc, char **argv)
{
	TestDacChip testchip;

	lwiot_init();
	testchip.begin();
	testchip.write(3300);
	testchip.write(1000);
	testchip.write(2590);

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
