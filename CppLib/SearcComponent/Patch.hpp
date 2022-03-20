#pragma once

#include "Stream.hpp"

namespace alib::Search::Lib {

	template<typename MemoSize>
	class Patch {
	public:
		using value_type = Stream::value_type;
		using pointer = Stream::pointer;

		using MemoSizeType = MemoSize;
	private:
		pointer first = nullptr;
		pointer last = nullptr;
	public:
		Patch() = default;
		Patch(pointer first, pointer last) noexcept : first(first), last(last) {}

		NODISCARD inline bool hasValue() const noexcept { return first != last; }
		NODISCARD inline pointer address() const noexcept { return first; }

		/** @brief –ß‚· */
		inline void undo() const noexcept {
			assert(first != nullptr);

			Stream stream(first, last);
			while (stream.hasStream()) {
				const auto ptr = stream.get<void*>();
				const auto size = stream.get<MemoSizeType>();
				stream.seek(size);
				stream.write(ptr, size);
			}
			assert(stream.endOfStream());
		}
		/** @brief “K‰ž‚·‚é */
		inline void redo() const noexcept {
			assert(first != nullptr);

			Stream stream(first, last);
			while (stream.hasStream()) {
				const auto ptr = stream.get<void*>();
				const auto size = stream.get<MemoSizeType>();
				stream.write(ptr, size);
				stream.seek(size);
			}
			assert(stream.endOfStream());
		}
	};

}