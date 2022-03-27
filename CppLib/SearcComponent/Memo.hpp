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

		/**
		 * @brief �w�肳�ꂽ�f�[�^���L�^����
		 * @param data �f�[�^�|�C���^
		 * @param size �f�[�^�T�C�Y
		*/
		inline void modify(void* data, const size_type size) noexcept {
			buffer.modify(data, size);
		}

		/**
		 * @brief �w�肳�ꂽ�f�[�^���L�^����
		 * @tparam Class �f�[�^�^
		 * @param data �ύX�O�f�[�^
		*/
		template<class Class>
		inline void modify(Class& data) noexcept {
			WARN_PUSH_DISABLE(26474);
			buffer.modify(reinterpret_cast<void*>(std::addressof(data)), sizeof(Class));
			WARN_POP();
		}

		/**
		 * @brief �����p�b�`�쐬
		 * @return �����p�b�`
		*/
		NODISCARD inline Patch commit() {
			const size_type wholeSize = buffer.wholeSize();
			pointer first = pool.allocate(wholeSize);
			const Stream buf(first, wholeSize);
			return buffer.commit(buf);
		}
		/**
		 * @brief �p�b�`�f�[�^���������
		 * @param patch �p�b�`
		*/
		inline void release(const Patch& patch) {
			pool.release(patch.address());
		}
	};

	template<class Space>
	struct MemoSingleton {
		inline static Memo memo;

		inline static void Lock() noexcept { memo.lock(); }
		inline static void Unlock() noexcept { memo.unlock(); }

		/**
		 * @brief �w�肳�ꂽ�|�C���^���L�^����
		 * @tparam Type �f�[�^�^
		 * @param ptr �f�[�^�|�C���^
		*/
		template<typename Type>
		inline static void Modify(Type* ptr) noexcept {
			memo.modify(*ptr);
		}

		/**
		 * @brief �w�肳�ꂽ�f�[�^���L�^����
		 * @tparam Type �f�[�^�^
		 * @param ptr �f�[�^�|�C���^
		 * @param o �ύX�l
		*/
		template<typename Type>
		inline static void Modify(Type* ptr, const Type& o) noexcept {
			if (*ptr != o) {
				Modify(ptr);
				(*ptr) = o;
			}
		}
	};

}
