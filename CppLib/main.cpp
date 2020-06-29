#include <iostream>
#include "Timer.hpp"
#include "Stopwatch.hpp"
#include "FixedDeque.hpp"
#include "FixedGrid.hpp"
#include "FixedQueue.hpp"
#include "FixedStack.hpp"
#include "MemoryPool.hpp"

using namespace std;

class A {
public:

	A() {
		cout << "new:" << x << endl;
	}

	A(const int _x) {
		x = _x;
		cout << "new:" << x << endl;
	}

	~A() {
		cout << "delete:" << x << endl;
	}

	void show() {
		cout << "show:" << x << endl;
	}

private:

	int x = 0;

};

int main() {

	using namespace alib;

	Stopwatch sw;

	sw.start();

	MemoryPool<A, 5> pool;

	cout << "-" << endl;

	A* a0;
	A* a1;
	A* a2;
	A* a3;
	A* a4;

	a0 = pool.push();
	a1 = pool.push(2);

	pool.pop();
	pool.pop();

	a0 = pool.push(0);
	a1 = pool.push(1);
	a2 = pool.push(2);
	a3 = pool.push(3);
	a4 = pool.push(4);

	a0->show();
	a1->show();
	a2->show();
	a3->show();
	a4->show();

	pool.clear();

	sw.stop();

	cout << sw.toString_ms() << endl;

	return 0;
}
