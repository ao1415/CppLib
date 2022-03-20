#pragma once

#include <algorithm>
#include <utility>

#include <cassert>
#include <cstring>
#include <cstddef>

#include "SearchComponentLib.hpp"

namespace alib::Search::Lib {

	class Stream {
	public:
		using value_type = std::byte;
		using pointer = value_type*;
	private:
		pointer first = nullptr;
		pointer last = nullptr;
		pointer reserved = nullptr;

		inline void add(const size_type n) noexcept {
			WARN_PUSH_DISABLE(26481);
			last += n;
			WARN_POP();
		}
		inline void sub(const size_type n) noexcept {
			WARN_PUSH_DISABLE(26481);
			last -= n;
			WARN_POP();
		}
	public:
		Stream() = default;
		WARN_PUSH_DISABLE(26481);
		Stream(pointer data, size_type size) noexcept : first(data), last(data), reserved(data + size) {}
		WARN_POP();
		Stream(pointer data, pointer reserved) noexcept : first(data), last(data), reserved(reserved) {}

		template<typename Ptr>
		NODISCARD inline Ptr reinterpret() const noexcept {
			WARN_PUSH_DISABLE(26490);
			return reinterpret_cast<Ptr>(last);
			WARN_POP();
		};

		template<typename Type>
		NODISCARD inline Type get() noexcept {
			assert(sizeof(Type) <= capacity());
			Type ret = *(reinterpret<Type*>());
			add(sizeof(Type));
			return ret;
		}

		template<typename Type>
		NODISCARD inline Type pop() noexcept {
			assert(sizeof(Type) <= size());
			sub(sizeof(Type));
			return *(reinterpret<Type*>());
		}

		inline void pop(Stream& stream, const size_type length) noexcept {
			assert(length <= size());
			assert(length <= stream.capacity());
			sub(length);
			std::memcpy(stream.last, last, length);
			stream.add(length);
		}

		template<typename Type>
		inline void set(const Type value) noexcept {
			assert(sizeof(Type) <= capacity());
			*(reinterpret<Type*>()) = value;
			add(sizeof(Type));
		}

		inline void copy(void* ptr, const size_type length) noexcept {
			assert(length <= capacity());
			std::memcpy(last, ptr, length);
			add(length);
		}

		inline void seek(const size_type length) noexcept {
			assert(length <= capacity());
			add(length);
		}

		inline void write(void* ptr, const size_type length) noexcept {
			assert(length <= capacity());
			std::memcpy(ptr, last, length);
			add(length);
		}

		NODISCARD inline size_type size() const noexcept { return std::distance(first, last); }
		NODISCARD inline size_type capacity() const noexcept { return std::distance(last, reserved); }

		NODISCARD inline bool endOfStream() const noexcept { return 0 == capacity(); }
		NODISCARD inline bool hasStream() const noexcept { return 0 < capacity(); }

		NODISCARD inline std::pair<pointer, pointer> data() const noexcept { return { first,last }; }
	};

}