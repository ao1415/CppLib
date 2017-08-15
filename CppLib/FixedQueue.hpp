#pragma once

#include "FixedDeque.hpp"

namespace alib {

	template<typename Type, size_t Size>
	class Queue {
	public:

		Queue() = default;
		Queue(const Queue& other) {

		}
		Queue(Queue&& other) {

		}

		const bool empty() const {
			return start == end;
		}

		constexpr int size() const noexcept {
			return containerAllocator(Size);
		}

		const Type& front() const {

		}

		void push(const Type& v) {

		}
		void push(Type&& v) {

		}

		void emplace(Type&& v) {

		}

		void pop() {

		}

		void swap(Queue& other) {

		}

	private:

		FixedDeque<Type, Size> deque;

	};

}
