#include "pch.h"
#include "../CppLib/BasicPoint.hpp"

namespace BasicPoint {

	/**
	 * @brief コンストラクタ
	*/
	TEST(Basic, BasicPoint_ConstructTest01) {

		alib::Point pos1;
		constexpr alib::Point pos2;
		alib::Point pos3(1, 2);
		constexpr alib::Point pos4(1, 2);
		alib::Point pos5(pos3);
		constexpr alib::Point pos6(pos4);

		ASSERT_EQ(0, pos1.x); ASSERT_EQ(0, pos1.y);
		ASSERT_EQ(0, pos2.x); ASSERT_EQ(0, pos2.y);
		ASSERT_EQ(1, pos3.x); ASSERT_EQ(2, pos3.y);
		ASSERT_EQ(1, pos4.x); ASSERT_EQ(2, pos4.y);
		ASSERT_EQ(1, pos5.x); ASSERT_EQ(2, pos5.y);
		ASSERT_EQ(1, pos6.x); ASSERT_EQ(2, pos6.y);
	}

	/**
	 * @brief オペレータ
	*/
	TEST(Basic, BasicPoint_OperatorTest01) {

		alib::Point pos1(0, 0);
		alib::Point pos2(1, 10);

		pos1 = pos2;
		ASSERT_EQ(1, pos1.x); ASSERT_EQ(10, pos1.y);

		pos2 = alib::Point(10, 100);
		ASSERT_EQ(11, (pos1 + pos2).x); ASSERT_EQ(110, (pos1 + pos2).y);
		ASSERT_EQ(9, (pos2 - pos1).x); ASSERT_EQ(90, (pos2 - pos1).y);

		ASSERT_TRUE(pos1 < pos2);

		ASSERT_TRUE(pos1 == alib::Point(1, 10));
		ASSERT_FALSE(pos1 == alib::Point(0, 10));
		ASSERT_TRUE(pos1 != alib::Point(0, 10));
		ASSERT_FALSE(pos1 != alib::Point(1, 10));

		ASSERT_TRUE(pos1.equals(alib::Point(1, 10)));
		ASSERT_FALSE(pos1.equals(alib::Point(0, 10)));
		ASSERT_TRUE(pos1.equals(1, 10));
		ASSERT_FALSE(pos1.equals(0, 10));
	}

	/**
	 * @brief ユーティリティ
	*/
	TEST(Basic, BasicPoint_UtilityTest01) {

		alib::Point pos1(0, 0);
		alib::Point pos2(1, 10);

		ASSERT_TRUE(pos1.isZero());
		ASSERT_FALSE(pos2.isZero());

		pos2.clear();
		ASSERT_TRUE(pos2.isZero());

		pos1.set(1, 10);
		ASSERT_EQ(1, pos1.x);
		ASSERT_EQ(10, pos1.y);

		pos2.set(pos1);
		ASSERT_EQ(1, pos2.x);
		ASSERT_EQ(10, pos2.y);

		pos1.set(3, 4);
		pos2.set(6, 8);

		ASSERT_EQ(25, pos1.dot());

		ASSERT_EQ(25, pos1.dot(pos2));
		ASSERT_EQ(25, alib::Point::dot(pos1, pos2));

		ASSERT_EQ(5, pos1.length());

		ASSERT_EQ(5, pos1.length(pos2));
		ASSERT_EQ(5, alib::Point::length(pos1, pos2));

		ASSERT_EQ(7, pos1.manhattanLength());

		ASSERT_EQ(7, pos1.manhattanLength(pos2));
		ASSERT_EQ(7, alib::Point::manhattanLength(pos1, pos2));
	}

	/**
	 * @brief ユーティリティ
	*/
	TEST(Basic, BasicPoint_UtilityTest02) {

		alib::Point pos1(0, 0);
		alib::Point pos2(1, 0);

		ASSERT_NE(pos1.hash(), pos2.hash());
	}

	/**
	 * @brief ユーティリティ
	*/
	TEST(Basic, BasicPoint_UtilityTest03) {

		alib::Point pos(0, 0);

		ASSERT_EQ(pos.toLeft(), alib::Point::Left());
		ASSERT_EQ(pos.toRight(), alib::Point::Right());
		ASSERT_EQ(pos.toUp(), alib::Point::Up());
		ASSERT_EQ(pos.toDown(), alib::Point::Down());
	}
}
