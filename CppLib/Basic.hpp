#pragma once

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
