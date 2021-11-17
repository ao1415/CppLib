#pragma once

#include "Basic.hpp"
#include <iostream>
#include <string>
#include <cmath>

namespace alib {

	/**
	 * @brief xyÀ•W
	 * @tparam Type À•WŒ^
	*/
	template<typename Type>
	struct BasicPoint {

		/** @brief ã•ûŒü */
		inline static constexpr BasicPoint Up() { return BasicPoint(0, -1); }
		/** @brief ‰º•ûŒü */
		inline static constexpr BasicPoint Down() { return BasicPoint(0, 1); }
		/** @brief ¶•ûŒü */
		inline static constexpr BasicPoint Left() { return BasicPoint(-1, 0); }
		/** @brief ‰E•ûŒü */
		inline static constexpr BasicPoint Right() { return BasicPoint(1, 0); }
		/** @brief ‘S•ûŒü */
		inline static constexpr auto Direction() {
			return std::array<BasicPoint, 4>{ {Up(), Right(), Down(), Left()} };
		}

		/** @brief Œ^ */
		using value_type = Type;

		/** @brief x */
		value_type x = value_type();
		/** @brief y */
		value_type y = value_type();

		NODISCARD BasicPoint() = default;
		NODISCARD BasicPoint(const BasicPoint&) = default;
		NODISCARD constexpr BasicPoint(value_type x, value_type y) noexcept : x(x), y(y) {}

		constexpr BasicPoint& operator=(const BasicPoint&) = default;

		NODISCARD inline constexpr BasicPoint operator+(const BasicPoint v) const noexcept { return BasicPoint(this->x + v.x, this->y + v.y); }
		NODISCARD inline constexpr BasicPoint operator-(const BasicPoint v) const noexcept { return BasicPoint(this->x - v.x, this->y - v.y); }

		inline constexpr void operator+=(const BasicPoint v) noexcept { this->x += v.x; this->y += v.y; }
		inline constexpr void operator-=(const BasicPoint v) noexcept { this->x -= v.x; this->y -= v.y; }

		NODISCARD inline constexpr bool operator<(const BasicPoint v) const noexcept { if (this->y != v.y) return this->y < v.y; else return this->x < v.x; }

		NODISCARD inline constexpr bool operator==(const BasicPoint v) const noexcept { return this->x == v.x && this->y == v.y; }
		NODISCARD inline constexpr bool operator!=(const BasicPoint v) const noexcept { return this->x != v.x || this->y != v.y; }

		NODISCARD inline constexpr bool equals(const BasicPoint v) const noexcept { return *this == v; }
		NODISCARD inline constexpr bool equals(const value_type x, const value_type y) const noexcept { return (this->x == x && this->y == y); }

		NODISCARD inline constexpr bool isZero() const noexcept {
			const auto zero = value_type();
			return (this->x == zero && this->y == zero);
		}
		inline constexpr void clear() noexcept {
			const auto zero = value_type();
			this->x = zero;
			this->y = zero;
		}

		inline constexpr void set(const value_type x, const value_type y) noexcept { this->x = x; this->y = y; }
		inline constexpr void set(const BasicPoint v) noexcept { this->x = v.x; this->y = v.y; }

		NODISCARD inline constexpr value_type dot() const noexcept { return (this->x * this->x + this->y * this->y); }
		NODISCARD inline constexpr value_type dot(const BasicPoint v) const noexcept {
			const auto dx = this->x - v.x;
			const auto dy = this->y - v.y;
			return (dx * dx + dy * dy);
		}
		NODISCARD inline static constexpr value_type dot(const BasicPoint v1, const BasicPoint v2) noexcept {
			const auto dx = v1.x - v2.x;
			const auto dy = v1.y - v2.y;
			return (dx * dx + dy * dy);
		}

		NODISCARD inline constexpr value_type length() const noexcept { return narrow_cast<value_type>(std::sqrt(this->x * this->x + this->y * this->y)); }
		NODISCARD inline constexpr value_type length(const BasicPoint v) const noexcept {
			const auto dx = this->x - v.x;
			const auto dy = this->y - v.y;
			return narrow_cast<value_type>(std::sqrt(dx * dx + dy * dy));
		}
		NODISCARD inline static constexpr value_type length(const BasicPoint v1, const BasicPoint v2) noexcept {
			const auto dx = v1.x - v2.x;
			const auto dy = v1.y - v2.y;
			return narrow_cast<value_type>(std::sqrt(dx * dx + dy * dy));
		}

		NODISCARD inline constexpr value_type manhattanLength() const noexcept { return (this->x < 0 ? -this->x : this->x) + (this->y < 0 ? -this->y : this->y); }
		NODISCARD inline constexpr value_type manhattanLength(const BasicPoint v) const noexcept {
			const auto dx = this->x - v.x;
			const auto dy = this->y - v.y;
			return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
		}
		NODISCARD inline static constexpr value_type manhattanLength(const BasicPoint v1, const BasicPoint v2) noexcept {
			const auto dx = v1.x - v2.x;
			const auto dy = v1.y - v2.y;
			return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
		}

		NODISCARD inline constexpr size_t hash() const noexcept {
			constexpr size_t offset_basis = 14695981039346656037ULL;
			constexpr size_t FNV_prime = 1099511628211ULL;

			constexpr size_t size = sizeof(BasicPoint);
			const std::byte* const bytes = reinterpret_cast<const std::byte* const>(this);

			size_t result = offset_basis;
			forange(idx, size) {
				result ^= static_cast<size_t>(bytes[idx]);
				result *= FNV_prime;
			}

			return result;
		}

		NODISCARD inline std::string toString() const { return "(" + std::to_string(x) + "," + std::to_string(y) + ")"; }

	};

	/** @brief ®”Œ^À•W*/
	using Point = BasicPoint<int>;
	/** @brief ”{¸“x®”Œ^À•W*/
	using PointL = BasicPoint<long long int>;
	/** @brief ¬”Œ^À•W*/
	using Vec2 = BasicPoint<double>;
	/** @brief ”{¸“x¬”Œ^À•W*/
	using Vec2L = BasicPoint<long double>;

	template<typename Type>
	inline std::istream& operator>>(std::istream& is, BasicPoint<Type>& v) { is >> v.x >> v.y; return is; }
	template<typename Type>
	inline std::ostream& operator<<(std::ostream& os, const BasicPoint<Type> v) { os << v.x << " " << v.y; return os; }

	inline std::istream& operator>>(std::istream& is, BasicPoint<char>& v) {
		int x, y;
		is >> x >> y;
		v.x = narrow_cast<char>(x);
		v.y = narrow_cast<char>(y);
		return is;
	}
	inline std::ostream& operator<<(std::ostream& os, const BasicPoint<char> v) { os << static_cast<int>(v.x) << " " << static_cast<int>(v.y); return os; }

	inline std::istream& operator>>(std::istream& is, BasicPoint<unsigned char>& v) {
		int x, y;
		is >> x >> y;
		v.x = narrow_cast<unsigned char>(x);
		v.y = narrow_cast<unsigned char>(y);
		return is;
	}
	inline std::ostream& operator<<(std::ostream& os, const BasicPoint<unsigned char> v) { os << static_cast<int>(v.x) << " " << static_cast<int>(v.y); return os; }

	inline std::istream& operator>>(std::istream& is, BasicPoint<std::byte>& v) {
		int x, y;
		is >> x >> y;
		v.x = static_cast<std::byte>(x);
		v.y = static_cast<std::byte>(y);
		return is;
	}
	inline std::ostream& operator<<(std::ostream& os, const BasicPoint<std::byte> v) { os << static_cast<int>(v.x) << " " << static_cast<int>(v.y); return os; }
}
