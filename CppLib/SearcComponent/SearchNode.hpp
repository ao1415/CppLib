#pragma once

#include "Memo.hpp"

namespace alib::Search::Lib {

	template<class Config>
	struct SearchNode {
		using SearchMethod = typename Config::SearchMethod;
		using pointer = SearchNode*;

		SearchNode() = default;
		SearchNode(pointer parent, const SearchMethod& arg) noexcept : searchArgument(arg) {
			assert(parent != nullptr);
			this->parent = parent;
			parent->addRef();
			depth = parent->depth + 1;
		}

		/** @brief 参照カウント */
		int ref = 1;
		/** @brief ノード深度 */
		int depth = 0;
		/** @brief 親ノードのポインタ */
		pointer parent = nullptr;
		/** @brief 探査引数 */
		SearchMethod searchArgument{};
		/** @brief ノード遷移パッチ */
		Memo::Patch patch{};

		void addRef() noexcept { ref++; }
		void subRef() noexcept { ref--; }
	};

	template<class Config>
	class SearchNodePool {
	public:
		using pointer = typename SearchNode<Config>::pointer;
	private:
		PagingMemory<SearchNode<Config>, 16_K> pool{};
		pointer top = nullptr;
	public:
		NODISCARD pointer alloc() {
			if (top != nullptr) {
				pointer ptr = top;
				top = ptr->parent;
				return ptr;
			}
			else {
				return pool.allocate(1);
			}
		}
		void release(pointer ptr) noexcept {
			ptr->parent = top;
			top = ptr;
		}
	};

}
