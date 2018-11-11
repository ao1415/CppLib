#pragma once

#include "FixedDeque.hpp"

namespace alib {

	template<typename Type, size_t Size>
	class FixedQueue {
	private:

		FixedDeque<Type, Size> que;

	public:

		FixedQueue() = default;
		FixedQueue(const FixedQueue& other) = default;
		FixedQueue(FixedQueue&& other) {
			que = std::move(other.que);
		}

		FixedQueue& operator=(const FixedQueue& other) = default;
		FixedQueue& operator=(FixedQueue&& other) = default;

		void push(const Type& v) {
			que.push_back(v);
		}
		void push(Type&& v) {
			que.push_back(v);
		}

		void pop() {
			que.pop_front();
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

		const Type& front() const {
			return que.front();
		}
		Type& front() {
			return que.front();
		}

		void clear() noexcept {
			que.clear();
		}

		void swap(FixedQueue& other) noexcept(_Is_nothrow_swappable<Type>::value) {
			que.swap(other.que);
		}

	};

}
