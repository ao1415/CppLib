#include "pch.h"
#include "../CppLib/FixedGrid.hpp"

namespace FixedGrid {
	/**
	 * @brief 基本コンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest01) {

		alib::FixedGrid<int, 10, 10> grid1;
		alib::FixedGrid<int, 10, 10> grid2 = alib::FixedGrid<int, 10, 10>();
		alib::FixedGrid<int, 10, 10> grid3{};

		ASSERT_TRUE(true);
	}
	/**
	 * @brief 基本コンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest02) {

		constexpr alib::FixedGrid<int, 10, 10> grid1;
		constexpr alib::FixedGrid<int, 10, 10> grid2 = alib::FixedGrid<int, 10, 10>();
		constexpr alib::FixedGrid<int, 10, 10> grid3{};

		ASSERT_TRUE(true);
	}

	/**
	 * @brief コピーコンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest03) {

		alib::FixedGrid<int, 10, 10> copy1;
		forange(i, copy1.size()) copy1[i] = alib::narrow_cast<int>(i);
		alib::FixedGrid<int, 10, 10> grid1(copy1);

		ASSERT_EQ(copy1, grid1);
		forange(i, copy1.size()) ASSERT_EQ(copy1[i], grid1[i]);
	}
	/**
	 * @brief コピーコンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest04) {

		constexpr alib::FixedGrid<int, 10, 10> copy1;
		constexpr alib::FixedGrid<int, 10, 10> grid1(copy1);

		ASSERT_EQ(copy1, grid1);
		forange(i, copy1.size()) ASSERT_EQ(copy1[i], grid1[i]);
	}

	/**
	 * @brief ムーブコンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest05) {

		alib::FixedGrid<int, 10, 10> copy1;
		forange(i, copy1.size()) copy1[i] = alib::narrow_cast<int>(i);
		alib::FixedGrid<int, 10, 10> copy2(copy1);

		alib::FixedGrid<int, 10, 10> grid1(std::move(copy1));

		ASSERT_EQ(copy2, grid1);
		forange(i, copy2.size()) ASSERT_EQ(copy2[i], grid1[i]);
	}
	/**
	 * @brief ムーブコンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest06) {

		constexpr alib::FixedGrid<int, 10, 10> copy1;
		constexpr alib::FixedGrid<int, 10, 10> grid1(std::move(copy1));

		ASSERT_EQ(copy1, grid1);
		forange(i, copy1.size()) ASSERT_EQ(copy1[i], grid1[i]);
	}

	/**
	 * @brief 初期化コンストラクタ
	*/
	TEST(Basic, FixedGrid_ConstructTest07) {

		alib::FixedGrid<int, 10, 10> grid(1);

		forange(i, grid.size()) ASSERT_EQ(1, grid[i]);
	}

	/**
	 * @brief コピー
	*/
	TEST(Basic, FixedGrid_CopyTest01) {

		alib::FixedGrid<int, 10, 10> copy1;
		forange(i, copy1.size()) copy1[i] = alib::narrow_cast<int>(i);
		alib::FixedGrid<int, 10, 10> grid1;
		grid1 = copy1;

		ASSERT_EQ(copy1, grid1);
		forange(i, copy1.size()) ASSERT_EQ(copy1[i], grid1[i]);
	}
	/**
	 * @brief ムーブ
	*/
	TEST(Basic, FixedGrid_CopyTest02) {

		alib::FixedGrid<int, 10, 10> copy1;
		forange(i, copy1.size()) copy1[i] = alib::narrow_cast<int>(i);
		alib::FixedGrid<int, 10, 10> copy2(copy1);

		alib::FixedGrid<int, 10, 10> grid1;
		grid1 = std::move(copy1);

		ASSERT_EQ(copy2, grid1);
		forange(i, copy2.size()) ASSERT_EQ(copy2[i], grid1[i]);
	}

	/**
	 * @brief 参照テスト
	*/
	TEST(Basic, FixedGrid_AccessTest01) {

		alib::FixedGrid<int, 10, 10> grid;
		grid(0, 0) = 1;
		grid(0, 1) = 2;
		grid(8, 9) = 8;
		grid(9, 9) = 9;

		ASSERT_EQ(1, grid(0, 0));
		ASSERT_EQ(2, grid(0, 1));
		ASSERT_EQ(8, grid(8, 9));
		ASSERT_EQ(9, grid(9, 9));

		ASSERT_EQ(1, grid.at(0, 0));
		ASSERT_EQ(2, grid.at(0, 1));
		ASSERT_EQ(8, grid.at(8, 9));
		ASSERT_EQ(9, grid.at(9, 9));

		ASSERT_EQ(1, grid[0]);
		ASSERT_EQ(2, grid[10]);
		ASSERT_EQ(8, grid[98]);
		ASSERT_EQ(9, grid[99]);

		const alib::FixedGrid<int, 10, 10> cgrid(grid);
		ASSERT_EQ(1, cgrid(0, 0));
		ASSERT_EQ(2, cgrid(0, 1));
		ASSERT_EQ(8, cgrid(8, 9));
		ASSERT_EQ(9, cgrid(9, 9));

		ASSERT_EQ(1, cgrid.at(0, 0));
		ASSERT_EQ(2, cgrid.at(0, 1));
		ASSERT_EQ(8, cgrid.at(8, 9));
		ASSERT_EQ(9, cgrid.at(9, 9));

		ASSERT_EQ(1, cgrid[0]);
		ASSERT_EQ(2, cgrid[10]);
		ASSERT_EQ(8, cgrid[98]);
		ASSERT_EQ(9, cgrid[99]);
	}

	/**
	 * @brief 参照テスト
	*/
	TEST(Basic, FixedGrid_AccessTest02) {

		alib::FixedGrid<int, 10, 10> grid;

		try {
			auto _ = grid.at(-1, 0);
			ASSERT_TRUE(false);
		}
		catch (std::out_of_range e) {
			ASSERT_TRUE(true);
		}
		catch (std::exception e) {
			ASSERT_TRUE(false);
		}

		try {
			auto _ = grid.at(10, 9);
			ASSERT_TRUE(false);
		}
		catch (std::out_of_range e) {
			ASSERT_TRUE(true);
		}
		catch (std::exception e) {
			ASSERT_TRUE(false);
		}
	}
	/**
	 * @brief ユーティリティテスト
	*/
	TEST(Basic, FixedGrid_UtilityTest01) {

		alib::FixedGrid<int, 5, 10> grid;
		ASSERT_EQ(5, grid.width());
		ASSERT_EQ(10, grid.height());

		ASSERT_TRUE(grid.inside(0, 0));
		ASSERT_TRUE(grid.inside(4, 9));
		ASSERT_FALSE(grid.inside(-1, 0));
		ASSERT_FALSE(grid.inside(5, 9));
		ASSERT_FALSE(grid.inside(0, -1));
		ASSERT_FALSE(grid.inside(4, 10));

		ASSERT_FALSE(grid.outside(0, 0));
		ASSERT_FALSE(grid.outside(4, 9));
		ASSERT_TRUE(grid.outside(-1, 0));
		ASSERT_TRUE(grid.outside(5, 9));
		ASSERT_TRUE(grid.outside(0, -1));
		ASSERT_TRUE(grid.outside(4, 10));
	}
}

#include "../CppLib/BasicPoint.hpp"
namespace FixedGrid {
	/**
	 * @brief 参照テスト
	*/
	TEST(Basic, FixedGrid_AccessTest03) {

		alib::FixedGrid<int, 10, 10> grid;
		grid(0, 0) = 1;
		grid(0, 1) = 2;
		grid(8, 9) = 8;
		grid(9, 9) = 9;

		ASSERT_EQ(1, grid(alib::Point(0, 0)));
		ASSERT_EQ(2, grid(alib::Point(0, 1)));
		ASSERT_EQ(8, grid(alib::Point(8, 9)));
		ASSERT_EQ(9, grid(alib::Point(9, 9)));

		ASSERT_EQ(1, grid.at(alib::Point(0, 0)));
		ASSERT_EQ(2, grid.at(alib::Point(0, 1)));
		ASSERT_EQ(8, grid.at(alib::Point(8, 9)));
		ASSERT_EQ(9, grid.at(alib::Point(9, 9)));

		const alib::FixedGrid<int, 10, 10> cgrid(grid);
		ASSERT_EQ(1, cgrid(alib::Point(0, 0)));
		ASSERT_EQ(2, cgrid(alib::Point(0, 1)));
		ASSERT_EQ(8, cgrid(alib::Point(8, 9)));
		ASSERT_EQ(9, cgrid(alib::Point(9, 9)));

		ASSERT_EQ(1, cgrid.at(alib::Point(0, 0)));
		ASSERT_EQ(2, cgrid.at(alib::Point(0, 1)));
		ASSERT_EQ(8, cgrid.at(alib::Point(8, 9)));
		ASSERT_EQ(9, cgrid.at(alib::Point(9, 9)));
	}

	/**
	 * @brief 参照テスト
	*/
	TEST(Basic, FixedGrid_AccessTest04) {

		alib::FixedGrid<int, 10, 10> grid;

		try {
			auto _ = grid.at(alib::Point(-1, 0));
			ASSERT_TRUE(false);
		}
		catch (std::out_of_range e) {
			ASSERT_TRUE(true);
		}
		catch (std::exception e) {
			ASSERT_TRUE(false);
		}

		try {
			auto _ = grid.at(alib::Point(10, 9));
			ASSERT_TRUE(false);
		}
		catch (std::out_of_range e) {
			ASSERT_TRUE(true);
		}
		catch (std::exception e) {
			ASSERT_TRUE(false);
		}
	}
	/**
	 * @brief ユーティリティテスト
	*/
	TEST(Basic, FixedGrid_UtilityTest02) {

		alib::FixedGrid<int, 5, 10> grid;

		ASSERT_TRUE(grid.inside(alib::Point(0, 0)));
		ASSERT_TRUE(grid.inside(alib::Point(4, 9)));
		ASSERT_FALSE(grid.inside(alib::Point(-1, 0)));
		ASSERT_FALSE(grid.inside(alib::Point(5, 9)));
		ASSERT_FALSE(grid.inside(alib::Point(0, -1)));
		ASSERT_FALSE(grid.inside(alib::Point(4, 10)));

		ASSERT_FALSE(grid.outside(alib::Point(0, 0)));
		ASSERT_FALSE(grid.outside(alib::Point(4, 9)));
		ASSERT_TRUE(grid.outside(alib::Point(-1, 0)));
		ASSERT_TRUE(grid.outside(alib::Point(5, 9)));
		ASSERT_TRUE(grid.outside(alib::Point(0, -1)));
		ASSERT_TRUE(grid.outside(alib::Point(4, 10)));
	}
}
