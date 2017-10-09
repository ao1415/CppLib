#include <iostream>
#include "FixedGrid.hpp"
#include "Stopwatch.hpp"

using namespace std;

int main() {

	alib::FixedGrid<int, 3, 4> grid(3);
	grid.clear();

	for (int y = 0; y < grid.height(); y++)
	{
		for (int x = 0; x < grid.width(); x++)
		{
			cout << grid[y][x]<<",";
		}
		cout << endl;
	}

	return 0;
}
