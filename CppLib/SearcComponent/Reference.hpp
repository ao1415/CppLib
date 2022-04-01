#pragma once

#include <array>
#include <iterator>
#include <memory>
#include <utility>

#include "Memo.hpp"

namespace alib::Search {
	namespace Lib {

		template<typename Type, class Space>
		class Reference {
		public:
			using value_type = Type;
			using pointer = value_type*;
			static_assert(std::is_trivially_copyable_v<value_type>, "memcpy出来ない型です");
			static_assert(sizeof(value_type) <= 16, "変数のサイズが大きすぎます");
		private:
			using IMemo = MemoSingleton<Space>;
			pointer ptr = nullptr;
		protected:
			Reference(const pointer ptr) noexcept : ptr(ptr) {}
		public:
			inline static Reference Ref(value_type& value) {
				return Reference(std::addressof(value));
			}

			Reference(const Reference&) = delete;
			Reference(Reference&&) = default;
			Reference& operator=(const Reference&) = delete;
			Reference& operator=(Reference&&) = delete;

			value_type operator=(const value_type value) {
				IMemo::Modify(*ptr, value);
				return value;
			}
			NODISCARD operator value_type() const noexcept { return *ptr; }

			void store(const value_type value) { IMemo::Modify(*ptr, value); }
			NODISCARD value_type load() const noexcept { return *ptr; }
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
			Value(const value_type v) noexcept : Reference<Type, Space>(std::addressof(val)), val(v) {}
		};

		template<typename Type, size_type Size, class Space>
		class ArrayIterator {
		public:
			using Ref = Reference<Type, Space>;

			using iterator_category = std::random_access_iterator_tag;
			using value_type = Type;
			using difference_type = ptrdiff_t;
			using pointer = Type*;
			using reference = Ref;
		private:
			pointer ptr;
		public:

			ArrayIterator() = default;
			explicit ArrayIterator(pointer p, size_type offset = 0) noexcept : ptr(p + offset) {}

			NODISCARD reference operator*() const noexcept {
				return Ref::Ref(*ptr);
			}

			ArrayIterator& operator++() noexcept {
				++ptr;
				return *this;
			}

			ArrayIterator operator++(int) noexcept {
				ArrayIterator tmp = *this;
				++ptr;
				return tmp;
			}

			ArrayIterator& operator--() noexcept {
				--ptr;
				return *this;
			}

			ArrayIterator operator--(int) noexcept {
				ArrayIterator tmp = *this;
				--ptr;
				return tmp;
			}

			ArrayIterator& operator+=(const ptrdiff_t diff) noexcept {
				ptr += diff;
				return *this;
			}

			NODISCARD ArrayIterator operator+(const ptrdiff_t diff) const noexcept {
				ArrayIterator tmp = *this;
				tmp += diff;
				return tmp;
			}

			ArrayIterator& operator-=(const ptrdiff_t diff) noexcept {
				ptr -= diff;
				return *this;
			}

			NODISCARD ptrdiff_t operator-(const ArrayIterator& right) const noexcept {
				return ptr - right.ptr;
			}

			NODISCARD ArrayIterator operator-(const ptrdiff_t diff) const noexcept {
				ArrayIterator tmp = *this;
				tmp -= diff;
				return tmp;
			}

			NODISCARD Ref operator[](const ptrdiff_t idx) const noexcept {
				return Ref::Ref(ptr[idx]);
			}

			NODISCARD bool operator==(const ArrayIterator& right) const noexcept {
				return ptr == right.ptr;
			}
			NODISCARD bool operator!=(const ArrayIterator& right) const noexcept {
				return !(*this == right);
			}
			NODISCARD bool operator<(const ArrayIterator& right) const noexcept {
				return ptr < right.ptr;
			}
			NODISCARD bool operator>(const ArrayIterator& right) const noexcept {
				return right < *this;
			}
			NODISCARD bool operator<=(const ArrayIterator& right) const noexcept {
				return !(right < *this);
			}
			NODISCARD bool operator>=(const ArrayIterator& right) const noexcept {
				return !(*this < right);
			}
		};

		template <class Type, size_type Size, class Space>
		NODISCARD ArrayIterator<Type, Size, Space> operator+(const ptrdiff_t diff, ArrayIterator<Type, Size, Space> next) noexcept {
			next += diff;
			return next;
		}

		template<typename Type, size_type Size, class Space>
		class Array {
		public:
			using value_type = Type;

			using Ref = Reference<value_type, Space>;

			using container_type = std::array<value_type, Size>;
			using iterator = ArrayIterator<value_type, Size, Space>;
			using const_iterator = typename container_type::const_iterator;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = typename container_type::const_reverse_iterator;
		private:
			using IMemo = MemoSingleton<Space>;
			container_type elems;
		public:
			Array() noexcept {};
			Array(const Array&) = delete;
			Array(Array&&) = default;
			Array& operator=(const Array&) = delete;
			Array& operator=(Array&&) = delete;

			Array(std::initializer_list<value_type> ilist) {
				assert(ilist.size() == Size);
				auto it = ilist.begin();
				for (auto& v : elems) {
					v = *it;
					++it;
				}
			}

			NODISCARD const value_type& load(const size_type idx) const noexcept { return elems[idx]; }
			void store(const size_type idx, const value_type& o) noexcept { IMemo::Modify(elems[idx], o); }

			NODISCARD const value_type& operator[](const size_type idx) const noexcept { return elems[idx]; }
			NODISCARD Ref operator[](const size_type idx) noexcept { return Ref::Ref(elems[idx]); }

			NODISCARD const value_type& at(const size_type idx) const { return elems.at(idx); }
			NODISCARD Ref at(const size_type idx) { return Ref::Ref(elems.at(idx)); }

			NODISCARD size_type size() const noexcept { return elems.size(); }
			void fill(const value_type& value) {
				for (auto& v : elems) {
					IMemo::Modify(v, value);
				}
			}

			NODISCARD const value_type& front() const noexcept { return elems.front(); }
			NODISCARD Ref front() noexcept { return Ref::Ref(elems.front()); }
			NODISCARD const value_type& back() const noexcept { return elems.back(); }
			NODISCARD Ref back() noexcept { return Ref::Ref(elems.back()); }

			NODISCARD const size_type* data() const noexcept { return elems.data(); }
			NODISCARD const Array& as_const() const noexcept { return *this; }

			NODISCARD iterator begin() noexcept { return iterator(elems.data(), 0); }
			NODISCARD iterator end() noexcept { return iterator(elems.data(), Size); }
			NODISCARD const_iterator begin() const noexcept { return elems.begin(); }
			NODISCARD const_iterator end() const noexcept { return elems.end(); }
			NODISCARD const_iterator cbegin() const noexcept { return elems.cbegin(); }
			NODISCARD const_iterator cend() const noexcept { return elems.cend(); }

			NODISCARD reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
			NODISCARD reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
			NODISCARD const_reverse_iterator rbegin() const noexcept { return elems.rbegin(); }
			NODISCARD const_reverse_iterator rend() const noexcept { return elems.rend(); }
			NODISCARD const_reverse_iterator crbegin() const noexcept { return elems.crbegin(); }
			NODISCARD const_reverse_iterator crend() const noexcept { return elems.crend(); }
		};

		template<typename Type, size_type Size, class Space>
		class Vector : public Array<Type, Size, Space> {
		public:
			using Ref = Reference<Type, Space>;
			using base = Array<Type, Size, Space>;
			using container_type = typename base::container_type;
		private:
			using IMemo = MemoSingleton<Space>;
			size_type count = 0;

			void hasCapacity(const size_type n) noexcept { assert(0 < n && n <= Size); }
			void inside(const size_type idx) noexcept { assert(0 <= idx && idx < count); }
			void any() noexcept { assert(0 < count); }
		public:
			Vector() noexcept : base() {};
			Vector(const Vector& other) = default;
			Vector(Vector&& other) = default;
			Vector& operator=(const Vector& other) = default;
			Vector& operator=(Vector&& other) = default;

			void set(const size_type idx, const Type& o) noexcept {
				inside(idx);
				IMemo::Modify(base::operator[](idx));
				base::operator[](idx) = o;
			}
			NODISCARD const Type& get(const size_type idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}

			void clear() noexcept { IMemo::Modify(count); count = 0; }
			NODISCARD inline bool empty() const noexcept { return count == 0; }
			NODISCARD inline bool full() const noexcept { return count == base::size(); }
			NODISCARD inline size_type full_size() const noexcept { return base::size(); }
			NODISCARD inline size_type size() const noexcept { return count; }

			void push_back(const Type& value) noexcept {
				hasCapacity(count + 1);
				IMemo::Modify(count);
				WARN_PUSH_DISABLE(26446);
				IMemo::Modify(base::operator[](count));
				base::operator[](count) = value;
				WARN_POP();
				++count;
			}
			template<class ...Args>
			void emplace_back(Args&& ...args) noexcept {
				hasCapacity(count + 1);
				IMemo::Modify(count);
				IMemo::Modify(base::operator[](count));
				new(std::addressof(base::operator[](count))) Type(std::forward<Args>(args)...);
				++count;
			}

			void pop_back() noexcept { any(); IMemo::Modify(count); --count; }

			WARN_PUSH_DISABLE(26434 26446);
			NODISCARD Ref front() noexcept { any(); return Ref::Ref(base::front()); }
			NODISCARD const Type& front() const noexcept { any(); return base::front(); }

			NODISCARD Ref back() noexcept { any(); return Ref::Ref(base::operator[](count - 1)); }
			NODISCARD const Type& back() const noexcept { any(); return base::operator[](count - 1); }
			WARN_POP();

			void resize(const size_type n, const Type& value) noexcept {
				hasCapacity(n);
				IMemo::Modify(count);
				if (count < n) {
					const auto addCount = n - count;
					IMemo::Modify(std::addressof(base::operator[](count)), sizeof(Type) * addCount);
					forstep(idx, count, n) { base::operator[](idx) = value; }
				}
				count = n;
			}
			void resize(int n) noexcept {
				hasCapacity(n);
				IMemo::Modify(count);
				count = n;
			}

			NODISCARD const Type& operator[](const size_type idx) const noexcept { inside(idx); return base::operator[](idx); }
			NODISCARD Ref operator[](const size_type idx) noexcept { inside(idx); return Ref::Ref(base::operator[](idx)); }

			NODISCARD const Type& at(const size_type idx) const { inside(idx); return base::at(idx); }
			NODISCARD Ref at(const size_type idx) { inside(idx); return Ref::Ref(base::at(idx)); }
		};
	}
	template<class Class>
	struct MemoType {
		template<typename Type>
		using Value = Lib::Value<Type, Class>;
		template<typename Type, size_type Size>
		using Array = Lib::Array<Type, Size, Class>;
		template<typename Type, size_type Size>
		using Vector = Lib::Vector<Type, Size, Class>;
	};
}
