#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "FixedGrid.hpp"

int main() {

	using namespace alib;

	constexpr Point pos;

	std::cout << pos << std::endl;
	std::cout << pos.hash() << std::endl;
	std::cout << Point::manhattanLength(pos, pos) << std::endl;

	constexpr FixedGrid<int, 3, 4> grid;
	static_assert(std::is_integral_v<decltype(grid)::value_type>);
	constexpr int a = grid[1];
	constexpr int b = grid.at(1);
	std::cout << a << "," << b << std::endl;

	return 0;
}
