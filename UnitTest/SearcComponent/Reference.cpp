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

		auto ref = Reference<int, Space>::Ref(a);

		//auto ref1 = ref;
		decltype(ref) ref2 = std::move(ref);
		//ref2 = ref;
		//ref2 = std::move(ref);

		ASSERT_EQ((int)ref2, 10);

		ref2 = 20;
		ASSERT_EQ(a, 20);
		ASSERT_EQ((int)ref2, 20);

		auto patch1 = memo.commit();

		patch1.undo();
		ASSERT_EQ(a, 10);
		ASSERT_EQ((int)ref2, 10);

		patch1.redo();
		ASSERT_EQ(a, 20);
		ASSERT_EQ((int)ref2, 20);

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
	TEST(SearcComponent_Memo, SearcComponent_Array_FuncTest01) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		Array<int, 3, Space> arr;
		ASSERT_EQ(arr.size(), 3);

		arr.store(0, 10);
		arr[1] = 20;
		arr.at(2) = 30;

		ASSERT_EQ(arr.load(0), 10);
		ASSERT_EQ(arr[0], 10);
		ASSERT_EQ(arr.at(0), 10);

		ASSERT_EQ(arr.load(1), 20);
		ASSERT_EQ(arr[1], 20);
		ASSERT_EQ(arr.at(1), 20);

		ASSERT_EQ(arr.load(2), 30);
		ASSERT_EQ(arr[2], 30);
		ASSERT_EQ(arr.at(2), 30);

		memo.lock();
		{
			int count = 1;
			for (auto& v : arr) {
				v.store(count * 100);
				count++;
			}
			ASSERT_EQ(arr.load(0), 100);
			ASSERT_EQ(arr.load(1), 200);
			ASSERT_EQ(arr.load(2), 300);
			memo.commit().undo();
			ASSERT_EQ(arr.load(0), 10);
			ASSERT_EQ(arr.load(1), 20);
			ASSERT_EQ(arr.load(2), 30);
		}

		{
			int count = 1;
			for (auto it = arr.begin(); it != arr.end(); ++it) {
				(*it).store(count * 1'000);
				count++;
			}
			ASSERT_EQ(arr.load(0), 1'000);
			ASSERT_EQ(arr.load(1), 2'000);
			ASSERT_EQ(arr.load(2), 3'000);
			memo.commit().undo();
			ASSERT_EQ(arr.load(0), 10);
			ASSERT_EQ(arr.load(1), 20);
			ASSERT_EQ(arr.load(2), 30);
		}

		{
			int count = 1;
			for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
				(*it).store(count * 10'000);
				count++;
			}
			ASSERT_EQ(arr.load(0), 30'000);
			ASSERT_EQ(arr.load(1), 20'000);
			ASSERT_EQ(arr.load(2), 10'000);
			memo.commit().undo();
			ASSERT_EQ(arr.load(0), 10);
			ASSERT_EQ(arr.load(1), 20);
			ASSERT_EQ(arr.load(2), 30);
		}

		arr.fill(1);
		ASSERT_EQ(arr.load(0), 1);
		ASSERT_EQ(arr.load(1), 1);
		ASSERT_EQ(arr.load(2), 1);
		memo.commit().undo();
		ASSERT_EQ(arr.load(0), 10);
		ASSERT_EQ(arr.load(1), 20);
		ASSERT_EQ(arr.load(2), 30);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Array_FuncTest02) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();
		memo.lock();

		Array<int, 3, Space> arr{ 10,20,30 };

		ASSERT_EQ(arr.load(0), 10);
		ASSERT_EQ(arr.load(1), 20);
		ASSERT_EQ(arr.load(2), 30);

		{
			std::vector<int> vec;
			for (auto& v : arr) {
				ASSERT_GT(v.load(), 0);
				vec.push_back(v.load());
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}
		{
			std::vector<int> vec;
			for (const auto& v : std::as_const(arr)) {
				ASSERT_GT(v, 0);
				vec.push_back(v);
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}
		{
			std::vector<int> vec;
			for (const auto& v : arr.as_const()) {
				ASSERT_GT(v, 0);
				vec.push_back(v);
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}

		{
			std::vector<int> vec;
			for (auto it = arr.begin(); it != arr.end(); ++it) {
				ASSERT_GT((*it).load(), 0);
				vec.push_back((*it).load());
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}
		{
			std::vector<int> vec;
			for (auto it = std::as_const(arr).begin(); it != std::as_const(arr).end(); ++it) {
				ASSERT_GT(*it, 0);
				vec.push_back(*it);
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}
		{
			std::vector<int> vec;
			for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
				ASSERT_GT(*it, 0);
				vec.push_back(*it);
			}
			ASSERT_EQ(vec.at(0), 10);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 30);
		}


		{
			std::vector<int> vec;
			for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
				ASSERT_GT((*it).load(), 0);
				vec.push_back((*it).load());
			}
			ASSERT_EQ(vec.at(0), 30);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 10);
		}
		{
			std::vector<int> vec;
			for (auto it = std::as_const(arr).rbegin(); it != std::as_const(arr).rend(); ++it) {
				ASSERT_GT(*it, 0);
				vec.push_back(*it);
			}
			ASSERT_EQ(vec.at(0), 30);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 10);
		}
		{
			std::vector<int> vec;
			for (auto it = arr.crbegin(); it != arr.crend(); ++it) {
				ASSERT_GT(*it, 0);
				vec.push_back(*it);
			}
			ASSERT_EQ(vec.at(0), 30);
			ASSERT_EQ(vec.at(1), 20);
			ASSERT_EQ(vec.at(2), 10);
		}
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Array_FuncTest03) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		Array<int, 3, Space> arr{ 10,20,30 };

		ASSERT_EQ(arr.front().load(), 10);
		ASSERT_EQ(std::as_const(arr).front(), 10);

		ASSERT_EQ(arr.back().load(), 30);
		ASSERT_EQ(std::as_const(arr).back(), 30);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Memo, SearcComponent_Array_FuncTest04) {
		using namespace alib::Search::Lib;
		struct Space {};
		auto& memo = MemoSingleton<Space>::Get();

		Array<int, 3, Space> arr{ 10,20,30 };

		decltype(arr)::iterator it01 = arr.begin();
		decltype(arr)::iterator it02 = arr.end();

		ASSERT_EQ(it01 + 3, it02);
		ASSERT_EQ(it01, it02 - 3);
		ASSERT_EQ(it02 - it01, 3);


		decltype(arr)::iterator it03 = arr.begin();
		decltype(arr)::iterator it04 = arr.begin();
		decltype(arr)::iterator it05 = ++it03;
		decltype(arr)::iterator it06 = it04++;

		ASSERT_EQ(it03, it04);
		ASSERT_EQ(it03, it01 + 1);
		ASSERT_EQ(it04, it01 + 1);
		ASSERT_EQ(it03, it02 - 2);
		ASSERT_EQ(it04, it02 - 2);

		ASSERT_EQ(it03, it05);
		ASSERT_NE(it04, it06);


		decltype(arr)::iterator it07 = arr.end();
		decltype(arr)::iterator it08 = arr.end();
		decltype(arr)::iterator it09 = --it07;
		decltype(arr)::iterator it10 = it08--;

		ASSERT_EQ(it07, it08);
		ASSERT_EQ(it07, it01 + 2);
		ASSERT_EQ(it08, it01 + 2);
		ASSERT_EQ(it07, it02 - 1);
		ASSERT_EQ(it08, it02 - 1);

		ASSERT_EQ(it07, it09);
		ASSERT_NE(it08, it10);


		decltype(arr)::iterator it11 = arr.begin();
		decltype(arr)::iterator it12 = arr.end();
		it11 += 3;
		it12 -= 3;

		ASSERT_EQ(it11, it02);
		ASSERT_EQ(it12, it01);
		ASSERT_EQ(it12[2].load(), (*(it02 - 1)).load());


		ASSERT_TRUE(it01 == it01);
		ASSERT_FALSE(it01 == it02);

		ASSERT_TRUE(it01 != it02);
		ASSERT_FALSE(it01 != it01);

		ASSERT_TRUE(it01 < it02);
		ASSERT_FALSE(it01 < it01);
		ASSERT_FALSE(it02 < it01);

		ASSERT_TRUE(it02 > it01);
		ASSERT_FALSE(it02 > it02);
		ASSERT_FALSE(it01 > it02);

		ASSERT_TRUE(it01 <= it02);
		ASSERT_TRUE(it01 <= it01);
		ASSERT_FALSE(it02 <= it01);

		ASSERT_TRUE(it02 >= it01);
		ASSERT_TRUE(it02 >= it02);
		ASSERT_FALSE(it01 >= it02);
	}
}
