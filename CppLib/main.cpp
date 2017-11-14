#include <iostream>
#include "FixedGrid.hpp"
#include "Stopwatch.hpp"

using namespace std;

int main() {

	constexpr int size = 100;
	constexpr int c = 100000000;

	using Grid = std::array<std::array<int, size>, size>;

	Grid grid1;
	alib::Stopwatch sw;
	int count = 0;

	alib::FixedGrid<int, size, size> grid2;
	sw.start();
	for (int i = 0; i < c; i++)
	{
		grid1[size - 1][size - 1] = i;
		count += grid1[size - 1][size - 1];
		count %= 8192;
	}
	sw.stop();
	cout << count << endl;
	cout << sw.toString_us() << endl;

	sw.start();
	for (int i = 0; i < c; i++)
	{
		grid2[size - 1][size - 1] = i;
		count += grid2[size - 1][size - 1];
		count %= 8192;
	}
	sw.stop();
	cout << count << endl;
	cout << sw.toString_us() << endl;


	return 0;
}
