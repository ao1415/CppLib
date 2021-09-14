#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "FixedGrid.hpp"
#include "Stopwatch.hpp"

int main() {

	using namespace alib;

	constexpr Point pos;

	for (const auto& d : Point::Direction()) {
		std::cout << d << std::endl;
	}

	int count = 0;
	std::cin >> count;

	Stopwatch sw;
	sw.start();
	long long int c = 0;
	forange(i, count) {
		forange(i, count) {
			for (const auto& d : Point::Direction()) {
				c++;
			}
		}
	}
	sw.stop();
	std::cout << c << std::endl;
	std::cout << sw.toString_us() << std::endl;

	sw.start();
	c = 0;
	forange(i, count) {
		forange(i, count) {
			const auto direction = Point::Direction();
			for (const auto& d : direction) {
				c++;
			}
		}
	}
	sw.stop();
	std::cout << c << std::endl;
	std::cout << sw.toString_us() << std::endl;

	return 0;
}
