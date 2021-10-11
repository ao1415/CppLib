#include <iostream>
#include <numeric>
#include <queue>

#include "Basic.hpp"
#include "BasicPoint.hpp"
#include "FixedGrid.hpp"

#include "BeamSearch.hpp"

using namespace std;

struct Argument {
	Argument() noexcept {}
	Argument(int turn, int num) noexcept
		:turn(turn),
		num(num) {}

	int turn = 0;
	int num = 0;
};

using BeamConfig = alib::BeamSearchConfig<11, 20, 2900, Argument>;

using Table = alib::FixedGrid<int, 11, 10>;

class Beam : public BeamConfig::BeamBase {
public:

	const Table& table;
	std::pair<double, std::vector<int>> best{ 0,{} };

	alib::BeamSearch::RefVector<int, 11> sum;

	Beam(const Table& table) noexcept : table(table) {}

	void search(const SearchArgument& search) override {

		sum.push_back(table.at(search.argument.turn, search.argument.num));

		std::cout << getDepth() << ":" << search.score << ":";
		for (const auto& v : sum) {
			std::cout << v << ",";
		}
		std::cout << std::endl;

		if (best.first < search.score) {
			best.first = search.score;
			best.second.assign(sum.begin(), sum.end());
		}

		if (endOfSearch()) return;

		forange(i, table.height()) {

			double score = search.score +
				table.at(search.argument.turn + 1, i);

			nextSearch(score, search.argument.turn + 1, alib::narrow_cast<int>(i));
		}

	}
};

int main() {

	using namespace alib;

	Table table;

	forange(y, table.height()) {
		forange(x, table.width()) {
			table(x, y) = narrow_cast<int>(y);
		}
	}

	Beam beam(table);
	beam.start(BeamConfig::BeamBase::SearchArgument(0));

	std::cout << beam.best.first << std::endl;
	for (const auto& v : beam.best.second) {
		std::cout << v << ",";
	}
	std::cout << std::endl;

	return 0;
}
