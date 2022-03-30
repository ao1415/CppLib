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

		using MemoryData = std::pair<uintptr_t, size_type>;

		static_assert(std::is_same_v<value_type, traits::value_type>);
		static_assert(std::is_trivially_copyable_v<value_type>);
	private:

		NODISCARD inline static constexpr size_type GetPagingCount() noexcept {
			return Count;
		}

		std::stack<pointer> unusedMemory;
		std::vector<MemoryData> memory;

		pointer first = nullptr;
		pointer last = nullptr;
		pointer reserved = nullptr;

		std::vector<MemoryData>::iterator used;

		allocator_type alloc;

		NODISCARD uintptr_t toInteger(const_pointer ptr) const noexcept {
			WARN_PUSH_DISABLE(26490);
			return reinterpret_cast<uintptr_t>(ptr);
			WARN_POP();
		}
		NODISCARD pointer toPointer(const uintptr_t ptr) const noexcept {
			WARN_PUSH_DISABLE(26490);
			return reinterpret_cast<pointer>(ptr);
			WARN_POP();
		}
		NODISCARD pointer addPointer(const pointer ptr, const size_type size) const noexcept {
			WARN_PUSH_DISABLE(26481);
			return ptr + size;
			WARN_POP();
		}

		NODISCARD std::vector<MemoryData>::iterator findMemory(const_pointer ptr) {
			uintptr_t p = toInteger(ptr) - (GetPagingCount() * sizeof(value_type));
			return std::upper_bound(memory.begin(), memory.end(), MemoryData(p, 0));;
		}

		void deploy(pointer p) noexcept {
			first = p;
			last = p;
			reserved = addPointer(first, GetPagingCount());

			used = findMemory(p);
		}
		void recall() noexcept {
			first = nullptr;
			last = nullptr;
			reserved = nullptr;
		}

	public:

		PagingMemory() = default;
		~PagingMemory() {
			for (auto rit = memory.rbegin(), rend = memory.rbegin(); rit != rend; ++rit) {
				traits::deallocate(alloc, toPointer(rit->first), GetPagingCount());
			}
		}

		/**
		 * @brief 指定されたサイズを確保する
		 * @param size 確保サイズ
		 * @return 確保したポインタ
		*/
		NODISCARD pointer allocate(const size_type size) {
			assert(size <= GetPagingCount());

			if (first != nullptr) {
				const size_type capacity = std::distance(last, reserved);
				if (capacity < size) {
					if (0 < used->second) {
						recall();
					}
					else {
						deploy(first);
					}
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
			used->second++;

			return ret;
		}

		/**
		 * @brief 指定されたインスタンスを作成する
		 * @param val コンストラクタ引数
		*/
		template <class... Args>
		NODISCARD pointer create(Args&&... val) {
			return new(allocate(1)) value_type(std::forward<Args>(val)...);
			//return traits::construct(alloc, allocate(1), std::forward<Args>(val)...);
		}

		/**
		 * @brief 確保したポインタを解放する
		 * @param data
		*/
		void release(const_pointer data) {
			const auto it = findMemory(data);

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
