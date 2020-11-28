#include <iostream>
#include <numeric>
#include <queue>

#include "Stopwatch.hpp"

#include "FiexdPriorityQueue.hpp"

using namespace std;

int main() {

	using namespace alib;

	Stopwatch sw;
	FiexdPriorityQueue<int, 2 << 16> que;

	sw.start();

	que.push(100000);
	forange(i, 100000)
	{
		que.push(i);
	}

	forange(i, 100000)
	{
		que.pop();
	}
	
	cout << que.top() << endl;


	sw.stop();

	cout << sw.toString_ms() << endl;

	return 0;
}
