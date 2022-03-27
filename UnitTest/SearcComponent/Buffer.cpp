#include "pch.h"
#include "../pch.h"

#include <vector>
#include <memory>

#include "../../CppLib/SearcComponent/Buffer.hpp"

namespace SearcComponent::buffer {

	template<class T, class M = unsigned short>
	size_t calcSize() {
		return sizeof(void*) + sizeof(M) + sizeof(T) + sizeof(T);
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Buffer, SearcComponent_Buffer_FuncTest01) {

		using namespace alib::Search::Lib;

		Buffer<100> buffer;
		buffer.lock();
		buffer.unlock();
		buffer.lock();

		int a = 10;
		buffer.modify(std::addressof(a), sizeof(a));
		a = 20;

		const size_t modifySize = calcSize<decltype(a)>();
		EXPECT_EQ(buffer.wholeSize(), modifySize);

		std::vector<std::byte> data(buffer.wholeSize());
		Stream buf(data.data(), data.size());

		buffer.commit(buf);

		size_t ptr = 0;
		using size_type = decltype(buffer)::MemoSizeType;
		{
			const auto p = std::addressof(a);
			using value_type = std::decay_t<decltype(*p)>;

			EXPECT_EQ(*reinterpret_cast<value_type**>(std::addressof(data.at(ptr))), p);
			ptr += sizeof(value_type*);
			EXPECT_EQ(*reinterpret_cast<size_type*>(std::addressof(data.at(ptr))), sizeof(value_type));
			ptr += sizeof(size_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 20);
			ptr += sizeof(value_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 10);
			ptr += sizeof(value_type);
		}
	}

	/**
	 * @brief 単純機能テスト
	*/
	TEST(SearcComponent_Buffer, SearcComponent_Buffer_FuncTest02) {

		using namespace alib::Search::Lib;

		Buffer<100> buffer;
		buffer.lock();

		int a = 10;
		buffer.modify(std::addressof(a), sizeof(a));
		a = 20;
		buffer.modify(std::addressof(a), sizeof(a));
		a = 30;

		double b = 10.1;
		buffer.modify(std::addressof(b), sizeof(b));
		b = 20.2;
		buffer.modify(std::addressof(b), sizeof(b));
		b = 30.3;

		const size_t modifySize = calcSize<decltype(a)>() + calcSize<decltype(a)>() + calcSize<decltype(b)>() + calcSize<decltype(b)>();
		EXPECT_EQ(buffer.wholeSize(), modifySize);

		std::vector<std::byte> data(buffer.wholeSize());
		Stream buf(data.data(), data.size());

		buffer.commit(buf);

		size_t ptr = 0;
		using size_type = decltype(buffer)::MemoSizeType;
		{
			const auto p = std::addressof(b);
			using value_type = std::decay_t<decltype(*p)>;

			EXPECT_EQ(*reinterpret_cast<value_type**>(std::addressof(data.at(ptr))), p);
			ptr += sizeof(value_type*);
			EXPECT_EQ(*reinterpret_cast<size_type*>(std::addressof(data.at(ptr))), sizeof(value_type));
			ptr += sizeof(size_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 30.3);
			ptr += sizeof(value_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 20.2);
			ptr += sizeof(value_type);
		}
		{
			const auto p = std::addressof(b);
			using value_type = std::decay_t<decltype(*p)>;

			EXPECT_EQ(*reinterpret_cast<value_type**>(std::addressof(data.at(ptr))), p);
			ptr += sizeof(value_type*);
			EXPECT_EQ(*reinterpret_cast<size_type*>(std::addressof(data.at(ptr))), sizeof(value_type));
			ptr += sizeof(size_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 30.3);
			ptr += sizeof(value_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 10.1);
			ptr += sizeof(value_type);
		}
		{
			const auto p = std::addressof(a);
			using value_type = std::decay_t<decltype(*p)>;

			EXPECT_EQ(*reinterpret_cast<value_type**>(std::addressof(data.at(ptr))), p);
			ptr += sizeof(value_type*);
			EXPECT_EQ(*reinterpret_cast<size_type*>(std::addressof(data.at(ptr))), sizeof(value_type));
			ptr += sizeof(size_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 30);
			ptr += sizeof(value_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 20);
			ptr += sizeof(value_type);
		}
		{
			const auto p = std::addressof(a);
			using value_type = std::decay_t<decltype(*p)>;

			EXPECT_EQ(*reinterpret_cast<value_type**>(std::addressof(data.at(ptr))), p);
			ptr += sizeof(value_type*);
			EXPECT_EQ(*reinterpret_cast<size_type*>(std::addressof(data.at(ptr))), sizeof(value_type));
			ptr += sizeof(size_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 30);
			ptr += sizeof(value_type);
			EXPECT_EQ(*reinterpret_cast<value_type*>(std::addressof(data.at(ptr))), 10);
			ptr += sizeof(value_type);
		}
	}
}
