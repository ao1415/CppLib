#pragma once

#define forange(COUNTER, END) forstep_type(decltype(END), COUNTER, 0, END)
#define forange_type(COUNTER_TYPE, COUNTER, END) forstep_type(COUNTER_TYPE, COUNTER, 0, END)
#define forstep(COUNTER, BEGIN, END) forstep_type(decltype(BEGIN), COUNTER, BEGIN, END)
#define forstep_type(COUNTER_TYPE, COUNTER, BEGIN, END) for (COUNTER_TYPE COUNTER = static_cast<COUNTER_TYPE>(BEGIN), loop_end_##COUNTER = static_cast<COUNTER_TYPE>(END); COUNTER < loop_end_##COUNTER; COUNTER += 1)

namespace alib {

	constexpr size_t containerAllocator(size_t n) {
		n = n <= 0 ? 0 : n - 1;
		n |= (n >> 1);
		n |= (n >> 2);
		n |= (n >> 4);
		n |= (n >> 8);
		n |= (n >> 16);
		return n + 1;
	}

}
