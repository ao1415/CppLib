#pragma once
#include <array>
#include "Basic.hpp"

namespace alib {

	/**
	 * @brief ŒÅ’è’·2ŽŸŒ³”z—ñ
	 * @tparam Type ”z—ñŒ^
	*/
	template<typename Type, size_t Width, size_t Height>
	class FixedGrid : public std::array<Type, Width* Height>{
	public:

		using base = std::array<Type, Width* Height>;
		using value_type = typename Type;
		using base::at;

		constexpr FixedGrid() noexcept : base() {};
		constexpr FixedGrid(const FixedGrid& other) = default;
		constexpr FixedGrid(FixedGrid&& other) = default;

		FixedGrid(const value_type& v) noexcept { base::fill(v); };

		constexpr FixedGrid& operator=(const FixedGrid& other) = default;
		constexpr FixedGrid& operator=(FixedGrid&& other) = default;

		NODISCARD inline constexpr const value_type& operator()(size_t x, size_t y) const { return base::operator[](y* Width + x); }
		NODISCARD inline constexpr value_type& operator()(size_t x, size_t y) { return base::operator[](y* Width + x); }

		NODISCARD inline constexpr const value_type& at(size_t x, size_t y) const { return base::at(y * Width + x); }
		NODISCARD inline constexpr value_type& at(size_t x, size_t y) { return base::at(y * Width + x); }

		NODISCARD inline constexpr size_t width() const noexcept { return Width; }
		NODISCARD inline constexpr size_t height() const noexcept { return Height; }

		NODISCARD inline constexpr bool inside(size_t x, size_t y) const noexcept { return (0 <= x && x < Width && 0 <= y && y < Height); }
		NODISCARD inline constexpr bool outside(size_t x, size_t y) const noexcept { return (0 > x || x >= Width || 0 > y || y >= Height); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr const value_type& operator()(const T p) const { return operator()(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr value_type& operator()(const T p) { return operator()(p.x, p.y); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr const value_type& at(const T p) const { return at(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr value_type& at(const T p) { return at(p.x, p.y); }

		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr bool inside(const T p) const noexcept { return inside(p.x, p.y); }
		template<typename T, std::enable_if_t<std::is_integral_v<typename T::value_type>, std::nullptr_t> = nullptr>
		NODISCARD inline constexpr bool outside(const T p) const noexcept { return outside(p.x, p.y); }

	};

}
