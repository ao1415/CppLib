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

using BeamConfig = alib::BeamSearchConfig<1600, 200, 1900, Argument>;

using Table = std::array<int, 1600>;

class Beam : public BeamConfig::BeamBase {
public:

	const Table& table;
	std::pair<double, std::vector<int>> best{ 0,{} };

	alib::VersionRef::Vector<int, 1600> arr;
	alib::VersionRef::Value<int> sum = 0;

	Beam(const Table& table) noexcept : table(table) {}

	void init(const SearchArgument& search) override {

		forange(i, table.size()) {
			double score = table[i];
			nextSearch(score, 0, alib::narrow_cast<int>(i));
		}

	}

	void search(const SearchArgument& search) override {

		arr.push_back(table[search.argument.num]);
		sum() += arr.back();

		if (best.first < search.score) {
			best.first = search.score;
			best.second.assign(arr.begin(), arr.end());
		}

		if (endOfSearch()) return;

		forange(i, table.size()) {
			double score = search.score + table[i];
			nextSearch(score, search.argument.turn + 1, alib::narrow_cast<int>(i));
		}

	}
};

int main() {

	using namespace alib;

	Table table{};

	forange(i, table.size()) {
		table[i] = alib::narrow_cast<int>(i);
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
