#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "BeamSearch.hpp"

using namespace std;

struct Argument {
	Argument() noexcept {}
};

using BeamConfig = alib::BeamSearchConfig<10, 10, 2900, Argument>;

class Beam : public BeamConfig::BeamBase {
public:

	void search(SearchArgument search) override {
		nextSearch(search.score, search.argument);
	}
};

int main() {

	using namespace alib;

	Beam beam;

	beam.start(0);

	return 0;
}
