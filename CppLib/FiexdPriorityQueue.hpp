#pragma once

#include <array>
#include <algorithm>

#include "Basic.hpp"

namespace alib
{

	template <class Type, size_t Size, class Compare = std::less<typename Type>>
	class FiexdPriorityQueue
	{
	public:
		using value_type = Type;
		using reference = Type &;
		using const_reference = const Type &;
		using value_compare = Compare;

		using Sequence = std::array<Type, containerAllocator<Size>()>;

	private:
		Sequence c;
		Compare comp;

		size_t end = 0;

	public:
		FiexdPriorityQueue() : c(), comp() {}

		explicit FiexdPriorityQueue(const Compare &__x, size_t __e, const Sequence &__s) : c(__s), end(__e), comp(__x)
		{
			std::make_heap(c.begin(), c.begin() + end, comp);
		}

		explicit FiexdPriorityQueue(const Compare &__x, size_t __e = 0, Sequence &&__s = Sequence()) : c(std::move(__s)), end(__e), comp(__x)
		{
			std::make_heap(c.begin(), c.begin() + end, comp);
		}

		bool empty() const { return (end == 0); }
		size_t size() const { return end; }

		const_reference top() const { return c.front(); }

		void push(const value_type &__x)
		{
			c[end] = __x;
			end++;
			std::push_heap(c.begin(), c.begin() + end, comp);
		}

		void push(value_type &&__x)
		{
			c[end] = std::move(__x);
			end++;
			std::push_heap(c.begin(), c.begin() + end, comp);
		}

		template <typename... _Args>
		void emplace(_Args &&... __args)
		{
			c[end] = std::move(Type(std::forward<_Args>(__args)...));
			end++;
			std::push_heap(c.begin(), c.begin() + end, comp);
		}

		void pop()
		{
			std::pop_heap(c.begin(), c.begin() + end, comp);
			end--;
		}
	};

} // namespace alib
