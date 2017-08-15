#include <iostream>
#include "FixedStack.hpp"
#include "Stopwatch.hpp"

using namespace std;

int main() {

	alib::FixedStack<int, 16> stack1;

	stack1.push(1);
	stack1.push(2);
	stack1.push(3);
	stack1.push(4);
	stack1.push(5);

	alib::FixedStack<int, 16> stack2 = stack1;

	cout << "size:" << stack1.element_num() << endl;
	cout << "size:" << stack2.element_num() << endl;

	while (!stack1.empty())
	{
		cout << stack1.top() << endl;
		stack1.pop();
	}

	cout << "size:" << stack1.element_num() << endl;
	cout << "size:" << stack2.element_num() << endl;

	stack1.swap(stack2);

	cout << "size:" << stack1.element_num() << endl;
	cout << "size:" << stack2.element_num() << endl;

	while (!stack1.empty())
	{
		cout << stack1.top() << endl;
		stack1.pop();
	}

	cout << "size:" << stack1.element_num() << endl;
	cout << "size:" << stack2.element_num() << endl;

	return 0;
}
