#pragma once

#include "FixedDeque.hpp"

namespace alib {

	template<typename Type, size_t Size>
	class FixedStack {
	private:

		FixedDeque<Type, Size> que;

	public:

		FixedStack() = default;
		FixedStack(const FixedStack& other) = default;
		FixedStack(FixedStack&& other) {
			que = std::move(other);
		}

		FixedStack& operator=(const FixedStack& other) = default;
		FixedStack& operator=(FixedStack&& other) = default;

		void push(const Type& v) {
			que.push_back(v);
		}
		void push(Type&& v) {
			que.push_back(v);
		}

		void pop() {
			que.pop_back();
		}

		constexpr size_t size() const noexcept {
			return que.size();
		}

		size_t element_num() const noexcept {
			return que.element_num();
		}

		bool empty() const noexcept {
			return que.empty();
		}

		bool full() const noexcept {
			return que.full();
		}

		const Type& top() const {
			return que.back();
		}
		Type& top() {
			return que.back();
		}

		void clear() noexcept {
			que.clear();
		}

		void swap(FixedStack& other) noexcept(_Is_nothrow_swappable<Type>::value) {
			que.swap(other.que);
		}

	};

}
