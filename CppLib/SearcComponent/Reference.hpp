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
			Reference& operator=(const Reference& o) {
				store(o.load());
				return *this;
			}
			Reference& operator=(Reference&&) = delete;

			value_type operator=(const value_type value) {
				IMemo::Modify(*ptr, value);
				return value;
			}
			NODISCARD operator value_type() const noexcept { return *ptr; }

			void store(const value_type value) noexcept { IMemo::Modify(*ptr, value); }
			NODISCARD value_type load() const noexcept { return *ptr; }
		};


		template<typename Type, class Space>
		class Value {
		public:
			using value_type = Type;
		private:
			using IMemo = MemoSingleton<Space>;
			value_type val{};
		public:
			Value(const Value& o) {
				store(o.load());
			}
			Value(Value&&) = default;
			Value& operator=(const Value& o) {
				store(o.load());
				return *this;
			}
			Value& operator=(Value&&) = delete;

			Value() noexcept : val(value_type()) {}
			explicit Value(const value_type v) noexcept : val(v) {}

			value_type operator=(const value_type value) {
				IMemo::Modify(val, value);
				return value;
			}
			NODISCARD operator value_type() const noexcept { return val; }

			void store(const value_type value) noexcept { IMemo::Modify(val, value); }
			NODISCARD value_type load() const noexcept { return val; }
		};

		template <class T>
		class has_iterator {
			template <class U>
			static constexpr std::true_type check(typename U::iterator*) {
				return std::true_type{};
			};

			template <class U>
			static constexpr std::false_type check(...) {
				return std::false_type{};
			}
		public:
			static constexpr bool value = decltype(check<T>(nullptr))::value;
		};

		template<typename Type, size_type Size, class Space>
		class Vector {
		public:
			using value_type = Type;
			using container_type = std::array<value_type, Size>;

			using iterator = typename container_type::iterator;
			using const_iterator = typename container_type::const_iterator;
			using reverse_iterator = typename container_type::reverse_iterator;
			using const_reverse_iterator = typename container_type::const_reverse_iterator;

		private:
			using IMemo = MemoSingleton<Space>;
			Value<size_type, Space> count{};
			container_type c{};

			bool hasCapacity(const size_type n) const noexcept {
				return (0 < n && n <= Size);
			}
			bool inside(const size_type index) const noexcept {
				return (0 <= index && index < count);
			}

			void assertCapacity(const size_type n) const noexcept {
				assert(hasCapacity(n));
			}
			void assertInside(const size_type index) const noexcept {
				assert(inside(index));
			}
			void assertAny() const noexcept {
				assert(0 < count);
			}

			template <class Iter>
			void setIter(Iter first, Iter last) noexcept {
				int i = 0;
				for (auto it = first; it != last; ++it) {
					c[i] = *it;
					++i;
				}
			}
		public:
			Vector() = default;
			Vector(const Vector&) = default;
			Vector(Vector&&) = default;
			Vector& operator=(const Vector&) = default;
			Vector& operator=(Vector&&) = delete;

			explicit Vector(const size_type n) noexcept {
				assertCapacity(n);
				count.store(n);
			}
			Vector(const size_type n, const value_type v) noexcept {
				assertCapacity(n);
				count.store(n);
				forange(i, count.load()) {
					c[i] = v;
				}
			}
			template <class Iter>
			Vector(Iter first, Iter last) noexcept {
				const size_type n = last - first;
				assertCapacity(n);
				count.store(n);
				setIter(first, last);
			}

			Vector(std::initializer_list<value_type> il) noexcept {
				const size_type n = il.size();
				assertCapacity(n);
				count.store(n);
				setIter(il.begin(), il.end());
			}

			NODISCARD iterator begin() noexcept { return c.begin(); }
			NODISCARD iterator end() noexcept { return c.begin() += count.load(); }
			NODISCARD const_iterator begin() const noexcept { return c.begin(); }
			NODISCARD const_iterator end() const noexcept { return c.begin() += count.load(); }
			NODISCARD const_iterator cbegin() const noexcept { return c.cbegin(); }
			NODISCARD const_iterator cend() const noexcept { return c.cbegin() += count.load(); }
			NODISCARD reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
			NODISCARD reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
			NODISCARD const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
			NODISCARD const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
			NODISCARD const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
			NODISCARD const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

			NODISCARD size_type size() const noexcept { return count; }
			NODISCARD size_type max_size() const noexcept { return Size; }

			void resize(const size_type n, const Type& v) noexcept {
				assertCapacity(n);
				if (count.load() < n) {
					const auto addCount = n - count.load();
					forstep(i, count.load(), n) {
						c[i] = v;
					}
				}
				count.store(n);
			}
			void resize(int n) noexcept {
				assertCapacity(n);
				count.store(n);
			}

			NODISCARD size_type capacity() const noexcept { return Size; }
			NODISCARD bool empty() const noexcept { return count.load() == 0; }

			NODISCARD value_type& operator[](const size_type index) noexcept {
				assertInside(index);
				return c[index];
			}
			NODISCARD const value_type& operator[](const size_type index) const noexcept {
				assertInside(index);
				return c[index];
			}
			NODISCARD value_type& at(const size_type index) {
				if (not inside(index)) {
					throw std::out_of_range("invalid vector subscript");
				}
				return c[index];
			}
			NODISCARD const value_type& at(const size_type index) const {
				if (not inside(index)) {
					throw std::out_of_range("invalid vector subscript");
				}
				return c[index];
			}

			NODISCARD value_type& front() noexcept {
				assertAny();
				return c[0];
			}
			NODISCARD const value_type& front() const noexcept {
				assertAny();
				return c[0];
			}

			NODISCARD value_type& back() noexcept {
				assertAny();
				return c[count - 1];
			}
			NODISCARD const value_type& back() const noexcept {
				assertAny();
				return c[count - 1];
			}

			void assign(const size_type n, const value_type& v) noexcept {
				assertCapacity(n);
				if (count.load() < n) {
					count.store(n);
				}
				forange(i, n) {
					c[i] = v;
				}
			}
			template <class Iter>
			void assign(Iter first, Iter last) noexcept {
				const size_type n = last - first;
				assertCapacity(n);
				if (count.load() < n) {
					count.store(n);
				}
				setIter(first, last);
			}
			void assign(std::initializer_list<value_type> il) noexcept {
				const size_type n = il.size();
				assertCapacity(n);
				if (count.load() < n) {
					count.store(n);
				}
				setIter(il.begin(), il.end());
			}

			void push_back(const Type& v) noexcept {
				const size_type n = count.load() + 1;
				assertCapacity(n);
				count.store(n);
				c[n] = v;
			}

			template<class ...Args>
			value_type& emplace_back(Args&& ...args) noexcept {
				const size_type n = count.load() + 1;
				assertCapacity(n);
				count.store(n);
				c[n] = Type(std::forward<Args>(args)...);
				return c[n];
			}

			void pop_back() noexcept {
				assertAny();
				count.store(count.load() - 1);
			}

			void clear() noexcept {
				count.store(0);
			}
		};

	}
	template<class Class>
	struct MemoType {
		template<typename Type>
		using Value = Lib::Value<Type, Class>;
		template<typename Type, size_type Size>
		using Array = std::array<Type, Size>;
		template<typename Type, size_type Size>
		using Vector = Lib::Vector<Type, Size, Class>;
	};
}
