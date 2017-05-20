#include <iostream>
#include "Basic.hpp"
#include "Queue.hpp"
#include "FixedDeque.hpp"

using namespace std;

int main() {

	alib::FixedDeque<int, 16> deque;

	cout << "size:" << deque.size() << endl;
	cout << "element:" << deque.element_num() << endl;

	deque.push_back(1);
	deque.push_back(2);
	deque.push_back(3);

	deque.push_front(-1);
	deque.push_front(-2);
	deque.push_front(-3);
	
	cout << "element:" << deque.element_num() << endl;

	cout << deque.back() << endl;
	deque.pop_back();
	cout << deque.back() << endl;
	deque.pop_back();
	cout << deque.back() << endl;
	deque.pop_back();
	cout << deque.back() << endl;
	deque.pop_back();
	cout << deque.back() << endl;
	deque.pop_back();
	cout << deque.back() << endl;
	deque.pop_back();

	cout << "element:" << deque.element_num() << endl;

	deque.showArray();

	return 0;
}
