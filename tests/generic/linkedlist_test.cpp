/*
 * Linked list unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/test.h>
#include <lwiot/stl/linkedlist.h>
#include <lwiot/stl/referencewrapper.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/foreach.h>

struct Token {
	Token(const Token&) = default;
	Token( Token&& token) = default;

	Token(int& x, double y, const char *str) : x(x), y(y), tst(str)
	{
	}

	Token& operator=(const Token&) = default;

	Token& operator=(Token&& rhs) noexcept
	{
		this->tst = lwiot::stl::move(rhs.tst);
		this->y = rhs.y;
		this->x = rhs.x;

		return *this;
	}

	bool operator==(const Token& other)
	{
		return other.x.get() == this->x.get();
	}

	lwiot::stl::ReferenceWrapper<int> x;
	double y;
	lwiot::String tst;
};

static bool should_assert = true;

class Entry {
public:
	explicit Entry(lwiot::stl::String data) : _data(lwiot::stl::move(data))
	{ }

	Entry(Entry&& other) noexcept
	{
		this->_data = lwiot::stl::move(other._data);
	}

	~Entry()
	{
		if(should_assert)
			assert(this->_data.length() == 0);
	}

	Entry& operator=(Entry&& rhs) noexcept
	{
		this->_data = lwiot::stl::move(rhs._data);
		return *this;
	}

	lwiot::stl::String _data;
};

static void append_test()
{
	lwiot::stl::LinkedList<Entry> ll;
	lwiot::stl::LinkedList<Entry> temp;
	Entry e1("Hello");
	Entry e2("World!");
	Entry e3("Test");
	Entry e4("Again");
	Entry e5("This");
	Entry e6("is");
	Entry e7("a");
	Entry e8("list!");

	ll.push_back(lwiot::stl::move(e1));
	ll.push_back(lwiot::stl::move(e2));
	ll.push_back(lwiot::stl::move(e3));
	ll.push_back(lwiot::stl::move(e4));

	temp.push_back(lwiot::stl::move(e5));
	temp.push_back(lwiot::stl::move(e6));
	temp.push_back(lwiot::stl::move(e7));
	temp.push_back(lwiot::stl::move(e8));


	lwiot::stl::foreach(temp, [&](lwiot::stl::LinkedList<Entry>::iterator& iter) {
		ll.push_back(lwiot::stl::move(*iter));
	});

	temp.clear();
	should_assert = false;

	assert(ll.back()._data == "list!");
}

int main(int argc, char **argv)
{
	int a, b, c, d;

	lwiot_init();
	a = 4;
	b = 5;
	c = 6;
	d = 7;

	Token t1(a, 1.234, "Hi");
	Token t2(b, 1.234, "Hello");
	Token t3(c, 1.234, "Bye");
	Token t4(d, 3.234, "Swap");

	lwiot::stl::LinkedList<Token> list;
	list.push_back(t1);
	list.push_back(t2);
	list.push_back(t3);

	lwiot::stl::LinkedList<Token> list2(list);

	lwiot::stl::foreach(list, [](lwiot::stl::LinkedList<Token>::iterator& iter) {
		auto& token = *iter;
		print_dbg("Entry value: [%i][%f]\n", token.x.get(), token.y);
	});

	print_dbg("First value: %f\n", list2.front().y);
	print_dbg("String length: %i\n", list2.front().tst.length());

	auto iter = list2.begin();
	++iter;
	list2.replace(iter, lwiot::stl::move(t4));

	print_dbg("After swap:\n");
	for(Token& token : list2) {
		lwiot::String str = lwiot::stl::move(token.tst);
		print_dbg("List entry value: [%i][%f]\n", token.x.get(), token.y);
	}

	list2.erase(iter);
	++iter;
	list2.erase(iter);

	print_dbg("\n");
	print_dbg("After erase:\n");
	for(const Token& token : list2) {
		print_dbg("List enry value: [%i][%f]\n", token.x.get(), token.y);
	}

	list2.push_back(t3);
	iter = list2.begin();
	list2.erase(iter);

	swap(list, list2);

	assert(list.front().x.get() == c);

	list.clear();

	lwiot::stl::foreach(list, [](auto& iter) {
		assert(false);
	});

	append_test();
	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
