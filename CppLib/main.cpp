#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "BeamSearch.hpp"

using namespace std;

struct Args {
	Args() noexcept {}
};

class Beam : public alib::BeamSearchTemplate<10, 10, 2900, Args> {
public:

	void search(ArgumentType args) override {
		throw exception();
	}
};

int main() {

	using namespace alib;

	Beam beam;

	beam.start(0);

	return 0;
}
