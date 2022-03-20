#pragma once

#include <memory>
#include <cstddef>

#include "SearchComponentLib.hpp"
#include "Buffer.hpp"
#include "Stream.hpp"
#include "Patch.hpp"
#include "PagingMemory.hpp"

namespace alib::Search::Lib {

	class Memo {
	public:
		using value_type = Stream::value_type;
		using pointer = Stream::pointer;

		using MemoSizeType = unsigned short;
		using Patch = Patch<MemoSizeType>;
	private:
		Buffer<16_K, MemoSizeType> buffer{};
		PagingMemory<std::byte, 1_M> pool;
	public:
		inline void lock() noexcept { buffer.lock(); }
		inline void unlock() noexcept { buffer.unlock(); }

		inline void modify(void* data, const size_type size) noexcept {
			buffer.modify(data, size);
		}

		template<class Class>
		inline void modify(Class& data) noexcept {
			WARN_PUSH_DISABLE(26474);
			buffer.modify(reinterpret_cast<void*>(std::addressof(data)), sizeof(Class));
			WARN_POP();
		}

		/** @brief ç∑ï™ÉpÉbÉ`çÏê¨ */
		NODISCARD inline Patch commit() {
			const size_type wholeSize = buffer.wholeSize();
			pointer first = pool.allocate(wholeSize);
			const Stream buf(first, wholeSize);
			return buffer.commit(buf);
		}
		inline void release(const Patch& patch) {
			pool.release(patch.address());
		}
	};

	template<class Space>
	struct MemoSingleton {
		inline static Memo memo;

		inline static void Lock() noexcept { memo.lock(); }
		inline static void Unlock() noexcept { memo.unlock(); }

		template<typename Type>
		inline static void Modify(Type* ptr) noexcept {
			memo.modify(*ptr);
		}

		template<typename Type>
		inline static void Modify(Type* ptr, const Type& o) noexcept {
			if (*ptr != o) {
				Modify(ptr);
				(*ptr) = o;
			}
		}
	};

}
