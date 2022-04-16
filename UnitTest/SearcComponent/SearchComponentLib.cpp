#include "pch.h"
#include "../pch.h"

#include "../../CppLib/SearcComponent/SearchComponentLib.hpp"

namespace SearcComponent::SearchComponentLibTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_SearchComponentLib, SearcComponent_FuncTest01) {
		using namespace alib::Search::Lib;

		ASSERT_EQ(1_K, 1024);
		ASSERT_EQ(16_K, 16 * 1024);

		ASSERT_EQ(1_M, 1024 * 1024);
		ASSERT_EQ(16_M, 16 * 1024 * 1024);
	}
}