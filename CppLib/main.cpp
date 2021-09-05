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

	FixedGrid<int, 3, 4> grid(1);
	static_assert(std::is_integral_v<decltype(grid)::value_type>);
	int a = grid[1];
	int b = grid.at(1);

	return 0;
}
