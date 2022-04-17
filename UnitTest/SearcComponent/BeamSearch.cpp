#include "pch.h"
#include "../pch.h"

#include "../../CppLib/BeamSearch.hpp"

namespace SearcComponent::BeamSearchTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_BeamSearch, BeamSearch_FuncTest01) {

		struct Arg {
			Arg() {}
			Arg(const int v) : value(v) {}
			int value{};
		};

		using Method = alib::Search::DefaultSearchMethod<Arg, double, size_t>;
		//using Method = alib::Search::DefaultSearchMethod<Arg>;
		using Config = alib::Search::ConfigTemplate<3, 10, 2000 * 1000, Method>;

		class Beam : public alib::Search::BeamSearchTemplate<Config, Beam> {
		public:
			using base = alib::Search::BeamSearchTemplate<Config, Beam>;
			using SearchMethod = base::SearchMethod;
			using Reference = alib::Search::MemoType<Beam>;

		private:

			Reference::Array<Reference::Value<int>, 3> list;

		public:

			void init(const SearchMethod&) noexcept override {
				forange_type(int, i, 10) {
					nextSearch(i, i, i);
					//nextSearch(i, i);
				}
			}

			void search(const SearchMethod& arg) noexcept override {
				const auto depth = getDepth();
				list[depth] = arg.argument.value;
				forange_type(int, i, 10) {
					nextSearch(arg.score + i, arg.hash * 10 + i, i);
					//nextSearch(arg.score + i, i);
				}
				int a = 0;
			}

		};

		Beam beam;
		beam.start({});
		//beam.start({ 0, Arg() });
		const auto result = beam.getResultList();

		ASSERT_EQ(9, result[0].value);
		ASSERT_EQ(9, result[1].value);
		ASSERT_EQ(9, result[2].value);
	}

}
