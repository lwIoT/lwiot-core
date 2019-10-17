/*
 * Future unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <lwiot/util/application.h>
#include <lwiot/functor.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/stl/hashmap.h>

struct emplace_test {
	emplace_test(int x, int y) : x(x), y(y)
	{
		print_dbg("Constructor called!\n");
	}

	emplace_test(const emplace_test& other) : x(other.x), y(other.y)
	{
		print_dbg("COPY constructor called!\n");
	}

	int x, y;
};

class Application : public lwiot::Functor {
private:
	void test_hash()
	{
		auto v1 = lwiot::stl::hash("Testing..");
		auto v2 = lwiot::stl::hash(1234);

		print_dbg("Value 1: %llu\n", v1);
		print_dbg("Value 2: %llu\n", v2);
	}

	void test_map()
	{
		lwiot::stl::HashMap<lwiot::String, int> map;

		map.emplace("aba", 1);
		map.emplace("abb", 1);
		map.emplace("abc", 1);
		map.emplace("abe", 1);
		map.emplace("abf", 1);
		map.emplace("abg", 1);
		map.emplace("abh", 1);
		map.emplace("abi", 1);
		map.emplace("abj", 1);
		map.emplace("abk", 1);
		map.emplace("abl", 1);
		map.emplace("abm", 1);
		map.emplace("abn", 1);
		map.emplace("abo", 1);
		map.emplace("abp", 1);
		map.emplace("abq", 1);
		map.emplace("abr", 1);
		map.emplace("abs", 1);
		map.emplace("abt", 1);
		map.emplace("abu", 1);

		map.dump();
	}

	void string_map_test()
	{
		lwiot::stl::HashMap<lwiot::String, int> map;

		map.emplace("ab", 4);
		map.insert(lwiot::stl::Pair<lwiot::String, int>("abc", 12));

		auto& value = map["ab"];
		assert(value == 4);
	}


protected:
	void run() override
	{
		print_dbg("Hashmap test started!\n");

		print_dbg("Testing hash..\n");
		this->test_hash();

		print_dbg("Testing map..\n");
		this->test_map();

		print_dbg("Testing string map..\n");
		this->string_map_test();
	}
} app;

int main(int argc, char **argv)
{
	lwiot::Application testapp(app);
	testapp.start();

	return -EXIT_SUCCESS;
}
