#include <iostream>
#include <numeric>
#include <queue>

#include "BeamSearch.hpp"

class Beam : public alib::Search::SearchTemplate<alib::Search::DebugProcess> {
private:
	using base = alib::Search::SearchTemplate<alib::Search::DebugProcess>;
	using ArgumentType = base::ArgumentType;
public:

	void init(const ArgumentType& arg) {

	}

	void search(const ArgumentType& arg) {

	}

};

int main() {

	using namespace alib;

	Beam beam;
	beam.start({});

	return 0;
}
