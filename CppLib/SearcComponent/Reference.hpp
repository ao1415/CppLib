#pragma once

#include <array>
#include <memory>
#include <utility>

#include "Memo.hpp"

namespace alib::Search::Lib {

	template<typename Type, class Space>
	class Reference {
	public:
		static_assert(std::is_trivially_copyable_v<Type>, "memcpy出来ない型です");
		static_assert(sizeof(Type) <= 16, "変数のサイズが大きすぎます");
		using value_type = Type;
		using pointer = value_type*;
	private:
		using IMemo = MemoSingleton<Space>;
		pointer ptr = nullptr;
	protected:
		Reference(const pointer ptr) noexcept : ptr(ptr) {}
	public:
		inline static Reference Ref(value_type& value) { return Reference(std::addressof(value)); }

		Reference& operator=(const Reference&) = delete;

		inline value_type operator=(const value_type value) {
			IMemo::Modify(ptr, value);
			return value;
		}
		NODISCARD inline operator value_type() const noexcept { return *ptr; }

		inline void store(const value_type value) { IMemo::Modify(ptr, value); }
		NODISCARD inline value_type load() const noexcept { return *ptr; }
	};


	template<typename Type, class Space>
	class Value : public Reference<Type, Space> {
	public:
		using base_type = Reference<Type, Space>;
		using value_type = typename base_type::value_type;
	private:
		using IMemo = MemoSingleton<Space>;
		value_type val{};
	public:
		using Reference<Type, Space>::operator=;

		Value() noexcept : Reference<Type, Space>(&val), val(value_type()) {}
		Value(const value_type val) noexcept : Reference<Type, Space>(&val), val(val) {}
	};

	template<typename Type, size_type Size, class Space>
	class Array {
	public:
		using IMemo = MemoSingleton<Space>;
		using Ref = Reference<Type, Space>;
		using container_type = std::array<Type, Size>;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;
	private:
		container_type elems;
	public:
		Array() noexcept {};
		Array(const Array& other) = default;
		Array(Array&& other) = default;
		Array& operator=(const Array& other) = default;
		Array& operator=(Array&& other) = default;

		inline void store(const size_type idx, const Type& o) noexcept { IMemo::Modify(elems[idx]); elems[idx] = o; }
		NODISCARD inline const Type& load(const size_type idx) const noexcept { return elems[idx]; }

		NODISCARD inline const Type& operator[](const size_type idx) const noexcept { return elems[idx]; }
		NODISCARD inline Ref operator[](const size_type idx) noexcept { return Ref::Ref(elems[idx]); }

		NODISCARD inline const Type& at(const size_type idx) const { return elems.at(idx); }
		NODISCARD inline Ref at(const size_type idx) { return Ref::Ref(elems.at(idx)); }

		NODISCARD inline size_type size() const noexcept(noexcept(c.size())) { return c.size(); }
		NODISCARD inline const_iterator begin() const noexcept { return c.begin(); }
		NODISCARD inline const_iterator end() const noexcept { return c.end(); }
		NODISCARD inline const_iterator cbegin() const noexcept { return c.cbegin(); }
		NODISCARD inline const_iterator cend() const noexcept { return c.cend(); }
		NODISCARD inline const_reverse_iterator rbegin() const noexcept { return c.rbegin(); }
		NODISCARD inline const_reverse_iterator rend() const noexcept { return c.rend(); }
		NODISCARD inline const_reverse_iterator crbegin() const noexcept { return c.crbegin(); }
		NODISCARD inline const_reverse_iterator crend() const noexcept { return c.crend(); }
	};
	template<typename Type, size_type Size, class Space>
	class Vector : public Array<Type, Size, Space> {
	public:
		using IMemo = MemoSingleton<Space>;
		using Ref = Reference<Type, Space>;
		using base = Array<Type, Size, Space>;
		using container_type = typename base::container_type;
	private:
		size_type count = 0;

		inline void hasCapacity(const size_type n) noexcept { assert(0 < n && n <= Size); }
		inline void inside(const size_type idx) noexcept { assert(0 <= idx && idx < count); }
		inline void any() noexcept { assert(0 < count); }
	public:
		Vector() noexcept : base() {};
		Vector(const Vector& other) = default;
		Vector(Vector&& other) = default;
		Vector& operator=(const Vector& other) = default;
		Vector& operator=(Vector&& other) = default;

		inline void set(const size_type idx, const Type& o) noexcept {
			inside(idx);
			IMemo::Modify(base::operator[](idx));
			base::operator[](idx) = o;
		}
		NODISCARD inline const Type& get(const size_type idx) const noexcept {
			inside(idx);
			return base::operator[](idx);
		}

		inline void clear() noexcept { IMemo::Modify(count); count = 0; }
		NODISCARD inline bool empty() const noexcept { return count == 0; }
		NODISCARD inline bool full() const noexcept { return count == base::size(); }
		NODISCARD inline size_type full_size() const noexcept { return base::size(); }
		NODISCARD inline size_type size() const noexcept { return count; }

		inline void push_back(const Type& value) noexcept {
			hasCapacity(count + 1);
			IMemo::Modify(count);
			WARN_PUSH_DISABLE(26446);
			IMemo::Modify(base::operator[](count));
			base::operator[](count) = value;
			WARN_POP();
			++count;
		}
		template<class ...Args>
		inline void emplace_back(Args&& ...args) noexcept {
			hasCapacity(count + 1);
			IMemo::Modify(count);
			IMemo::Modify(base::operator[](count));
			new(std::addressof(base::operator[](count))) Type(std::forward<Args>(args)...);
			++count;
		}

		inline void pop_back() noexcept { any(); IMemo::Modify(count); --count; }

		WARN_PUSH_DISABLE(26434 26446);
		NODISCARD inline Ref front() noexcept { any(); return Ref::Ref(base::front()); }
		NODISCARD inline const Type& front() const noexcept { any(); return base::front(); }

		NODISCARD inline Ref back() noexcept { any(); return Ref::Ref(base::operator[](count - 1)); }
		NODISCARD inline const Type& back() const noexcept { any(); return base::operator[](count - 1); }
		WARN_POP();

		inline void resize(const size_type n, const Type& value) noexcept {
			hasCapacity(n);
			IMemo::Modify(count);
			if (count < n) {
				const auto addCount = n - count;
				IMemo::Modify(std::addressof(base::operator[](count)), sizeof(Type) * addCount);
				forstep(idx, count, n) { base::operator[](idx) = value; }
			}
			count = n;
		}
		inline void resize(int n) noexcept {
			hasCapacity(n);
			IMemo::Modify(count);
			count = n;
		}

		NODISCARD inline const Type& operator[](const size_type idx) const noexcept { inside(idx); return base::operator[](idx); }
		NODISCARD inline Ref operator[](const size_type idx) noexcept { inside(idx); return Ref::Ref(base::operator[](idx)); }

		NODISCARD inline const Type& at(const size_type idx) const { inside(idx); return base::at(idx); }
		NODISCARD inline Ref at(const size_type idx) { inside(idx); return Ref::Ref(base::at(idx)); }
	};

	template<class Class>
	struct MemoType {
		template<typename Type>
		using Value = Value<Type, Class>;
		template<typename Type, size_type Size>
		using Array = Array<Type, Size, Class>;
		template<typename Type, size_type Size>
		using Vector = Vector<Type, Size, Class>;
	};

}
