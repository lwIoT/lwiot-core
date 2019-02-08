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
	explicit Token(const Token&) = default;
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
		return other.x == x;
	}

	lwiot::stl::ReferenceWrapper<int> x;
	double y;
	lwiot::String tst;
};

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

	list.clear();
	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
