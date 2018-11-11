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

	MilliSecTimer msTimer(chrono::milliseconds(3000));
	Stopwatch sw;

	msTimer.start();

	sw.start();
	while (!msTimer) {
		const auto interval = msTimer.interval();

		if (interval == 1000)
		{
			sw.stop();
			cout << sw.toString_ms() << endl;
		}
		else if (interval == 2000)
		{
			sw.stop();
			cout << sw.toString_ms() << endl;
		}
	}
	sw.stop();

	cout << sw.toString_ms() << endl;

	return 0;
}
