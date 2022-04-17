#include "pch.h"
#include "../pch.h"

#include "../../CppLib/SearcComponent/SearchConfig.hpp"
#include "../../CppLib/SearcComponent/SearchNode.hpp"

namespace SearcComponent::SearchNodeTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_SearchNode, SearchNode_FuncTest01) {
		using namespace alib::Search;
		using namespace alib::Search::Lib;

		struct Arg {
			Arg() {}
			Arg(int a, double b) :a(a), b(b) {}
			int a{};
			double b{};
		};

		using Method = DefaultSearchMethod<Arg, double, size_t>;
		using Config = ConfigTemplate<10, 100, 2000, Method>;

		SearchNode<Config> node1;
		SearchNode<Config> node2(std::addressof(node1), Method{ 0,0,Arg{ 1,2.0 } });

		ASSERT_EQ(2, node1.ref);

		ASSERT_EQ(1, node2.ref);
		ASSERT_EQ(1, node2.depth);
		ASSERT_EQ(std::addressof(node1), node2.parent);
		ASSERT_EQ(1, node2.searchArgument.argument.a);
		ASSERT_EQ(2.0, node2.searchArgument.argument.b);

		node2.subRef();
		ASSERT_EQ(0, node2.ref);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_SearchNodePool, SearchNodePool_FuncTest01) {
		using namespace alib::Search;
		using namespace alib::Search::Lib;

		struct Arg {
			Arg() {}
			Arg(int a, double b) :a(a), b(b) {}
			int a{};
			double b{};
		};

		using Method = DefaultSearchMethod<Arg, double, size_t>;
		using Config = ConfigTemplate<10, 100, 2000, Method>;

		SearchNodePool<Config> pool;

		const auto ptr1 = pool.alloc();
		pool.release(ptr1);

		const auto ptr2 = pool.alloc();
		const auto ptr3 = pool.alloc();

		ASSERT_EQ(ptr1, ptr2);
		ASSERT_NE(ptr1, ptr3);
	}
}