#pragma once

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

		const int size() const {
			return 0;
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

		Type m_data[Size];

		int start = 0;
		int end = 0;

	};

}
