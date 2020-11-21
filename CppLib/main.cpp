#include <iostream>
#include <numeric>

#include "Stopwatch.hpp"

#include "Random.hpp"

using namespace std;

int main() {

	using namespace alib;

	Stopwatch sw;

	XoShiro128 xoshiro;

	double minVal = numeric_limits<double>::max();
	double maxVal = numeric_limits<double>::min();
	for (int i = 0; i < 100000; i++)
	{
		const double n = xoshiro.nextDouble();
		minVal = min(minVal, n);
		maxVal = max(maxVal, n);
	}
	cout << minVal << endl;
	cout << maxVal << endl;

	sw.start();

	XoShiro128::value_type val = 0;
	for (int i = 0; i < 1000000; i++)
	{
		val ^= xoshiro.next();
	}

	sw.stop();

	cout << val << endl;

	cout << sw.toString_ms() << endl;

	sw.start();
	double val2 = 0;
	for (int i = 0; i < 1000000; i++)
	{
		val2 += xoshiro.nextDouble();
	}

	sw.stop();

	cout << val2 << endl;

	cout << sw.toString_ms() << endl;

	return 0;
}
