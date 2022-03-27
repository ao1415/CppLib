#pragma once

#include <utility>
#include <type_traits>

#ifndef _MSC_VER
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define WARN_PUSH_DISABLE(x)
#define WARN_POP()
#else
#include <iso646.h>

#define LIKELY(x) x
#define UNLIKELY(x) x

#define WARN_PUSH_DISABLE(x) __pragma(warning(push))\
__pragma(warning(disable:x))
#define WARN_POP() __pragma(warning(pop))
#endif

// [0, END)でCOUNTERを増加させる
#define forange(COUNTER, END) forstep_type(std::decay_t<decltype(END)>, COUNTER, std::decay_t<decltype(END)>(), END)
// [0, END)で指定された型のCOUNTERを増加させる
#define forange_type(COUNTER_TYPE, COUNTER, END) forstep_type(COUNTER_TYPE, COUNTER, std::decay_t<COUNTER_TYPE>(), END)
// [BEGIN, END)でCOUNTERを増加させる
#define forstep(COUNTER, BEGIN, END) forstep_type(std::decay_t<decltype(BEGIN)>, COUNTER, BEGIN, END)
// [BEGIN, END)で指定された型のCOUNTERを増加させる
#define forstep_type(COUNTER_TYPE, COUNTER, BEGIN, END) \
WARN_PUSH_DISABLE(26496 26498)\
for (COUNTER_TYPE COUNTER = static_cast<COUNTER_TYPE>(BEGIN), _loop_end_##COUNTER = static_cast<COUNTER_TYPE>(END); COUNTER < _loop_end_##COUNTER; COUNTER++)\
WARN_POP()

#define NODISCARD [[nodiscard]]

namespace alib {

#ifdef _DEBUG
	/**
	 * @brief 警告なしで縮小キャストを行う
	 * @tparam U キャスト元型
	 * @param u キャストする値
	 * @return キャストされた値
	*/
	template <class T, class U>
	NODISCARD constexpr T narrow_cast(U&& u) noexcept {
		WARN_PUSH_DISABLE(26472);
		return static_cast<T>(std::forward<U>(u));
		WARN_POP();
	}
#else
#define narrow_cast static_cast
#endif

	/**
	 * @brief 指定されたサイズから、確保するサイズを算出する
	 * @tparam Size 必要なコンテナサイズ
	 * @return 確保するコンテナサイズ
	*/
	template <size_t Size>
	NODISCARD constexpr auto ContainerAllocator() noexcept {
		size_t n = Size;

		if (n != 0) {
			n -= 1;
		}
		forange(lShift, 5) {
			n |= (n >> (1 << lShift));
		}

		if (n != std::numeric_limits<decltype(n)>::max()) {
			n += 1;
		}
		return n;
	}

	/**
	 * @brief 標準入力ストリームから読み取る
	 * @tparam Type 読み取る型
	 * @return 読み取った値
	*/
	template <class Type>
	NODISCARD Type GetCin() {
		Type value{};
		std::cin >> value;
		return value;
	}

}
