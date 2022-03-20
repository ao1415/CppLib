#pragma once

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

#include "SearchComponentLib.hpp"

namespace alib::Search::Lib {

	template<class Type, size_type Count>
	class PagingMemory final {
	public:
		using value_type = Type;
		using allocator_type = std::allocator<value_type>;
		using traits = std::allocator_traits<allocator_type>;

		using pointer = typename traits::pointer;
		using const_pointer = typename traits::const_pointer;

		static_assert(std::is_same_v<value_type, traits::value_type>);
		static_assert(std::is_trivially_copyable_v<value_type>);
	private:

		NODISCARD inline static constexpr size_type GetPagingCount() noexcept { return Count; }

		std::stack<pointer> unusedMemory;
		std::vector<std::pair<uintptr_t, size_type>> memory;

		pointer first = nullptr;
		pointer last = nullptr;
		pointer reserved = nullptr;

		allocator_type alloc;

		NODISCARD inline uintptr_t toInteger(const const_pointer ptr) const noexcept {
			WARN_PUSH_DISABLE(26490);
			return reinterpret_cast<uintptr_t>(ptr);
			WARN_POP();
		}
		NODISCARD inline pointer toPointer(const uintptr_t ptr) const noexcept {
			WARN_PUSH_DISABLE(26490);
			return reinterpret_cast<pointer>(ptr);
			WARN_POP();
		}
		NODISCARD inline pointer addPointer(const pointer ptr, const size_type size) const noexcept {
			WARN_PUSH_DISABLE(26481);
			return ptr + size;
			WARN_POP();
		}

		inline void deploy(pointer p) noexcept {
			first = p;
			last = p;
			reserved = addPointer(first, GetPagingCount());
		}
		inline void recall() noexcept {
			first = nullptr;
			last = nullptr;
			reserved = nullptr;
		}

	public:

		PagingMemory() noexcept = default;
		~PagingMemory() {
			for (auto rIt = memory.rbegin(), rend = memory.rbegin(); rIt != rend; ++rIt) {
				traits::deallocate(alloc, toPointer(rIt->first), GetPagingCount());
			}
		}

		NODISCARD pointer allocate(const size_type size) {
			assert(size <= GetPagingCount());

			if (first != nullptr) {
				const size_type capacity = std::distance(last, reserved);
				if (capacity < size) {
					recall();
				}
			}

			if (first == nullptr) {
				if (not unusedMemory.empty()) {
					deploy(unusedMemory.top());
					unusedMemory.pop();
				}
				else {
					pointer p = traits::allocate(alloc, GetPagingCount());
					memory.emplace_back(toInteger(p), 0);
					std::sort(memory.begin(), memory.end());
					deploy(p);
				}
			}

			pointer ret = last;
			last = addPointer(last, size);

			return ret;
		}

		template <class... Args>
		NODISCARD pointer create(Args&&... val) {
			return traits::construct(alloc, allocate(1), std::forward<Args>(val)...);
		}

		void release(const const_pointer data) {
			using pair = decltype(memory)::value_type;
			const uintptr_t ptr = toInteger(data) - GetPagingCount();
			const auto it = std::upper_bound(memory.begin(), memory.end(), pair(ptr, 0));

			assert(0 < it->second);
			it->second--;
			if (it->second <= 0) {
				if (it->first != toInteger(first)) {
					unusedMemory.push(toPointer(it->first));
				}
			}
		}
	};
}
