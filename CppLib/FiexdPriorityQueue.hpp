#pragma once

#include <queue>
#include "MemoryPool.hpp"

namespace alib {

	template<class Type, size_t Size, class _Pr = std::less<typename std::vector<Type>::value_type>>
	class FiexdPriorityQueue {
	private:

		std::priority_queue<Type, std::vector<Type>, _Pr> m_queue;

		MemoryPool pool<Type, Size> m_pool;

	public:



	};

}
