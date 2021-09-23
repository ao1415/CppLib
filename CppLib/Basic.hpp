#pragma once

#include <type_traits>
#include <utility>

// [0, END)でCOUNTERを増加させる
#define forange(COUNTER, END) forstep_type(std::decay_t<decltype(END)>, COUNTER, std::decay_t<decltype(END)>(), END)
// [0, END)で指定された型のCOUNTERを増加させる
#define forange_type(COUNTER_TYPE, COUNTER, END) forstep_type(COUNTER_TYPE, COUNTER, std::decay_t<COUNTER_TYPE>(), END)
// [BEGIN, END)でCOUNTERを増加させる
#define forstep(COUNTER, BEGIN, END) forstep_type(std::decay_t<decltype(BEGIN)>, COUNTER, BEGIN, END)
// [BEGIN, END)で指定された型のCOUNTERを増加させる
#define forstep_type(COUNTER_TYPE, COUNTER, BEGIN, END) \
__pragma(warning(push))\
__pragma(warning(disable:26496))\
for (COUNTER_TYPE COUNTER = static_cast<COUNTER_TYPE>(BEGIN), _loop_end_##COUNTER = static_cast<COUNTER_TYPE>(END); COUNTER < _loop_end_##COUNTER; COUNTER++)\
__pragma(warning(pop))\

#define NODISCARD [[nodiscard]]

template <class T, class U>
inline constexpr T narrow_cast(U&& u) noexcept {
#pragma warning(push)
#pragma warning(disable:26472)
	return static_cast<T>(std::forward<U>(u));
#pragma warning(pop)
}

namespace alib {

	/**
	 * @brief 指定されたサイズから、確保するサイズを算出する
	 * @tparam Size 必要なコンテナサイズ
	 * @return 確保するコンテナサイズ
	*/
	template <size_t Size>
	NODISCARD constexpr auto ContainerAllocator() noexcept {
		size_t n = Size;
		if (n != 0) { n -= 1; }
		forange(lShift, 5) { n |= (n >> (1 << lShift)); }
		if (n != std::numeric_limits<decltype(n)>::max()) { n += 1; }
		return n;
	}

}
