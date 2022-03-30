#include "pch.h"
#include "../pch.h"

#include <array>

#include "../../CppLib/Basic.hpp"
#include "../../CppLib/SearcComponent/Memo.hpp"

namespace SearcComponent::MemoTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Memo_FuncTest01) {
		using namespace alib::Search::Lib;

		struct A {};
		auto& memo = MemoSingleton<A>::Get();
		memo.unlock();
		memo.lock();

		int a = 10;
		memo.modify(a);
		a = 20;

		const auto patch = memo.commit();

		patch.undo();
		ASSERT_EQ(a, 10);

		patch.redo();
		ASSERT_EQ(a, 20);

		memo.release(patch);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Memo_FuncTest02) {
		using namespace alib::Search::Lib;

		struct A {};
		auto& memo = MemoSingleton<A>::Get();
		memo.unlock();
		memo.lock();

		std::array<int, 10> data{};
		forange(i, data.size()) {
			data.at(i) = alib::narrow_cast<int>(i);
		}
		for (auto& d : data) {
			memo.modify(d);
		}

		forange(i, data.size()) {
			data.at(i) = alib::narrow_cast<int>(i * 10);
		}
		const auto patch = memo.commit();

		patch.undo();
		forange(i, data.size()) {
			ASSERT_EQ(data.at(i), i);
		}

		patch.redo();
		forange(i, data.size()) {
			ASSERT_EQ(data.at(i), i * 10);
		}

		memo.release(patch);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Memo_FuncTest03) {
		using namespace alib::Search::Lib;

		struct A {};
		struct B {};
		struct C : public B {};
		auto& memoA1 = MemoSingleton<A>::Get();
		auto& memoA2 = MemoSingleton<A>::Get();
		auto& memoB = MemoSingleton<B>::Get();
		auto& memoC = MemoSingleton<C>::Get();

		ASSERT_EQ(std::addressof(memoA1), std::addressof(memoA2));
		ASSERT_NE(std::addressof(memoA1), std::addressof(memoB));
		ASSERT_NE(std::addressof(memoB), std::addressof(memoC));

		memoB.lock();
		memoC.lock();

		int b = 10;
		int c = 100;
		memoB.modify(b);
		memoC.modify(c);
		b = 20;
		c = 200;

		const auto patchB = memoB.commit();
		const auto patchC = memoC.commit();

		patchB.undo();
		ASSERT_EQ(b, 10);
		ASSERT_EQ(c, 200);

		patchB.redo();
		ASSERT_EQ(b, 20);
		ASSERT_EQ(c, 200);

		patchC.undo();
		ASSERT_EQ(b, 20);
		ASSERT_EQ(c, 100);

		patchC.redo();
		ASSERT_EQ(b, 20);
		ASSERT_EQ(c, 200);

		memoB.release(patchB);
		memoC.release(patchC);
	}
}
