#include <iostream>
#include "Stopwatch.hpp"
#include "Timer.hpp"

using namespace std;

int main() {

	std::cout << "Hello World!" << std::endl;

	Stopwatch sw;
	Timer timer(1000ms);

	sw.start();
	timer.start();

	while (!timer) {

	}

	sw.stop();

	std::cout << sw.hours() << "h" << std::endl;
	std::cout << sw.minutes() << "m" << std::endl;
	std::cout << sw.second() << "s" << std::endl;
	std::cout << sw.millisecond() << "ms" << std::endl;
	std::cout << sw.microseconds() << "us" << std::endl;
	std::cout << sw.nanoseconds() << "ns" << std::endl;

	return 0;
}
