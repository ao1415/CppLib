#include <iostream>
#include "Timer.hpp"
#include "Stopwatch.hpp"
#include "FixedDeque.hpp"
#include "FixedGrid.hpp"
#include "FixedQueue.hpp"
#include "FixedStack.hpp"

using namespace std;

int main() {

	using namespace alib;

	int arr1[1000] = { 0 };
	array<int, 1000> arr2;
	arr2.fill(0);

	Stopwatch sw;

	int count = 0;

	count = 0;
	sw.start();
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			for (int k = 0; k < 1000; k++)
			{
				arr1[k] += 1;
			}
			for (int k = 0; k < 1000; k++)
			{
				count += arr1[k];
			}
		}
	}
	sw.stop();

	cout << sw.toString_ms() << endl;

	count = 0;
	sw.start();
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			for (int k = 0; k < 1000; k++)
			{
				arr2[k] += 1;
			}
			for (int k = 0; k < 1000; k++)
			{
				count += arr2[k];
			}
		}
	}
	sw.stop();

	cout << sw.toString_ms() << endl;

	return 0;
}
