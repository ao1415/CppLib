#include <iostream>
#include "Basic.hpp"
#include "Queue.hpp"
#include "FixedDeque.hpp"

using namespace std;

int main() {

	alib::FixedDeque<int, 16> deque;
	alib::FixedDeque<int, 16> deque2;
	alib::FixedDeque<int, 8> deque3;

	cout << "size:" << deque.size() << endl;
	cout << "element:" << deque.element_num() << endl;

	deque.push_back(1);
	deque.push_back(2);
	deque.push_back(3);

	deque.push_front(-1);
	deque.push_front(-2);
	deque.push_front(-3);

	while (!deque.empty()) {
		cout << deque.front() << endl;
		deque.pop_front();
	}
	while (!deque2.empty()) {
		cout << deque2.front() << endl;
		deque2.pop_front();
	}

	return 0;
}
