#include "pch.h"
#include "../pch.h"

#include "../../CppLib/SearcComponent/Reference.hpp"

namespace SearcComponent::ReferenceTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Reference_FuncTest01) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();
		memo.lock();

		int a = 10;
		int b = 20;
		using Ref = Reference<int, Space>;

		auto ref = Ref::Ref(a);
		decltype(ref) ref2 = std::move(ref);

		auto ref3 = Ref::Ref(b);
		ref3 = ref2;

		ASSERT_EQ((int)ref2, 10);
		ASSERT_EQ((int)ref3, 10);

		ref2 = 20;
		ASSERT_EQ(a, 20);
		ASSERT_EQ((int)ref2, 20);
		ASSERT_EQ((int)ref3, 10);

		auto patch1 = memo.commit();

		patch1.undo();
		ASSERT_EQ(a, 10);
		ASSERT_EQ((int)ref2, 10);
		ASSERT_EQ((int)ref3, 20);

		patch1.redo();
		ASSERT_EQ(a, 20);
		ASSERT_EQ((int)ref2, 20);
		ASSERT_EQ((int)ref3, 10);

		ref2.store(30);
		ASSERT_EQ(a, 30);
		ASSERT_EQ(ref2.load(), 30);

		auto patch2 = memo.commit();

		patch2.undo();
		ASSERT_EQ(a, 20);
		ASSERT_EQ(ref2.load(), 20);

		patch2.redo();
		ASSERT_EQ(a, 30);
		ASSERT_EQ(ref2.load(), 30);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Value_FuncTest01) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();
		memo.lock();

		Value<int, Space> value1;
		Value<int, Space> value2(10);

		//decltype(value1) value3 = value1;
		decltype(value1) value3 = std::move(value1);

		//value1 = value2;
		//value1 = std::move(value2);

		ASSERT_EQ((int)value1, 0);
		ASSERT_EQ(value1.load(), 0);
		ASSERT_EQ((int)value2, 10);
		ASSERT_EQ(value2.load(), 10);

		value2 = 20;
		ASSERT_EQ((int)value2, 20);
		ASSERT_EQ(value2.load(), 20);

		auto patch1 = memo.commit();

		patch1.undo();
		ASSERT_EQ((int)value2, 10);
		ASSERT_EQ(value2.load(), 10);

		patch1.redo();
		ASSERT_EQ((int)value2, 20);
		ASSERT_EQ(value2.load(), 20);

		value2.store(30);
		ASSERT_EQ((int)value2, 30);
		ASSERT_EQ(value2.load(), 30);

		auto patch2 = memo.commit();

		patch2.undo();
		ASSERT_EQ((int)value2, 20);
		ASSERT_EQ(value2.load(), 20);

		patch2.redo();
		ASSERT_EQ((int)value2, 30);
		ASSERT_EQ(value2.load(), 30);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Value_FuncTest02) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();
		memo.lock();

		struct Data {
			Value<int, Space> value1{};
			Value<int, Space> value2{ 10 };
		};

		Data data1;
		Data data2;

		ASSERT_EQ(data1.value1.load(), 0);
		ASSERT_EQ(data1.value2.load(), 10);
		ASSERT_EQ(data2.value1.load(), 0);
		ASSERT_EQ(data2.value2.load(), 10);

		data2.value1 = 30;
		data2.value2 = 40;

		auto patch1 = memo.commit();

		patch1.undo();
		ASSERT_EQ(data2.value1.load(), 0);
		ASSERT_EQ(data2.value2.load(), 10);

		patch1.redo();
		ASSERT_EQ(data2.value1.load(), 30);
		ASSERT_EQ(data2.value2.load(), 40);

		data1 = data2;

		ASSERT_EQ(data1.value1.load(), 30);
		ASSERT_EQ(data1.value2.load(), 40);

		auto patch2 = memo.commit();

		patch2.undo();
		ASSERT_EQ(data1.value1.load(), 0);
		ASSERT_EQ(data1.value2.load(), 10);

		patch2.redo();
		ASSERT_EQ(data1.value1.load(), 30);
		ASSERT_EQ(data1.value2.load(), 40);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Vector_FuncTest01) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		using Val = Value<int, Space>;
		using Vec = Vector<Val, 10, Space>;

		Vec value6({ Val(0),Val(1),Val(2),Val(3),Val(4) });

		Vec value1;
		Vec value2(value6);

		value1 = value6;

		Vec value3(3);
		Vec value4(3, Val(10));
		std::vector<int> vec({ 0,1,2,3 });
		Vec value5(vec.begin(), vec.end());

		memo.lock();

		ASSERT_EQ(value1.size(), 5);
		ASSERT_EQ(value2.size(), 5);
		ASSERT_EQ(value3.size(), 3);
		ASSERT_EQ(value4.size(), 3);
		ASSERT_EQ(value5.size(), 4);
		ASSERT_EQ(value6.size(), 5);

		ASSERT_EQ(value1[0], 0);
		ASSERT_EQ(value1[1], 1);
		ASSERT_EQ(value1[2], 2);
		ASSERT_EQ(value1[3], 3);
		ASSERT_EQ(value1[4], 4);

		ASSERT_EQ(value2[0], 0);
		ASSERT_EQ(value2[1], 1);
		ASSERT_EQ(value2[2], 2);
		ASSERT_EQ(value2[3], 3);
		ASSERT_EQ(value2[4], 4);

		ASSERT_EQ(value3[0], 0);
		ASSERT_EQ(value3[1], 0);
		ASSERT_EQ(value3[2], 0);

		ASSERT_EQ(value4[0], 10);
		ASSERT_EQ(value4[1], 10);
		ASSERT_EQ(value4[2], 10);

		ASSERT_EQ(value5[0], 0);
		ASSERT_EQ(value5[1], 1);
		ASSERT_EQ(value5[2], 2);
		ASSERT_EQ(value5[3], 3);

		ASSERT_EQ(value6[0], 0);
		ASSERT_EQ(value6[1], 1);
		ASSERT_EQ(value6[2], 2);
		ASSERT_EQ(value6[3], 3);
		ASSERT_EQ(value6[4], 4);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Vector_FuncTest02) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		using Val = Value<int, Space>;
		using Vec = Vector<Val, 10, Space>;

		Vec value6({ Val(0),Val(1),Val(2),Val(3),Val(4) });

		memo.lock();
		// TODO
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Vector_FuncTest03) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		using Val = Value<int, Space>;
		using Vec = Vector<Val, 10, Space>;

		Vec value6({ Val(0),Val(1),Val(2),Val(3),Val(4) });

		memo.lock();
		// TODO
	}
}
