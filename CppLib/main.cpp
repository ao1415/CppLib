#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"

int main() {

	using namespace alib;

	constexpr Point pos;
	constexpr BasicPoint<unsigned char> posc(255, 127);

	std::cout << pos << std::endl;
	std::cout << pos.hash() << std::endl;
	std::cout << posc << std::endl;
	std::cout << posc.hash() << std::endl;

	return 0;
}
