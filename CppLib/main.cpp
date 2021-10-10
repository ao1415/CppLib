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
	Argument(int turn) noexcept
		:turn(turn) {}

	int turn = 0;
};

using BeamConfig = alib::BeamSearchConfig<10, 10, 2900, Argument>;

using Table = alib::FixedGrid<int, 10, 10>;

class Beam : public BeamConfig::BeamBase {
public:

	const Table& table;
	std::pair<double, std::vector<int>> best{ 0,{} };

	alib::BeamSearch::RefVector<int, 11> sum;

	Beam(const Table& table) noexcept : table(table) {}

	void search(const SearchArgument& search) override {

		sum.push_back(0);
		forange(i, table.height()) {
			sum.back() = table.at(i, search.argument.turn);
			double score = search.score + table.at(i, search.argument.turn);

			if (best.first < score) {
				best.first = score;
				best.second.assign(sum.begin(), sum.end());
			}

			std::cout << search.argument.turn << ":" << score << std::endl;
			for (const auto& v : sum) {
				std::cout << v << ",";
			}
			std::cout << std::endl;
			nextSearch(score, search.argument.turn + 1);
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
