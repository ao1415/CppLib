#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include "../Basic.hpp"

namespace alib::Search::Lib {

	template <class Type, class Compare = std::less<typename std::vector<Type>::value_type>>
	class ExPriorityQueue {
	public:
		using container_type = std::vector<Type>;
		using value_compare = Compare;

		using value_type = typename container_type::value_type;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using size_type = typename container_type::size_type;

		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		static_assert(std::is_same_v<Type, value_type>);
	protected:
		container_type c{};
		value_compare comp{};
	public:
		ExPriorityQueue() = default;
		explicit ExPriorityQueue(const value_compare& pred) noexcept(std::is_nothrow_default_constructible_v<container_type>&& std::is_nothrow_copy_constructible_v<value_compare>) : c(), comp(pred) {}
		ExPriorityQueue(const value_compare& pred, const container_type& cont) : c(cont), comp(pred) {
			std::make_heap(c.begin(), c.end(), comp);
		}
		ExPriorityQueue(const value_compare& pred, container_type&& cont) : c(std::move(cont)), comp(pred) {
			std::make_heap(c.begin(), c.end(), comp);
		}

		template <class Ite>
		ExPriorityQueue(Ite first, Ite last, const value_compare& pred, const container_type& cont) : c(cont), comp(pred) {
			c.insert(c.end(), first, last);
			std::make_heap(c.begin(), c.end(), comp);
		}
		template <class Ite>
		ExPriorityQueue(Ite first, Ite last) : c(first, last), comp() {
			std::make_heap(c.begin(), c.end(), comp);
		}
		template <class Ite>
		ExPriorityQueue(Ite first, Ite last, const value_compare& pred) : c(first, last), comp(pred) {
			std::make_heap(c.begin(), c.end(), comp);
		}
		template <class Ite>
		ExPriorityQueue(Ite first, Ite last, const value_compare& pred, container_type&& cont) : c(std::move(cont)), comp(pred) {
			c.insert(c.end(), first, last);
			std::make_heap(c.begin(), c.end(), comp);
		}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		explicit ExPriorityQueue(const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, const Alloc&>&& is_nothrow_default_constructible_v<value_compare>) : c(alloc), comp() {}
		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		ExPriorityQueue(const value_compare& pred, const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, const Alloc&>&& is_nothrow_copy_constructible_v<value_compare>) : c(alloc), comp(pred) {}
		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		ExPriorityQueue(const value_compare& pred, const container_type& cont, const Alloc& alloc) : c(cont, alloc), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		ExPriorityQueue(const value_compare& pred, container_type&& cont, const Alloc& alloc) : c(std::move(cont), alloc), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		ExPriorityQueue(const ExPriorityQueue& right, const Alloc& alloc) : c(right.c, alloc), comp(right.comp) {}
		template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
		ExPriorityQueue(ExPriorityQueue&& right, const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, container_type, const Alloc&>&& std::is_nothrow_move_constructible_v<value_compare>) : c(std::move(right.c), alloc), comp(std::move(right.comp)) {}

		NODISCARD inline iterator begin() noexcept { return c.begin(); }
		NODISCARD inline iterator end() noexcept { return c.end(); }
		NODISCARD inline const_iterator begin() const noexcept { return c.begin(); }
		NODISCARD inline const_iterator end() const noexcept { return c.end(); }
		NODISCARD inline const_iterator cbegin() const noexcept { return c.cbegin(); }
		NODISCARD inline const_iterator cend() const noexcept { return c.cend(); }
		NODISCARD inline reverse_iterator rbegin() noexcept { return c.rbegin(); }
		NODISCARD inline reverse_iterator rend() noexcept { return c.rend(); }
		NODISCARD inline const_reverse_iterator rbegin() const noexcept { return c.rbegin(); }
		NODISCARD inline const_reverse_iterator rend() const noexcept { return c.rend(); }
		NODISCARD inline const_reverse_iterator crbegin() const noexcept { return c.crbegin(); }
		NODISCARD inline const_reverse_iterator crend() const noexcept { return c.crend(); }

		NODISCARD inline bool empty() const noexcept(noexcept(c.empty())) { return c.empty(); }
		NODISCARD inline size_type size() const noexcept(noexcept(c.size())) { return c.size(); }
		NODISCARD inline const_reference top() const noexcept(noexcept(c.front())) { return c.front(); }

		inline void push(const value_type& val) {
			c.push_back(val);
			std::push_heap(c.begin(), c.end(), comp);
		}
		inline void push(value_type&& val) {
			c.push_back(std::move(val));
			std::push_heap(c.begin(), c.end(), comp);
		}
		template <class... Args>
		inline void emplace(Args&&... val) {
			c.emplace_back(std::forward<Args>(val)...);
			std::push_heap(c.begin(), c.end(), comp);
		}
		inline void pop() {
			std::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}

		inline void reserve(const size_type n) { return c.reseve(n); }
		inline void clear() noexcept { c.clear(); }

		inline void swap(ExPriorityQueue& right) noexcept {
			using namespace std;
			swap(c, right.c);
			swap(comp, right.comp);
		}
	};

}
