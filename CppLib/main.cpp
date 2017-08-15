#include <iostream>
#include "FixedQueue.hpp"
#include "Stopwatch.hpp"

using namespace std;

int main() {

	alib::FixedQueue<int, 4> que1;
	alib::FixedQueue<int, 4> que2;

	cout << "size1:" << que1.size() << endl;
	cout << "size2:" << que2.size() << endl;

	que1.push(1);
	que1.push(2);
	que1.push(3);
	que1.push(4);
	que1.push(5);

	que2.push(1);
	que2.push(2);
	que2.push(3);

	cout << "size1:" << que1.element_num() << endl;
	cout << "size2:" << que2.element_num() << endl;

	cout << "que1" << endl;
	while (!que1.empty())
	{
		cout << que1.front() << endl;
		que1.pop();
	}

	cout << "que2" << endl;
	while (!que2.empty())
	{
		cout << que2.front() << endl;
		que2.pop();
	}

	return 0;
}
