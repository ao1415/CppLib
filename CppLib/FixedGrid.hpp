#pragma once
#include <array>

#include "Basic.hpp"

namespace alib {

	/**
	 * @brief ŒÅ’è’·2ŽŸŒ³”z—ñ
	 * @tparam Type ”z—ñŒ^
	*/
	template<typename Type, size_t Width, size_t Height>
	class FixedGrid {
	public:
		inline static constexpr size_t Size = Width * Height;
		using base = std::array<Type, Size>;
		using value_type = Type;

		using iterator = typename base::iterator;
		using const_iterator = typename base::const_iterator;
		using reverse_iterator = typename base::reverse_iterator;
		using const_reverse_iterator = typename base::const_reverse_iterator;

	private:
		base c{};
	public:
		constexpr FixedGrid() = default;
		constexpr FixedGrid(const FixedGrid&) = default;
		constexpr FixedGrid(FixedGrid&&) = default;

		constexpr FixedGrid& operator=(const FixedGrid&) = default;
		constexpr FixedGrid& operator=(FixedGrid&&) = default;

		constexpr FixedGrid(const value_type& v) noexcept {
			for (auto& elem : c) {
				elem = v;
			}
		};

		NODISCARD constexpr const value_type& operator[](size_t n) const noexcept { return c[n]; }
		NODISCARD constexpr value_type& operator[](size_t n) noexcept { return c[n]; }

		NODISCARD constexpr const value_type& ay(size_t n) const { return c.at(n); }
		NODISCARD constexpr value_type& ay(size_t n) { return c.at(n); }

		NODISCARD constexpr const value_type& operator()(size_t x, size_t y) const noexcept { return c[y * Width + x]; }
		NODISCARD constexpr value_type& operator()(size_t x, size_t y) noexcept { return c[y * Width + x]; }

		NODISCARD constexpr const value_type& at(size_t x, size_t y) const { return c.at(y * Width + x); }
		NODISCARD constexpr value_type& at(size_t x, size_t y) { return c.at(y * Width + x); }

		NODISCARD constexpr const value_type* data() const noexcept { return c.data(); }
		NODISCARD constexpr value_type* data() noexcept { return c.data(); }

		NODISCARD constexpr iterator begin() noexcept { return c.begin(); }
		NODISCARD constexpr const_iterator begin() const noexcept { return c.begin(); }
		NODISCARD constexpr iterator end() noexcept { return c.end(); }
		NODISCARD constexpr const_iterator end() const noexcept { return c.end(); }
		NODISCARD constexpr reverse_iterator rbegin() noexcept { return c.rbegin(); }
		NODISCARD constexpr const_reverse_iterator rbegin() const noexcept { return c.rbegin(); }
		NODISCARD constexpr reverse_iterator rend() noexcept { return c.rend(); }
		NODISCARD constexpr const_reverse_iterator rend() const noexcept { return c.rend(); }
		NODISCARD constexpr const_iterator cbegin() const noexcept { return c.cbegin(); }
		NODISCARD constexpr const_iterator cend() const noexcept { return c.cend(); }
		NODISCARD constexpr const_reverse_iterator crbegin() const noexcept { return c.crbegin(); }
		NODISCARD constexpr const_reverse_iterator crend() const noexcept { return c.crend(); }

		NODISCARD constexpr size_t size() const noexcept { return Size; }
		NODISCARD constexpr size_t width() const noexcept { return Width; }
		NODISCARD constexpr size_t height() const noexcept { return Height; }

		NODISCARD constexpr bool inside(size_t x, size_t y) const noexcept { return (0 <= x && x < Width && 0 <= y && y < Height); }
		NODISCARD constexpr bool outside(size_t x, size_t y) const noexcept { return (0 > x || x >= Width || 0 > y || y >= Height); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr const value_type& operator()(const T p) const noexcept { return operator()(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr value_type& operator()(const T p) noexcept { return operator()(p.x, p.y); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr const value_type& at(const T p) const { return at(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr value_type& at(const T p) { return at(p.x, p.y); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr bool inside(const T p) const noexcept { return inside(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD constexpr bool outside(const T p) const noexcept { return outside(p.x, p.y); }

		constexpr bool operator==(const FixedGrid& o) const { return c == o.c; }
		constexpr bool operator!=(const FixedGrid& o) const { return c != o.c; }
		constexpr bool operator<(const FixedGrid& o) const { return c < o.c; }
		constexpr bool operator<=(const FixedGrid& o) const { return c <= o.c; }
		constexpr bool operator>(const FixedGrid& o) const { return c > o.c; }
		constexpr bool operator>=(const FixedGrid& o) const { return c >= o.c; }

		constexpr void swap(FixedGrid& o) noexcept(noexcept(o.swap(c))) { o.swap(c); }
	};

	template <class T, size_t W, size_t H>
	constexpr bool operator==(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator==(r); }
	template <class T, size_t W, size_t H>
	constexpr bool operator!=(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator!=(r); }
	template <class T, size_t W, size_t H>
	constexpr bool operator<(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator<(r); }
	template <class T, size_t W, size_t H>
	constexpr bool operator<=(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator<=(r); }
	template <class T, size_t W, size_t H>
	constexpr bool operator>(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator>(r); }
	template <class T, size_t W, size_t H>
	constexpr bool operator>=(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { return l.operator>=(r); }

	template <class T, size_t W, size_t H>
	constexpr void swap(const FixedGrid<T, W, H>& l, const FixedGrid<T, W, H>& r) { l.swap(r); }
}
