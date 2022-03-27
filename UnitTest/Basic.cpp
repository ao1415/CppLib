#include "pch.h"
#include "../CppLib/Basic.hpp"

namespace forange {
	/**
	 * @brief 単純ループテスト
	 * int(正)
	*/
	TEST(Basic, forange_LoopTest01) {

		int count = 0;

		forange(i, 10) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * int(負)
	*/
	TEST(Basic, forange_LoopTest02) {

		int count = 0;

		forange(i, -1) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			ASSERT_TRUE(false);
		}

		EXPECT_EQ(count, 0);
	}

	/**
	 * @brief 単純ループテスト
	 * double(正)
	*/
	TEST(Basic, forange_LoopTest03) {

		int count = 0;

		forange(i, 10.0) {
			EXPECT_STREQ(typeid(i).name(), typeid(double).name());
			count++;
		}

		EXPECT_GE(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forange_LoopTest04) {

		int count = 0;

		forange(i, size_t(10)) {
			EXPECT_STREQ(typeid(i).name(), typeid(size_t).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}
}

namespace forange_type {
	/**
	 * @brief 単純ループテスト
	 * int(正)
	*/
	TEST(Basic, forange_type_LoopTest01) {

		int count = 0;

		forange_type(int, i, 10) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * int(負)
	*/
	TEST(Basic, forange_type_LoopTest02) {

		int count = 0;

		forange_type(int, i, -1) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			ASSERT_TRUE(false);
		}

		EXPECT_EQ(count, 0);
	}

	/**
	 * @brief 単純ループテスト
	 * double(正)
	*/
	TEST(Basic, forange_type_LoopTest03) {

		int count = 0;

		forange_type(int, i, 10.0) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			count++;
		}

		EXPECT_GE(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forange_type_LoopTest04) {

		int count = 0;

		forange_type(size_t, i, 10) {
			EXPECT_STREQ(typeid(i).name(), typeid(size_t).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}
}

namespace forstep {
	/**
	 * @brief 単純ループテスト
	 * int(正)
	*/
	TEST(Basic, forstep_LoopTest01) {

		int count = 0;

		forstep(i, 0, 10) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * int(負)
	*/
	TEST(Basic, forstep_LoopTest02) {

		int count = 0;

		forstep(i, 0, -1) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			ASSERT_TRUE(false);
		}

		EXPECT_EQ(count, 0);
	}

	/**
	 * @brief 単純ループテスト
	 * double(正)
	*/
	TEST(Basic, forstep_LoopTest03) {

		int count = 0;

		forstep(i, 0, 10.0) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			count++;
		}

		EXPECT_GE(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forstep_LoopTest04) {

		int count = 0;

		forstep(i, 0, size_t(10)) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forstep_LoopTest05) {

		int count = -10;

		forstep(i, -10, size_t(10)) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}
}

namespace forstep_type {
	/**
	 * @brief 単純ループテスト
	 * int(正)
	*/
	TEST(Basic, forstep_type_LoopTest01) {

		int count = 0;

		forstep_type(int, i, 0, 10) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * int(負)
	*/
	TEST(Basic, forstep_type_LoopTest02) {

		int count = 0;

		forstep_type(int, i, 0, -1) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			ASSERT_TRUE(false);
		}

		EXPECT_EQ(count, 0);
	}

	/**
	 * @brief 単純ループテスト
	 * double(正)
	*/
	TEST(Basic, forstep_type_LoopTest03) {

		int count = 0;

		forstep_type(int, i, 0, 10.0) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			count++;
		}

		EXPECT_GE(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forstep_type_LoopTest04) {

		int count = 0;

		forstep_type(size_t, i, 0, size_t(10)) {
			EXPECT_STREQ(typeid(i).name(), typeid(size_t).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}

	/**
	 * @brief 単純ループテスト
	 * size_t
	*/
	TEST(Basic, forstep_type_LoopTest05) {

		int count = -10;

		forstep_type(int, i, -10, size_t(10)) {
			EXPECT_STREQ(typeid(i).name(), typeid(int).name());
			EXPECT_EQ(i, count);
			count++;
		}

		EXPECT_EQ(count, 10);
	}
}

namespace ContainerAllocator {
	/**
	 * @brief サイズ0指定
	*/
	TEST(Basic, ContainerAllocator_Test01) {
		constexpr auto size = alib::ContainerAllocator<0>();
		EXPECT_EQ(size, 1);
	}

	/**
	 * @brief サイズ1指定
	*/
	TEST(Basic, ContainerAllocator_Test02) {
		constexpr auto size = alib::ContainerAllocator<1>();
		EXPECT_EQ(size, 1);
	}

	/**
	 * @brief サイズ10指定
	*/
	TEST(Basic, ContainerAllocator_Test03) {
		constexpr auto size = alib::ContainerAllocator<10>();
		EXPECT_EQ(size, 16);
	}

	/**
	 * @brief サイズ16指定
	*/
	TEST(Basic, ContainerAllocator_Test04) {
		constexpr auto size = alib::ContainerAllocator<16>();
		EXPECT_EQ(size, 16);
	}

	/**
	 * @brief サイズ2^32-1指定
	*/
	TEST(Basic, ContainerAllocator_Test05) {
		constexpr auto max = std::numeric_limits<size_t>::max();
		constexpr auto size = alib::ContainerAllocator<max>();
		EXPECT_EQ(size, max);
	}

	/**
	 * @brief サイズ-1指定
	*/
	TEST(Basic, ContainerAllocator_Test06) {
		constexpr auto max = std::numeric_limits<size_t>::max();
		constexpr auto size = alib::ContainerAllocator<-1>();
		EXPECT_EQ(size, max);
	}
}

namespace NarrowCast {
	/**
	 * @brief 単純機能テスト
	*/
	TEST(Basic, NarrowCast_Test01) {
		EXPECT_EQ(alib::narrow_cast<int>(size_t(0)), 0);
		EXPECT_EQ(alib::narrow_cast<int>(size_t(1)), 1);
		EXPECT_EQ(alib::narrow_cast<int>(size_t(std::numeric_limits<int>::max())), std::numeric_limits<int>::max());
		EXPECT_EQ(alib::narrow_cast<int>(size_t(std::numeric_limits<int>::max()) + 1), -2147483648);
	}

}
