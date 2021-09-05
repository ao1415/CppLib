#pragma once
#include <array>

namespace alib {

	/**
	 * @brief ŒÅ’è’·2ŽŸŒ³”z—ñ
	 * @tparam Type ”z—ñŒ^
	*/
	template<typename Type, size_t Width, size_t Height>
	class FixedGrid : public std::array<Type, Width* Height>{
	private:

		using base = std::array<Type, Width* Height>;

	public:

		constexpr FixedGrid() : base() {}
		constexpr FixedGrid(const FixedGrid& other) = default;
		constexpr FixedGrid(FixedGrid&& other) = default;
		constexpr FixedGrid& operator =(const FixedGrid& other) = default;
		constexpr FixedGrid& operator =(FixedGrid&& other) = default;

		[[nodiscard]] inline constexpr const Type& operator()(int x, int y) const { return base::operator[](y* Width + x); }
		[[nodiscard]] inline constexpr Type& operator()(int x, int y) { return base::operator[](y* Width + x); }
		[[nodiscard]] inline constexpr const Type& operator()(size_t x, size_t y) const { return base::operator[](y* Width + x); }
		[[nodiscard]] inline constexpr Type& operator()(size_t x, size_t y) { return base::operator[](y* Width + x); }

		[[nodiscard]] inline constexpr const Type& at(int x, int y) const { return base::at(y * Width + x); }
		[[nodiscard]] inline constexpr Type& at(int x, int y) { return base::at(y * Width + x); }
		[[nodiscard]] inline constexpr const Type& at(size_t x, size_t y) const { return base::at(y * Width + x); }
		[[nodiscard]] inline constexpr Type& at(size_t x, size_t y) { return base::at(y * Width + x); }

		[[nodiscard]] inline constexpr const Type& operator()(const Point p) const { return operator()(p.x, p.y); }
		[[nodiscard]] inline constexpr Type& operator()(const Point p) { return operator()(p.x, p.y); }

		[[nodiscard]] inline constexpr const Type& at(const Point p) const { return at(p.x, p.y); }
		[[nodiscard]] inline constexpr Type& at(const Point p) { return at(p.x, p.y); }

		[[nodiscard]] inline constexpr size_t width() const { return Width; }
		[[nodiscard]] inline constexpr size_t height() const { return Height; }

		[[nodiscard]] inline constexpr bool inside(size_t x, size_t y) const { return (0 <= x && x < Width && 0 <= y && y < Height); }
		[[nodiscard]] inline constexpr bool outside(size_t x, size_t y) const { return (0 > x || x >= Width || 0 > y || y >= Height); }

	};

}
