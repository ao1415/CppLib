#pragma once

#include <type_traits>

// [0, END)でCOUNTERを増加させる
#define forange(COUNTER, END) forstep_type(std::decay_t<decltype(END)>, COUNTER, std::decay_t<decltype(END)>(), END)
// [0, END)で指定された型のCOUNTERを増加させる
#define forange_type(COUNTER_TYPE, COUNTER, END) forstep_type(COUNTER_TYPE, COUNTER, std::decay_t<COUNTER_TYPE>(), END)
// [BEGIN, END)でCOUNTERを増加させる
#define forstep(COUNTER, BEGIN, END) forstep_type(std::decay_t<decltype(BEGIN)>, COUNTER, BEGIN, END)
// [BEGIN, END)で指定された型のCOUNTERを増加させる
#define forstep_type(COUNTER_TYPE, COUNTER, BEGIN, END) for (COUNTER_TYPE COUNTER = static_cast<COUNTER_TYPE>(BEGIN), _loop_end_##COUNTER = static_cast<COUNTER_TYPE>(END); COUNTER < _loop_end_##COUNTER; COUNTER++)

namespace alib {

	/**
	 * @brief 指定されたサイズから、確保するサイズを算出する
	 * @tparam Size 必要なコンテナサイズ
	 * @return 確保するコンテナサイズ
	*/
	template <size_t Size>
	constexpr auto ContainerAllocator() {
		size_t n = Size;
		if (n != 0) { n -= 1; }
		forange(lShift, 5) { n |= (n >> (1 << lShift)); }
		if (n != std::numeric_limits<decltype(n)>::max()) { n += 1; }
		return n;
	}

}
