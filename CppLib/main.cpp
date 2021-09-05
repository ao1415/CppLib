#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"

int main() {

	using namespace alib;

	constexpr Point pos;

	std::cout << pos << std::endl;
	std::cout << pos.hash() << std::endl;
	std::cout << Point::manhattanLength(pos, pos) << std::endl;
	
	return 0;
}
