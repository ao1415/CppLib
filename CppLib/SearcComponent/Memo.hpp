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

		Memo() = default;
		Memo(const Memo&) = delete;
		Memo(Memo&&) = default;

		Memo& operator=(const Memo&) = delete;
		Memo& operator=(Memo&&) = default;

		void lock() noexcept { buffer.lock(); }
		void unlock() noexcept { buffer.unlock(); }

		/**
		 * @brief 指定されたデータを記録する
		 * @param data データポインタ
		 * @param size データサイズ
		*/
		void modify(void* data, const size_type size) noexcept {
			buffer.modify(data, size);
		}

		/**
		 * @brief 指定されたデータを記録する
		 * @tparam Class データ型
		 * @param data 変更前データ
		*/
		template<class Type>
		void modify(Type& data) noexcept {
			WARN_PUSH_DISABLE(26474);
			buffer.modify(reinterpret_cast<void*>(std::addressof(data)), sizeof(Type));
			WARN_POP();
		}

		/**
		 * @brief 差分パッチ作成
		 * @return 差分パッチ
		*/
		NODISCARD Patch commit() {
			const size_type wholeSize = buffer.wholeSize();
			pointer first = pool.allocate(wholeSize);
			const Stream buf(first, wholeSize);
			return buffer.commit(buf);
		}
		/**
		 * @brief パッチデータを解放する
		 * @param patch パッチ
		*/
		void release(const Patch& patch) {
			pool.release(patch.address());
		}
	};

	template<class Space>
	class MemoSingleton final {
	private:
		inline static Memo instance{};
	public:

		inline static Memo& Get() {
			return instance;
		}

		/**
		 * @brief 指定されたポインタを記録する
		 * @tparam Type データ型
		 * @param ptr データポインタ
		*/
		template<typename Type>
		inline static void Modify(Type& ptr) noexcept {
			instance.modify(ptr);
		}

		/**
		 * @brief 指定されたデータを記録する
		 * @tparam Type データ型
		 * @param ptr データポインタ
		 * @param o 変更値
		*/
		template<typename Type>
		inline static void Modify(Type& ptr, const Type& o) noexcept {
			if (ptr != o) {
				instance.modify(ptr);
				ptr = o;
			}
		}
	};

}
