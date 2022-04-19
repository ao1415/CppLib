#pragma once

#include <array>
#include <tuple>
#include <utility>

namespace alib {
	template<size_t Size, typename Type = uint64_t>
	class Zobrist {
	public:
		using value_type = Type;
	private:
		std::array<value_type, Size> hash{};

		constexpr uint64_t rotl(const uint64_t x, int k) const noexcept {
			return (x << k) | (x >> (64 - k));
		}
		constexpr std::tuple<uint64_t, uint64_t, value_type> next(const uint64_t s0, uint64_t s1) const noexcept {
			const uint64_t result = rotl(s0 + s1, 17) + s0;
			s1 ^= s0;
			const uint64_t t1 = rotl(s0, 49) ^ s1 ^ (s1 << 21);
			const uint64_t t2 = rotl(s1, 28);
			const uint64_t t3 = result;
			return std::make_tuple(t1, t2, t3);
		}
	public:
		constexpr Zobrist() noexcept {
			uint64_t s0 = 0x2bd7a6a6e99c2ddc;
			uint64_t s1 = 0x0992ccaf6a6fca05;
			for (auto& v : hash) {
				const auto ret = next(s0, s1);
				s0 = std::get<0>(ret);
				s1 = std::get<1>(ret);
				v = static_cast<value_type>(std::get<2>(ret));
			}
		}
		constexpr value_type operator[](const size_t i) const noexcept {
			return hash[i];
		}
	};
}
