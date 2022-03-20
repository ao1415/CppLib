#pragma once

#include <limits>

#include "SearchComponentLib.hpp"
#include "Patch.hpp"
#include "Stream.hpp"

namespace alib::Search::Lib {

	template<size_type Count, typename MemoSize = unsigned short>
	class Buffer {
	public:
		using value_type = Stream::value_type;
		using pointer = Stream::pointer;

		using MemoSizeType = MemoSize;
	private:
		Stream stream{};
		value_type buffer[Count]{};
		bool isLocked = false;
		size_type modiftSize = 0;
	public:
		Buffer() noexcept {
			WARN_PUSH_DISABLE(26485);
			stream = Stream(buffer, Count);
			WARN_POP();
		}

		inline void lock() noexcept { isLocked = true; }
		inline void unlock() noexcept { isLocked = false; }

		/** @brief 変更蓄積 */
		inline void modify(void* data, const size_type size) noexcept {
			if (not isLocked) { return; }

			const size_type writeSize = size + sizeof(MemoSizeType) + sizeof(void*);

			assert(0 < size);
			assert(size < std::numeric_limits<MemoSizeType>::max() - (sizeof(MemoSizeType) + sizeof(void*)));
			assert(writeSize <= stream.capacity());

			stream.copy(data, size);
			stream.set<MemoSizeType>(narrow_cast<MemoSizeType>(size));
			stream.set<void*>(data);

			modiftSize += size;
		}

		inline size_type wholeSize() const noexcept { return stream.size() + modiftSize; }
		inline Patch<MemoSizeType> commit(Stream buf) noexcept {
			assert(isLocked);

			while (buf.hasStream()) {
				const auto ptr = stream.pop<void*>();
				buf.set<void*>(ptr);
				const auto size = stream.pop<MemoSizeType>();
				buf.set<MemoSizeType>(size);
				buf.copy(ptr, size);// 変更後の値
				stream.pop(buf, size);// 変更前の値
			}
			assert(buf.endOfStream());
			modiftSize = 0;

			auto [first, last] = stream.data();
			return Patch<MemoSizeType>(first, last);
		}
	};

}
