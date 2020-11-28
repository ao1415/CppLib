#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "Stopwatch.hpp"

#include "GeometricSequence.hpp"

using namespace std;

int main() {

	using namespace alib;

	GeometricSequence<10> geo(0.9);

	double a = geo[4];
	cout << a << endl;

	forange(i, geo.size())
	{
		cout << geo[i] << endl;
	}


	return 0;
}
