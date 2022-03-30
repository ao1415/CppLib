#include "pch.h"
#include "../pch.h"

#include <array>

#include "../../CppLib/SearcComponent/PagingMemory.hpp"

namespace SearcComponent::PagingMemoryTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_PagingMemory, SearcComponent_PagingMemory_FuncTest01) {
		using namespace alib::Search::Lib;

		PagingMemory<std::byte, 10> memory;

		const auto p01 = memory.allocate(4);
		const auto p02 = memory.allocate(4);

		const auto p03 = memory.allocate(4);
		memory.release(p01);
		memory.release(p02);
		const auto p04 = memory.allocate(4);

		const auto p05 = memory.allocate(4);
		memory.release(p05);
		const auto p06 = memory.allocate(6);
		memory.release(p06);

		const auto p07 = memory.allocate(4);

		ASSERT_NE(p01, p02);
		ASSERT_NE(p02, p03);
		ASSERT_NE(p03, p04);
		ASSERT_NE(p04, p05);

		ASSERT_EQ(p01, p05);
		ASSERT_EQ(p02, p06);
		ASSERT_EQ(p05, p07);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_PagingMemory, SearcComponent_PagingMemory_FuncTest02) {
		using namespace alib::Search::Lib;

		struct Data {
			Data() : Data(0, 0.1) {}
			Data(int _a, double _b) :a(_a), b(_b) {}
			int a;
			double b;
		};

		PagingMemory<Data, 3> memory;

		const auto p01 = memory.create();
		const auto p02 = memory.create(12, 20);
		const auto p03 = memory.create(13, 20);

		const auto p04 = memory.create(14, 20);
		const auto p05 = memory.create(15, 20);
		const auto p06 = memory.create(16, 20);

		ASSERT_NE(p01, p02);
		ASSERT_NE(p02, p03);
		ASSERT_NE(p03, p04);
		ASSERT_NE(p04, p05);
		ASSERT_NE(p05, p06);

		ASSERT_EQ(p01->a, 0);
		ASSERT_EQ(p01->b, 0.1);
		ASSERT_EQ(p02->a, 12);
		ASSERT_EQ(p02->b, 20);

		memory.release(p01);
		memory.release(p02);
		memory.release(p03);

		const auto p07 = memory.create(17, 20);
		ASSERT_EQ(p01, p07);

		memory.release(p04);
		memory.release(p05);
		memory.release(p06);

		const auto p08 = memory.create(18, 20);
		ASSERT_EQ(p02, p08);
	}
}
