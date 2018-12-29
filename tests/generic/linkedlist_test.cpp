/*
 * Linked list unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/string.h>
#include <lwiot/test.h>
#include <lwiot/linkedlist.h>

#include <lwiot/stl/move.h>

struct Token {
	Token(int& x, double y) : x(x), y(y)
	{
	}

	int& x;
	double y;
};

int main(int argc, char **argv)
{
	int a, b, c;

	lwiot_init();
	a = 4;
	b = 5;
	c = 6;

	Token t1(a, 1.234);
	Token t2(b, 1.234);
	Token t3(c, 1.234);

	lwiot::LinkedList<Token> list;
	list.push_back(t1);
	list.push_back(t2);
	list.push_back(t3);

	lwiot::LinkedList<Token> list2(list);

	for(const Token& token : list2) {
		print_dbg("List enry value: [%i][%f]\n", token.x, token.y);
	}

	list.clear();
	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
