#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "BasicMemo.hpp"

int main() {

	using namespace alib;

	BufferMemoryStream buffer(32);

	struct Data {
		int a = 10;
		int b = 20;
	};

	Data data;
	std::cout << buffer.toString() << std::endl;
	buffer.push(data);
	std::cout << buffer.toString() << std::endl;

	forange(i, 10) {}

	const Point pos;
	std::cout << pos << std::endl;

	return 0;
}
