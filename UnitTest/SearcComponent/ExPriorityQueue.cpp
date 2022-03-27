#include "pch.h"
#include "../pch.h"

#include <vector>
#include <memory>

#include "../../CppLib/SearcComponent/ExPriorityQueue.hpp"

namespace SearcComponent::exPriorityQueue {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_ExPriorityQueue, SearcComponent_ExPriorityQueue_FuncTest01) {

		using namespace alib::Search::Lib;

		ExPriorityQueue<int, ExPriorityQueue<int>::less> que;

		que.push(10);
		que.push(5);
		que.push(20);
		que.push(1);
		que.push(30);

		for (const auto& v : que) {
			std::cerr << v << std::endl;
		}

		ASSERT_EQ(que.top(), 30);
		que.pop();
		ASSERT_EQ(que.top(), 20);
		que.pop();
		ASSERT_EQ(que.top(), 10);
		que.pop();
		ASSERT_EQ(que.top(), 5);
		que.pop();
		ASSERT_EQ(que.top(), 1);
		que.pop();

		ASSERT_TRUE(que.empty());
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_ExPriorityQueue, SearcComponent_ExPriorityQueue_FuncTest02) {

		using namespace alib::Search::Lib;

		ExPriorityQueue<int, ExPriorityQueue<int>::greater> que;

		que.push(10);
		que.push(5);
		que.push(20);
		que.push(1);
		que.push(30);

		for (const auto& v : que) {
			std::cerr << v << std::endl;
		}

		ASSERT_EQ(que.top(), 1);
		que.pop();
		ASSERT_EQ(que.top(), 5);
		que.pop();
		ASSERT_EQ(que.top(), 10);
		que.pop();
		ASSERT_EQ(que.top(), 20);
		que.pop();
		ASSERT_EQ(que.top(), 30);
		que.pop();

		ASSERT_TRUE(que.empty());
	}

	/**
	 * @brief コンストラクタ構築テスト
	*/
	TEST(SearcComponent_ExPriorityQueue, SearcComponent_ExPriorityQueue_FuncTest03) {

		using namespace alib::Search::Lib;

		const std::vector<int> vec1{ 1, 2, 3, 4, 5 };
		const std::vector<int> vec2{ 10, 20, 30, 40, 50 };
		const std::less<int> comp{};

		ExPriorityQueue<int> que01;
		ASSERT_TRUE(que01.empty());
		ExPriorityQueue<int> que02(comp);
		ASSERT_TRUE(que02.empty());
		ExPriorityQueue<int> que03(comp, vec1);
		ASSERT_EQ(que03.top(), 5);
		ExPriorityQueue<int> que04(comp, { 1, 2, 3, 4, 5 });
		ASSERT_EQ(que04.top(), 5);

		ExPriorityQueue<int> que05(vec1.begin(), vec1.end());
		ASSERT_EQ(que05.top(), 5);
		ExPriorityQueue<int> que06(vec1.begin(), vec1.end(), comp);
		ASSERT_EQ(que06.top(), 5);
		ExPriorityQueue<int> que07(vec1.begin(), vec1.end(), comp, vec2);
		ASSERT_EQ(que07.top(), 50);
		ExPriorityQueue<int> que08(vec1.begin(), vec1.end(), comp, { 10, 20, 30, 40, 50 });
		ASSERT_EQ(que08.top(), 50);

		ExPriorityQueue<int> que09(vec1.get_allocator());
		ASSERT_TRUE(que09.empty());
		ExPriorityQueue<int> que10(comp, vec1.get_allocator());
		ASSERT_TRUE(que10.empty());
		ExPriorityQueue<int> que11(comp, vec1, vec1.get_allocator());
		ASSERT_EQ(que11.top(), 5);
		ExPriorityQueue<int> que12(comp, { 10, 20, 30, 40, 50 }, vec1.get_allocator());
		ASSERT_EQ(que12.top(), 50);

		ExPriorityQueue<int> que13(que03, vec2.get_allocator());
		ASSERT_EQ(que13.top(), 5);
		ExPriorityQueue<int> que14(std::move(que04), vec2.get_allocator());
		ASSERT_EQ(que14.top(), 5);

		ExPriorityQueue que16(comp, vec1);
		ASSERT_EQ(que16.top(), 5);
		testing::StaticAssertTypeEq<decltype(que16)::value_type, int>();

		ExPriorityQueue que17(vec1.begin(), vec1.end());
		ASSERT_EQ(que17.top(), 5);
		testing::StaticAssertTypeEq<decltype(que17)::value_type, int>();
	}

	/**
	 * @brief イテレータテスト
	*/
	TEST(SearcComponent_ExPriorityQueue, SearcComponent_ExPriorityQueue_FuncTest04) {

		using namespace alib::Search::Lib;

		ExPriorityQueue<int> que;

		que.push(1);
		que.push(2);
		que.push(3);
		{
			int sum = 0;
			for (decltype(que)::iterator it = que.begin(); it != que.end(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}
		{
			int sum = 0;
			for (decltype(que)::const_iterator it = std::as_const(que).begin(); it != std::as_const(que).end(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}
		{
			int sum = 0;
			for (decltype(que)::const_iterator it = que.cbegin(); it != que.cend(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}

		{
			int sum = 0;
			for (decltype(que)::reverse_iterator it = que.rbegin(); it != que.rend(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}
		{
			int sum = 0;
			for (decltype(que)::const_reverse_iterator it = std::as_const(que).rbegin(); it != std::as_const(que).rend(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}
		{
			int sum = 0;
			for (decltype(que)::const_reverse_iterator it = que.crbegin(); it != que.crend(); ++it) {
				sum += *it;
			}
			ASSERT_EQ(sum, 6);
		}
	}

	/**
	 * @brief ユーティリティテスト
	*/
	TEST(SearcComponent_ExPriorityQueue, SearcComponent_ExPriorityQueue_FuncTest05) {

		using namespace alib::Search::Lib;

		ExPriorityQueue<std::pair<int, double>> que1;

		que1.reserve(10);
		ASSERT_GE(que1.capacity(), 10);

		const auto copy = std::make_pair(0, 0.1);
		forange(i, 10) {
			que1.push(copy);
		}
		forange(i, 10) {
			que1.push(std::make_pair(i, 0.2));
		}
		forange(i, 10) {
			que1.emplace(i, 0.3);
		}

		ASSERT_EQ(que1.size(), 30);
		ExPriorityQueue<std::pair<int, double>> que2;
		que2.swap(que1);
		ASSERT_EQ(que1.size(), 0);
		ASSERT_EQ(que2.size(), 30);

		swap(que1, que2);
		ASSERT_EQ(que1.size(), 30);
		ASSERT_EQ(que2.size(), 0);

		que1.clear();
		ASSERT_TRUE(que1.empty());
	}

}
