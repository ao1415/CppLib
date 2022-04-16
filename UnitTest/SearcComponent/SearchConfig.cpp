#include "pch.h"
#include "../pch.h"

#include "../../CppLib/SearcComponent/SearchConfig.hpp"

namespace SearcComponent::SearchConfigTest {

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearchConfig_DefaultSearchMethod, DefaultSearchMethod_FuncTest01) {
		using namespace alib::Search;

		struct Arg {
			Arg() {}
			Arg(int a, double b) :a(a), b(b) {}
			int a{};
			double b{};
		};

		using Method = DefaultSearchMethod<Arg>;

		testing::StaticAssertTypeEq<Arg, Method::ArgumentType>();
		testing::StaticAssertTypeEq<double, Method::ScoreType>();
		testing::StaticAssertTypeEq<void, Method::HashType>();

		Method method1;
		ASSERT_EQ(0, method1.argument.a);
		ASSERT_EQ(0.0, method1.argument.b);
		ASSERT_EQ(0.0, method1.score);

		Method method2(0);
		Method method3(0, Arg{ 1,2.0 });
		Method method4(0, 1, 2.0);

		ASSERT_EQ(0, method2.argument.a);
		ASSERT_EQ(0.0, method2.argument.b);

		ASSERT_EQ(1, method3.argument.a);
		ASSERT_EQ(2.0, method3.argument.b);

		ASSERT_EQ(1, method4.argument.a);
		ASSERT_EQ(2.0, method4.argument.b);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearchConfig_DefaultSearchMethod, DefaultSearchMethod_FuncTest02) {
		using namespace alib::Search;

		struct Arg {
			Arg() {}
			Arg(int a, double b) :a(a), b(b) {}
			int a{};
			double b{};
		};

		using Method = DefaultSearchMethod<Arg, double, size_t>;

		testing::StaticAssertTypeEq<Arg, Method::ArgumentType>();
		testing::StaticAssertTypeEq<double, Method::ScoreType>();
		testing::StaticAssertTypeEq<size_t, Method::HashType>();

		Method method;
		ASSERT_EQ(0, method.argument.a);
		ASSERT_EQ(0.0, method.argument.b);
		ASSERT_EQ(0.0, method.score);
		ASSERT_EQ(0, method.hash);

		Method method2(0);
		Method method3(0, 1);
		Method method4(0, 1, Arg{ 2, 3.0 });
		Method method5(0, 1, 2, 3.0);

		ASSERT_EQ(0, method2.argument.a);
		ASSERT_EQ(0.0, method2.argument.b);

		ASSERT_EQ(0, method3.argument.a);
		ASSERT_EQ(0.0, method3.argument.b);

		ASSERT_EQ(2, method4.argument.a);
		ASSERT_EQ(3.0, method4.argument.b);

		ASSERT_EQ(2, method5.argument.a);
		ASSERT_EQ(3.0, method5.argument.b);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearchConfig_ConfigTemplate, ConfigTemplate_FuncTest01) {
		using namespace alib::Search;

		struct Arg {
			Arg() {}
			Arg(int a, double b) :a(a), b(b) {}
			int a{};
			double b{};
		};

		using Method = DefaultSearchMethod<Arg, double, size_t>;
		using Config = ConfigTemplate<10, 100, 2000, Method>;

		testing::StaticAssertTypeEq<Method, Config::SearchMethod>();
		testing::StaticAssertTypeEq<Arg, Config::ArgumentType>();
		testing::StaticAssertTypeEq<double, Config::ScoreType>();
		testing::StaticAssertTypeEq<size_t, Config::HashType>();

		constexpr auto hash = Config::HasHash();
		constexpr auto debug = Config::GetDebug();
		constexpr auto depth = Config::GetDepth();
		constexpr auto width = Config::GetWidth();
		constexpr auto limit = Config::GetLimit();

		ASSERT_TRUE(hash);
		ASSERT_FALSE(debug);

		ASSERT_EQ(10, depth);
		ASSERT_EQ(100, width);
		ASSERT_EQ(2000, limit);
	}
}