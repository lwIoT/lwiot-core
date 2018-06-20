/*
 * Matplotlib C++ unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#undef _DEBUG
#include <matplotlibcpp.h>
#define _DEBUG 1

namespace plt = matplotlibcpp;

int main(int argc, char **argv)
{
	std::vector<double> x,y;
	srand(std::rand());

	for(int i = 0; i < 10; i++) {
		double num = std::rand() % 100;
		num /= 100;

		std::cout << num << std::endl;
		y.push_back(i % 2 == 0);
		x.push_back(i + num);
	}

	plt::step(x, y);
	plt::xlim(0, 10);
	plt::show();

	return -EXIT_SUCCESS;
}
