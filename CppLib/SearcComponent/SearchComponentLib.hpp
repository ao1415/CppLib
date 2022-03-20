#pragma once

#include "../Basic.hpp"

namespace alib::Search {
	using size_type = size_t;

	namespace Lib {
		NODISCARD constexpr size_type operator"" _K(unsigned long long k) noexcept {
			return k << 10;
		}
		NODISCARD constexpr size_type operator"" _M(unsigned long long m) noexcept {
			return m << 20;
		}
	}
}
