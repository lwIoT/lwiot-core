/*
 * ByteBuffer unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/bytebuffer.h>
#include <lwiot/test.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>

class TestBuffer : public lwiot::ByteBuffer {
public:
	TestBuffer(int num) : ByteBuffer(), _x(num)
	{ }

	TestBuffer(const TestBuffer& buffer) : ByteBuffer(buffer)
	{ }

	TestBuffer& operator=(TestBuffer&& rhs)
	{
		this->move(rhs);
		this->_x = rhs._x;

		return *this;
	}

	constexpr int getNum() const
	{
		return this->_x;
	}

private:
	int _x;
};

int main(int argc, char **argv)
{
	TestBuffer bf(5);

	lwiot_init();
	for(int i = 65; i < 75; i++)
		bf.write((uint8_t)i);

	print_dbg("Bytes in buffer: ");
	for(auto x : bf) {
		printf("%c ", x);
	}

	putc('\n', stdout);
	print_dbg("Bytes in second buffer: ");
	TestBuffer bf2(bf);

	assert(bf == bf2);
	bf2.write(77);
	bf2.write(78);
	assert(bf != bf2);

	bf = lwiot::stl::move(bf2);

	auto lambda = [](uint8_t& byte) -> void {
		printf("%c ", byte);
	};

	bf.foreach(lambda);

	putc('\n', stdout);
	lwiot_destroy();

	wait_close();
	return -EXIT_SUCCESS;
}

