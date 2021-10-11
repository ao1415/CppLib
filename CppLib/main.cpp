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

using BeamConfig = alib::BeamSearchConfig<10, 20, 2900, Argument>;

using Table = alib::FixedGrid<int, 10, 10>;

class Beam : public BeamConfig::BeamBase {
public:

	const Table& table;
	std::pair<double, std::vector<int>> best{ 0,{} };

	alib::VersionRef::Vector<int, 10> arr;
	alib::VersionRef::Value<int> sum = 0;

	Beam(const Table& table) noexcept : table(table) {}

	void init(const SearchArgument& search) override {

		forange(i, table.height()) {
			double score = table.at(0, i);
			nextSearch(score, 0, alib::narrow_cast<int>(i));
		}

	}

	void search(const SearchArgument& search) override {

		arr.push_back(table.at(search.argument.turn, search.argument.num));
		sum += arr.back();

		std::cout << getDepth() << ":" << sum.value() << ":";
		for (const auto& v : arr) {
			std::cout << v << ",";
		}
		std::cout << std::endl;

		if (best.first < search.score) {
			best.first = search.score;
			best.second.assign(arr.begin(), arr.end());
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
